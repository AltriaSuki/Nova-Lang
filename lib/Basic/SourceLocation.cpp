#include "nova/Basic/SourceLocation.hpp"
#include<cassert>
#include <cstdint>

namespace nova {
    SourceLocation SourceLocation::create(uint16_t file_id,uint32_t offset){
        assert(file_id < (1U << FILE_ID_BITS) && "file_id exceeds limit");
        assert(offset < (1U << OFFSET_BITS) && "offset exceeds limit");
        SourceLocation loc;
        loc.raw_encoding_ = (static_cast<int32_t>(file_id) << OFFSET_BITS) | offset;
        return loc;
    }
    SourceLocation SourceLocation::get_offset_location(int32_t offset) const {
        auto new_offset = static_cast<int32_t>(get_offset()) + offset;
        assert(new_offset >= 0 && new_offset < static_cast<int32_t>(1U << OFFSET_BITS) &&
               "new offset exceeds limit");
        return create(get_file_id(),static_cast<uint32_t>(new_offset));
    }
}