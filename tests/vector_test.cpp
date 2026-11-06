#include <nojo/vector.hpp>
#include <gtest/gtest.h>

TEST(VectorTest, EmptyContainer) {
    nojo::vector<int> vec;
    
    EXPECT_TRUE(vec.empty());
    EXPECT_EQ(vec.size(), 0);
    EXPECT_EQ(vec.capacity(), 0);
    EXPECT_EQ(vec.begin(), vec.end());
}

TEST(VectorTest, PushBackAndCapacityGrowth) {
    nojo::vector<int> vec;
    
    vec.push_back(10);
    EXPECT_EQ(vec.size(), 1);
    EXPECT_EQ(vec.capacity(), 1);
    EXPECT_EQ(vec[0], 10);

    vec.push_back(20);
    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec.capacity(), 2);
    
    vec.push_back(30);
    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(vec.capacity(), 4); 
    
    EXPECT_EQ(vec[0], 10);
    EXPECT_EQ(vec[1], 20);
    EXPECT_EQ(vec[2], 30);
}

TEST(VectorTest, CopyConstructorIsDeep) {
    nojo::vector<int> original;
    original.push_back(1);
    original.push_back(2);
    original.push_back(3);

    nojo::vector<int> copy(original);

    EXPECT_EQ(copy.size(), 3);
    EXPECT_EQ(copy[0], 1);
    EXPECT_EQ(copy[2], 3);

    EXPECT_NE(original.begin(), copy.begin());

    original[0] = 99;
    EXPECT_EQ(copy[0], 1);
}

TEST(VectorTest, EmplaceBackStrings) {
    nojo::vector<std::string> vec;
    
    vec.emplace_back("Hello");
    vec.emplace_back(5, 'A');

    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec[0], "Hello");
    EXPECT_EQ(vec[1], "AAAAA");
}