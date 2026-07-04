#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include <cstddef>
#include <list>
#include <optional>
#include <unordered_map>

class LRUCache {
public:
  explicit LRUCache(std::size_t capacity) : _capacity(capacity) {}

  std::optional<int> get(int key) {
    auto it = _hash_map.find(key);
    if (it == _hash_map.end()) {
      return {};
    }

    // move node to the head of list
    _list.splice(_list.begin(), _list, it->second);
    return it->second->second;
  }

  void put(int key, int value) {
    if (_capacity == 0) {
      return;
    }

    auto it = _hash_map.find(key);

    if (it != _hash_map.end()) {
      it->second->second = value;
      _list.splice(_list.begin(), _list, it->second);
      return;
    }

    if (_list.size() >= _capacity) {
      int delete_key = _list.back().first;
      _list.pop_back();
      _hash_map.erase(delete_key);
    }

    _list.push_front({key, value});

    _hash_map[key] = _list.begin();
  }

  bool contains(int key) const {
    return _hash_map.find(key) != _hash_map.end();
  }

  std::size_t size() const { return _list.size(); }

private:
  std::size_t _capacity;
  std::list<std::pair<int, int>> _list;
  std::unordered_map<int, std::list<std::pair<int, int>>::iterator> _hash_map;
};

#endif // !LRU_CACHE_H
