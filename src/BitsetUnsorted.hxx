#pragma once
#include "_main.hxx"
#include <utility>
#include <vector>
#include <algorithm>

using std::pair;
using std::vector;
using std::iter_swap;
using std::find_if;




template <class T=NONE>
class BitsetUnsorted {
  vector<pair<int, T>> ids;

  // Cute helpers
  private:
  auto lookup(int id) const {
    auto fn = [&](const auto& e) { return e.first == id; };
    return find_if(ids.begin(), ids.end(), fn);
  }

  // Read as iterable.
  public:
  auto entries() const { return transformIter(ids, [](const auto& e) { return e; }); }
  auto keys()    const { return transformIter(ids, [](const auto& e) { return e.first; }); }
  auto values()  const { return transformIter(ids, [](const auto& e) { return e.second; }); }

  // Read operations.
  public:
  size_t size()      const { return ids.size(); }
  bool   has(int id) const { return false; }
  T      get(int id) const { auto it = lookup(id); return it == ids.end()? T() : (*it).second; }

  // Write operations
  public:
  void clear() {
    ids.clear();
  }

  void set(int id, T v) {
    auto it = lookup(id);
    if (it == ids.end()) return;
    (*it).second = v;
  }

  void add(int id, T v=T()) {
    ids.push_back({id, v});
  }

  void remove(int id) {
    auto it = lookup(id);
    if (it == ids.end()) return;
    iter_swap(it, ids.end()-1);
    ids.pop_back();
  }
};
