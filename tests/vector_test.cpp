#include <gtest/gtest.h>
#include <nojo/vector.hpp>

TEST(VectorTest, DefaultConstruct) {
    nojo::vector<int> vec;
    EXPECT_TRUE(vec.empty());
    EXPECT_EQ(vec.size(), 0);
}