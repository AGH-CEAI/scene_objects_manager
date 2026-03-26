#ifndef SCENE_OBJECTS_MANAGER__UTILS_HPP_
#define SCENE_OBJECTS_MANAGER__UTILS_HPP_
#include <map>

template <typename K, typename V>
static std::map<V, K> reverse_map(const std::map<K, V>& m) {
  std::map<V, K> r;
  for (const auto& kv : m)
    r[kv.second] = kv.first;
  return r;
}

#endif  // SCENE_OBJECTS_MANAGER__UTILS_HPP_
