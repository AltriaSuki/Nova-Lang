#include "nova/Basic/IdentifierTable.hpp"
#include "nova/Basic/SourceManager.hpp"
#include "nova/Lex/Lexer.hpp"
#include "nova/Lex/Token.hpp"

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
//this benchmark measures the performance of the Lexer
namespace {

struct Options {
    std::string file_path;
    std::size_t bytes = 1 * 1024 * 1024;
    std::uint32_t repeat = 100;
    std::uint32_t warmup = 3;
};

void print_usage(std::ostream& os, const char* argv0) {
    os << "Usage: " << argv0 << " [--file PATH] [--bytes N] [--repeat N] [--warmup N]\n"
          "\n"
          "Lexer micro-benchmark.\n"
          "\n"
          "Examples:\n"
          "  " << argv0 << " --bytes 1000000 --repeat 200\n"
          "  " << argv0 << " --file examples/hello.nova --repeat 1000\n";
}

bool parse_u32(std::string_view s, std::uint32_t& out) {
    if (s.empty()) {
        return false;
    }
    std::uint64_t value = 0;
    for (char c : s) {
        if (c < '0' || c > '9') {
            return false;
        }
        value = value * 10 + static_cast<std::uint64_t>(c - '0');
        if (value > std::numeric_limits<std::uint32_t>::max()) {
            return false;
        }
    }
    out = static_cast<std::uint32_t>(value);
    return true;
}

bool parse_usize(std::string_view s, std::size_t& out) {
    if (s.empty()) {
        return false;
    }
    std::uint64_t value = 0;
    for (char c : s) {
        if (c < '0' || c > '9') {
            return false;
        }
        value = value * 10 + static_cast<std::uint64_t>(c - '0');
        if (value > std::numeric_limits<std::size_t>::max()) {
            return false;
        }
    }
    out = static_cast<std::size_t>(value);
    return true;
}

bool parse_args(int argc, char** argv, Options& opts) {
    for (int i = 1; i < argc; ++i) {
        std::string_view arg(argv[i]);
        auto take_value = [&](std::string_view flag) -> std::string_view {
            if (i + 1 >= argc) {
                std::cerr << "Missing value for " << flag << "\n";
                return {};
            }
            return std::string_view(argv[++i]);
        };

        if (arg == "--help" || arg == "-h") {
            print_usage(std::cout, argv[0]);
            return false;
        }
        if (arg == "--file") {
            opts.file_path = std::string(take_value("--file"));
            if (opts.file_path.empty()) {
                return false;
            }
            continue;
        }
        if (arg == "--bytes") {
            std::string_view value = take_value("--bytes");
            if (value.empty() || !parse_usize(value, opts.bytes) || opts.bytes == 0) {
                std::cerr << "Invalid --bytes value: " << value << "\n";
                return false;
            }
            continue;
        }
        if (arg == "--repeat") {
            std::string_view value = take_value("--repeat");
            if (value.empty() || !parse_u32(value, opts.repeat) || opts.repeat == 0) {
                std::cerr << "Invalid --repeat value: " << value << "\n";
                return false;
            }
            continue;
        }
        if (arg == "--warmup") {
            std::string_view value = take_value("--warmup");
            if (value.empty() || !parse_u32(value, opts.warmup)) {
                std::cerr << "Invalid --warmup value: " << value << "\n";
                return false;
            }
            continue;
        }

        std::cerr << "Unknown argument: " << arg << "\n";
        return false;
    }

    return true;
}

std::string read_file(std::string_view path) {
    std::ifstream in(std::string(path), std::ios::binary);
    if (!in) {
        return {};
    }
    std::string data;
    in.seekg(0, std::ios::end);
    std::streamsize size = in.tellg();
    if (size < 0) {
        return {};
    }
    data.resize(static_cast<std::size_t>(size));
    in.seekg(0, std::ios::beg);
    in.read(data.data(), size);
    if (!in) {
        return {};
    }
    return data;
}

std::string generate_source(std::size_t target_bytes) {
    constexpr std::string_view kChunk =
        "func main() {\n"
        "  let x = 42;\n"
        "  let y = 0x2A;\n"
        "  let z = \"Hello, World!\";\n"
        "  let c = 'a';\n"
        "  // comment\n"
        "  x = x + 1;\n"
        "}\n";

    std::string out;
    out.reserve(target_bytes + static_cast<std::size_t>(kChunk.size()));
    while (out.size() < target_bytes) {
        out.append(kChunk);
    }
    return out;
}

struct RunResult {
    std::uint64_t token_count = 0;
    std::uint64_t checksum = 0;
};

RunResult lex_all(nova::SourceManager& sm, nova::IdentifierTable& ids, std::uint16_t file_id) {
    nova::Lexer lexer(sm, ids, file_id);
    nova::Token token;

    RunResult result{};
    while (true) {
        lexer.lex(token);
        ++result.token_count;
        result.checksum += static_cast<std::uint64_t>(token.get_length());
        result.checksum += static_cast<std::uint64_t>(
            static_cast<std::underlying_type_t<nova::TokenKind>>(token.get_kind()));
        if (token.is(nova::TokenKind::eof)) {
            break;
        }
    }
    return result;
}

} // namespace

int main(int argc, char** argv) {
    Options opts;
    if (!parse_args(argc, argv, opts)) {
        return 1;
    }

    std::string input;
    std::string virtual_filename = "<generated>";
    if (!opts.file_path.empty()) {
        input = read_file(opts.file_path);
        if (input.empty()) {
            std::cerr << "Failed to read file: " << opts.file_path << "\n";
            return 2;
        }
        virtual_filename = opts.file_path;
    } else {
        input = generate_source(opts.bytes);
    }

    const std::size_t input_bytes = input.size();

    nova::SourceManager sm;
    const std::uint16_t file_id = sm.add_file(std::move(virtual_filename), std::move(input));

    nova::IdentifierTable ids;
    for (std::uint32_t i = 0; i < opts.warmup; ++i) {
        (void)lex_all(sm, ids, file_id);
    }

    std::uint64_t total_tokens = 0;
    std::uint64_t total_checksum = 0;
    const auto start = std::chrono::steady_clock::now();
    for (std::uint32_t i = 0; i < opts.repeat; ++i) {
        RunResult r = lex_all(sm, ids, file_id);
        total_tokens += r.token_count;
        total_checksum += r.checksum;
    }
    const auto end = std::chrono::steady_clock::now();

    const std::chrono::duration<double> elapsed = end - start;
    const double seconds = elapsed.count();
    const double total_mb = static_cast<double>(input_bytes) * static_cast<double>(opts.repeat) /
                            (1024.0 * 1024.0);
    const double mb_per_sec = (seconds > 0.0) ? (total_mb / seconds) : 0.0;
    const double tokens_per_sec =
        (seconds > 0.0) ? (static_cast<double>(total_tokens) / seconds) : 0.0;

    std::cout << "lexer: bytes=" << input_bytes << " repeat=" << opts.repeat
              << " warmup=" << opts.warmup << "\n";
    std::cout << "elapsed: " << seconds << " s\n";
    std::cout << "throughput: " << mb_per_sec << " MiB/s\n";
    std::cout << "tokens: " << total_tokens << " (" << tokens_per_sec << " tokens/s)\n";
    std::cout << "checksum: " << total_checksum << "\n";
    return 0;
}
