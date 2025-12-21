#include "nova/Basic/SourceLocation.hpp"
#include <gtest/gtest.h>

TEST(SourceLocationTest, CreateAndAccessors) {
    uint16_t file_id =42;
    uint32_t offset = 123456;
    using namespace nova;
    SourceLocation loc = SourceLocation::create(file_id,offset);
    EXPECT_EQ(loc.get_file_id(), file_id);
    EXPECT_EQ(loc.get_offset(), offset);
    EXPECT_TRUE(loc.is_valid());
    EXPECT_FALSE(loc.is_invalid());
}

TEST(SourceLocationTest, OffsetLocation) {
    uint16_t file_id = 5;
    uint32_t offset = 1000;
    using namespace nova;
    SourceLocation loc = SourceLocation::create(file_id, offset);
    int32_t offset_delta = 500;
    SourceLocation new_loc = loc.get_offset_location(offset_delta);
    EXPECT_EQ(new_loc.get_file_id(), file_id);
    EXPECT_EQ(new_loc.get_offset(), offset + offset_delta);
}


//test the invalid location
TEST(SourceLocationTest, InvalidLocation) {
    using namespace nova;
    SourceLocation loc = SourceLocation::invalid();
    EXPECT_EQ(loc.get_file_id(), 0);
    EXPECT_EQ(loc.get_offset(), 0);
    EXPECT_FALSE(loc.is_valid());
    EXPECT_TRUE(loc.is_invalid());
}