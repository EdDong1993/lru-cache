#include "lru_cache.h"

#include <gtest/gtest.h>

namespace test {

TEST(lru_cache, without_deleter) {
    lru_cache<int, std::string> cache(4);

    cache.put(1, "one");
    cache.put(2, "two");
    cache.put(3, "three");
    cache.put(4, "four");
    EXPECT_TRUE(cache.exist(1));
    EXPECT_TRUE(cache.exist(2));
    EXPECT_TRUE(cache.exist(3));
    EXPECT_TRUE(cache.exist(4));
    std::string val;
    EXPECT_TRUE(cache.get(1, val));
    EXPECT_EQ(val, "one");
    cache.put(5, "five");
    EXPECT_TRUE(cache.exist(1));
    EXPECT_FALSE(cache.exist(2));
    EXPECT_EQ(cache.size(), 4);
}

TEST(lru_cache, with_deletor) {
    lru_cache<int, int *, std::default_delete<int>> cache(4);

    cache.put(1, new int(1));
    cache.put(2, new int(2));
    cache.put(3, new int(3));
    cache.put(4, new int(4));
    EXPECT_TRUE(cache.exist(1));
    EXPECT_TRUE(cache.exist(2));
    EXPECT_TRUE(cache.exist(3));
    EXPECT_TRUE(cache.exist(4));
    int *p{nullptr};
    EXPECT_TRUE(cache.get(1, p));
    EXPECT_EQ(*p, 1);
    cache.put(5, new int(5));
    EXPECT_TRUE(cache.exist(1));
    EXPECT_FALSE(cache.exist(2));
    EXPECT_EQ(cache.size(), 4);
}

}  // namespace test