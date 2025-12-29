#pragma once
#include "SourceLocation.hpp"
#include <string>
#include <string_view>
#include <vector>
#include <memory>

namespace nova {

struct FileEntry {
    std::string filename;
    std::string content;
    // every line's starting offset in content
    std::vector<uint32_t> line_offsets;
    uint16_t file_id;

    FileEntry(uint16_t id, std::string name, std::string data);

private:
    void compute_line_offsets();
};
// manages all source files and provides utilities to query source locations
class SourceManager {
private:
    std::vector<std::unique_ptr<FileEntry>> files_;

public:
    SourceManager() = default;
    ~SourceManager() = default;

    uint16_t add_file(std::string filename, std::string content);
    const FileEntry* get_file(uint16_t file_id) const;

    char get_char(SourceLocation loc) const;
    std::string_view get_text(SourceRange range) const;

    void get_line_column(SourceLocation loc, uint32_t& line, uint32_t& column) const;
    std::string_view get_filename(SourceLocation loc) const;
    std::string format_location(SourceLocation loc) const;

};

} // namespace nova
