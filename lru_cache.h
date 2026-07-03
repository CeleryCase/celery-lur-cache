#ifndef LRU_CACHE_H

#include <cstdint>
#include <list>
#include <unordered_map>

class LRUCache {
public:
  LRUCache(uint32_t capacity) : _capacity(capacity) {}

  int get(int key) {
    auto it = _hash_map.find(key);
    if (it == _hash_map.end()) {
      return -1;
    }

    auto kv = *it->second;

    _list.erase(it->second);

    _list.push_front(kv);

    // update the value of key in _hash_map
    _hash_map[key] = _list.begin();

    return kv.second;
  }

  void put(int key, int value) {
    auto it = _hash_map.find(key);

    if (it != _hash_map.end()) {
      _list.erase(it->second);
    }

    if (_list.size() >= _capacity) {
      int delete_key = _list.back().first;
      _list.pop_back();
      _hash_map.erase(delete_key);
    }

    _list.push_front({key, value});

    // update the value of key in _hash_map
    _hash_map[key] = _list.begin();
  }

  bool exists(int key) { return _hash_map.find(key) != _hash_map.end(); }

  uint32_t size() { return _list.size(); }

private:
  uint32_t _capacity;
  std::list<std::pair<int, int>> _list;
  std::unordered_map<int, std::list<std::pair<int, int>>::iterator> _hash_map;
};

#endif // !LRU_CACHE_H
