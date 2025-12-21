#pragma once
#include <cstdint>

namespace nova {

/// Compact source location (32-bit encoded)
class SourceLocation {
private:
    //such design to save memory usage
    //assuming max 4096 files 12 bits, and file size not exceeding 1MB 20bits
    //this variables is [file_id|offset]
    uint32_t raw_encoding_;

    static constexpr uint32_t OFFSET_BITS = 20;
    static constexpr uint32_t FILE_ID_BITS = 12;
    //20 bits for offset, 12 bits for file id
    static constexpr uint32_t OFFSET_MASK = (1U << OFFSET_BITS) - 1;

public:
    SourceLocation() : raw_encoding_(0) {}

    static SourceLocation create(uint16_t file_id, uint32_t offset);

    uint16_t get_file_id() const { return raw_encoding_ >> OFFSET_BITS; }
    uint32_t get_offset() const { return raw_encoding_ & OFFSET_MASK; }
    uint32_t get_raw_encoding() const { return raw_encoding_; }

    bool is_valid() const { return raw_encoding_ != 0; }
    bool is_invalid() const { return raw_encoding_ == 0; }

    SourceLocation get_offset_location(int32_t offset) const;

    bool operator==(SourceLocation other) const {
        return raw_encoding_ == other.raw_encoding_;
    }
    bool operator!=(SourceLocation other) const { return !(*this == other); }
    bool operator<(SourceLocation other) const {
        return raw_encoding_ < other.raw_encoding_;
    }

    static SourceLocation invalid() { return SourceLocation(); }
};

/// Source range [begin, end)
class SourceRange {
private:
    SourceLocation begin_;
    SourceLocation end_;

public:
    SourceRange() = default;
    SourceRange(SourceLocation loc) : begin_(loc), end_(loc) {}
    SourceRange(SourceLocation begin, SourceLocation end)
        : begin_(begin), end_(end) {}

    SourceLocation begin() const { return begin_; }
    SourceLocation end() const { return end_; }

    bool is_valid() const { return begin_.is_valid() && end_.is_valid(); }
};

} // namespace nova
