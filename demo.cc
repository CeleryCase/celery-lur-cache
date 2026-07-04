#include <iostream>

#include "src/lru_cache.h"

int main(void) {
  LRUCache cache(3);

  std::cout << "cache initialized" << std::endl;
  cache.put(1, 11);
  cache.put(2, 22);
  cache.put(3, 33);

  std::cout << "put (1, 11), (2, 22), (3,33)" << std::endl;

  auto value = cache.get(1);
  std::cout << "get key: 1, value: " << value.value() << std::endl;

  cache.put(4, 44);

  std::cout << "put (4, 44)" << std::endl;
  std::cout << "exists key: 2 is " << cache.contains(2) << std::endl;

  return 0;
}
