#include <gtest/gtest.h>

#include "../src/lru_cache.h"

namespace {

void ExpectCacheHit(LRUCache &cache, int key, int expected_value) {
  auto value = cache.get(key);
  ASSERT_TRUE(value.has_value());
  EXPECT_EQ(value.value(), expected_value);
}

void ExpectCacheMiss(LRUCache &cache, int key) {
  EXPECT_FALSE(cache.get(key).has_value());
}

} // namespace

TEST(LRUCacheTest, NewCacheIsEmptyAndMissingKeyReturnsEmptyOptional) {
  LRUCache cache(3);

  EXPECT_EQ(cache.size(), 0);
  EXPECT_FALSE(cache.contains(1));
  ExpectCacheMiss(cache, 1);
  EXPECT_EQ(cache.size(), 0);
}

TEST(LRUCacheTest, PutStoresValueAndMarksKeyAsExisting) {
  LRUCache cache(2);

  cache.put(1, 10);

  EXPECT_TRUE(cache.contains(1));
  EXPECT_EQ(cache.size(), 1);
  ExpectCacheHit(cache, 1, 10);
}

TEST(LRUCacheTest, PutUpdatesExistingKeyWithoutIncreasingSize) {
  LRUCache cache(2);

  cache.put(1, 10);
  cache.put(1, 20);

  EXPECT_EQ(cache.size(), 1);
  EXPECT_TRUE(cache.contains(1));
  ExpectCacheHit(cache, 1, 20);
}

TEST(LRUCacheTest, PutEvictsLeastRecentlyInsertedKeyWhenCapacityIsReached) {
  LRUCache cache(2);

  cache.put(1, 10);
  cache.put(2, 20);
  cache.put(3, 30);

  EXPECT_EQ(cache.size(), 2);
  EXPECT_FALSE(cache.contains(1));
  EXPECT_TRUE(cache.contains(2));
  EXPECT_TRUE(cache.contains(3));
  ExpectCacheMiss(cache, 1);
  ExpectCacheHit(cache, 2, 20);
  ExpectCacheHit(cache, 3, 30);
}

TEST(LRUCacheTest, GetRefreshesKeySoNextPutEvictsOlderKey) {
  LRUCache cache(2);

  cache.put(1, 10);
  cache.put(2, 20);
  ExpectCacheHit(cache, 1, 10);
  cache.put(3, 30);

  EXPECT_TRUE(cache.contains(1));
  EXPECT_FALSE(cache.contains(2));
  EXPECT_TRUE(cache.contains(3));
  ExpectCacheHit(cache, 1, 10);
  ExpectCacheMiss(cache, 2);
  ExpectCacheHit(cache, 3, 30);
}

TEST(LRUCacheTest, UpdatingExistingKeyRefreshesKeySoNextPutEvictsOlderKey) {
  LRUCache cache(2);

  cache.put(1, 10);
  cache.put(2, 20);
  cache.put(1, 100);
  cache.put(3, 30);

  EXPECT_TRUE(cache.contains(1));
  EXPECT_FALSE(cache.contains(2));
  EXPECT_TRUE(cache.contains(3));
  ExpectCacheHit(cache, 1, 100);
  ExpectCacheMiss(cache, 2);
  ExpectCacheHit(cache, 3, 30);
}

TEST(LRUCacheTest, MissingGetDoesNotAffectEvictionOrder) {
  LRUCache cache(2);

  cache.put(1, 10);
  cache.put(2, 20);
  ExpectCacheMiss(cache, 99);
  cache.put(3, 30);

  EXPECT_FALSE(cache.contains(1));
  EXPECT_TRUE(cache.contains(2));
  EXPECT_TRUE(cache.contains(3));
}

TEST(LRUCacheTest, SupportsNegativeKeysAndValues) {
  LRUCache cache(2);

  cache.put(-1, -10);
  cache.put(-2, -20);

  EXPECT_TRUE(cache.contains(-1));
  EXPECT_TRUE(cache.contains(-2));
  ExpectCacheHit(cache, -1, -10);
  ExpectCacheHit(cache, -2, -20);
}

TEST(LRUCacheTest, CapacityOneKeepsOnlyMostRecentKey) {
  LRUCache cache(1);

  cache.put(1, 10);
  cache.put(2, 20);

  EXPECT_EQ(cache.size(), 1);
  EXPECT_FALSE(cache.contains(1));
  EXPECT_TRUE(cache.contains(2));
  ExpectCacheMiss(cache, 1);
  ExpectCacheHit(cache, 2, 20);
}

TEST(LRUCacheTest, ZeroCapacityCacheDoesNotStoreValues) {
  LRUCache cache(0);

  cache.put(1, 10);
  cache.put(2, 20);

  EXPECT_EQ(cache.size(), 0);
  EXPECT_FALSE(cache.contains(1));
  EXPECT_FALSE(cache.contains(2));
  ExpectCacheMiss(cache, 1);
  ExpectCacheMiss(cache, 2);
}
