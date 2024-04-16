#pragma once
#include <map>

template <typename K, typename V>
static std::map<V, K> reverse_map(const std::map<K, V>& m) {
  std::map<V, K> r;
  for (const auto& kv : m)
    r[kv.second] = kv.first;
  return r;
}
