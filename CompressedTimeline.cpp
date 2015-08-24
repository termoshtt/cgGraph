
#include <vector>
#include <cstdint>
#include <algorithm>

#include <iostream>

namespace cgGraph {

class CompressedTimeline {
  std::vector<std::pair<uint64_t, uint64_t> > tl;

public:
  class iterator {
    CompressedTimeline *p;
    uint64_t index, count;

  public:
    typedef uint64_t value_type;

    iterator(CompressedTimeline *p_) : p(p_), index(0), count(1) {}

    bool operator==(const iterator &x) const {
      if (p != x.p)
        return false;
      if (p == nullptr)
        return true;
      if (index == x.index && count == x.count)
        return true;
      return false;
    }
    bool operator!=(const iterator &x) const { return !(*this == x); }
    void operator++() {
      if (!p)
        return;
      if (count < p->tl[index].second) {
        count++;
        return;
      }
      index++;
      count = 1;
      if (index >= p->tl.size())
        p = nullptr;
    }
    uint64_t &operator*() const { return p->tl[index].first; }
  };
  iterator begin() { return iterator(this); }
  iterator end() { return iterator(nullptr); }

  void push_back(uint64_t value) {
    if (tl.empty()) {
      tl.emplace_back(value, 1);
      return;
    }
    auto &last = *(tl.end() - 1);
    if (value == last.first) {
      last.second++;
    } else {
      tl.emplace_back(value, 1);
    }
  }

  uint64_t size() const {
    uint64_t sum = 0;
    for (auto &p : tl)
      sum += p.second;
    return sum;
  }

  uint64_t chunk_size() const { return tl.size(); }
};

} // namespace cgGraph

int main(int argc, char const *argv[]) {
  cgGraph::CompressedTimeline tl;
  tl.push_back(1);
  tl.push_back(1);
  tl.push_back(2);
  tl.push_back(2);
  tl.push_back(1);

  for (auto &&v : tl) {
    std::cout << v << std::endl;
  }

  std::cout << tl.chunk_size() << std::endl;
  std::cout << tl.size() << std::endl;

  return 0;
}
