#include <gtest/gtest.h>

#include "../src/lru_cache.h"

TEST(LRUCacheTest, NewCacheIsEmptyAndMissingKeysReturnMinusOne) {
  LRUCache cache(3);

  EXPECT_EQ(cache.size(), 0);
  EXPECT_FALSE(cache.exists(1));
  EXPECT_EQ(cache.get(1), -1);
  EXPECT_EQ(cache.size(), 0);
}

TEST(LRUCacheTest, PutStoresValueAndMarksKeyAsExisting) {
  LRUCache cache(2);

  cache.put(1, 10);

  EXPECT_TRUE(cache.exists(1));
  EXPECT_EQ(cache.size(), 1);
  EXPECT_EQ(cache.get(1), 10);
}

TEST(LRUCacheTest, PutUpdatesExistingKeyWithoutIncreasingSize) {
  LRUCache cache(2);

  cache.put(1, 10);
  cache.put(1, 20);

  EXPECT_EQ(cache.size(), 1);
  EXPECT_TRUE(cache.exists(1));
  EXPECT_EQ(cache.get(1), 20);
}

TEST(LRUCacheTest, PutEvictsLeastRecentlyInsertedKeyWhenCapacityIsReached) {
  LRUCache cache(2);

  cache.put(1, 10);
  cache.put(2, 20);
  cache.put(3, 30);

  EXPECT_EQ(cache.size(), 2);
  EXPECT_FALSE(cache.exists(1));
  EXPECT_TRUE(cache.exists(2));
  EXPECT_TRUE(cache.exists(3));
  EXPECT_EQ(cache.get(1), -1);
  EXPECT_EQ(cache.get(2), 20);
  EXPECT_EQ(cache.get(3), 30);
}

TEST(LRUCacheTest, GetRefreshesKeySoNextPutEvictsOlderKey) {
  LRUCache cache(2);

  cache.put(1, 10);
  cache.put(2, 20);
  EXPECT_EQ(cache.get(1), 10);
  cache.put(3, 30);

  EXPECT_TRUE(cache.exists(1));
  EXPECT_FALSE(cache.exists(2));
  EXPECT_TRUE(cache.exists(3));
  EXPECT_EQ(cache.get(1), 10);
  EXPECT_EQ(cache.get(2), -1);
  EXPECT_EQ(cache.get(3), 30);
}

TEST(LRUCacheTest, UpdatingExistingKeyRefreshesKeySoNextPutEvictsOlderKey) {
  LRUCache cache(2);

  cache.put(1, 10);
  cache.put(2, 20);
  cache.put(1, 100);
  cache.put(3, 30);

  EXPECT_TRUE(cache.exists(1));
  EXPECT_FALSE(cache.exists(2));
  EXPECT_TRUE(cache.exists(3));
  EXPECT_EQ(cache.get(1), 100);
  EXPECT_EQ(cache.get(2), -1);
  EXPECT_EQ(cache.get(3), 30);
}

TEST(LRUCacheTest, MissingGetDoesNotAffectEvictionOrder) {
  LRUCache cache(2);

  cache.put(1, 10);
  cache.put(2, 20);
  EXPECT_EQ(cache.get(99), -1);
  cache.put(3, 30);

  EXPECT_FALSE(cache.exists(1));
  EXPECT_TRUE(cache.exists(2));
  EXPECT_TRUE(cache.exists(3));
}

TEST(LRUCacheTest, SupportsNegativeKeysAndValues) {
  LRUCache cache(2);

  cache.put(-1, -10);
  cache.put(-2, -20);

  EXPECT_TRUE(cache.exists(-1));
  EXPECT_TRUE(cache.exists(-2));
  EXPECT_EQ(cache.get(-1), -10);
  EXPECT_EQ(cache.get(-2), -20);
}

TEST(LRUCacheTest, CapacityOneKeepsOnlyMostRecentKey) {
  LRUCache cache(1);

  cache.put(1, 10);
  cache.put(2, 20);

  EXPECT_EQ(cache.size(), 1);
  EXPECT_FALSE(cache.exists(1));
  EXPECT_TRUE(cache.exists(2));
  EXPECT_EQ(cache.get(1), -1);
  EXPECT_EQ(cache.get(2), 20);
}

TEST(LRUCacheTest, ZeroCapacityCacheDoesNotStoreValues) {
  LRUCache cache(0);

  cache.put(1, 10);
  cache.put(2, 20);

  EXPECT_EQ(cache.size(), 0);
  EXPECT_FALSE(cache.exists(1));
  EXPECT_FALSE(cache.exists(2));
  EXPECT_EQ(cache.get(1), -1);
  EXPECT_EQ(cache.get(2), -1);
}
