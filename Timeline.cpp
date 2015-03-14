
#include "Timeline.hpp"
#include "exception.hpp"
#include "cgGraph.pb.h"

#include <fstream>
#include <iomanip>

namespace cgGraph {

template <typename Int>
Timeline<Int>::Timeline()
    : is_finalized(false), is_first(true) {}

template <typename Int>
Timeline<Int>::Timeline(std::string filename)
    : is_finalized(false), is_first(false) /* 読み出した最後から続ける */
{
  load(filename);
}

template <typename Int> void Timeline<Int>::push(Int i) {
  check_finalize();
  if (is_first) {
    is_first = false;
    last_index = i;
    count = 1;
    return;
  }
  if (i == last_index) {
    count++;
  } else {
    tl.emplace_back(last_index, count);
    last_index = i;
    count = 1;
  }
}

template <typename Int> void Timeline<Int>::finalize() {
  if (!is_finalized) {
    tl.emplace_back(last_index, count);
    is_finalized = true;
  }
}

template <typename Int>
typename Timeline<Int>::Iterator Timeline<Int>::begin() const {
  return tl.begin();
}
template <typename Int>
typename Timeline<Int>::Iterator Timeline<Int>::end() const {
  return tl.end();
}
template <typename Int>
const typename Timeline<Int>::Pair &Timeline<Int>::get(Int i) const {
  return tl[i];
}

template <typename Int> Int Timeline<Int>::size() const { return tl.size(); }

template <typename Int> void Timeline<Int>::check_finalize() const {
  if (is_finalized) {
    throw cgGraphError("This instance has been finialized");
  }
}

template <typename Int> void Timeline<Int>::load(std::string filename) {
  std::ifstream ifs(filename, std::ios::in | std::ios::binary);
  if (!ifs)
    throw std::runtime_error("Cannot open file: " + filename);
  pb::TimeLine pbtl;
  pbtl.ParseFromIstream(&ifs);
  for (int t = 0; t < pbtl.chunk_size(); ++t) {
    const pb::Chunk &c = pbtl.chunk(t);
    tl.emplace_back(c.index(), c.count());
  }
}

template <typename Int> void Timeline<Int>::save(std::string filename) {
  finalize();
  std::ofstream ofs(filename,
                    std::ios::out | std::ios::binary | std::ios::trunc);
  if (!ofs)
    throw std::runtime_error("Cannot open file: " + filename);
  pb::TimeLine pbtl;
  for (Pair &p : tl) {
    pb::Chunk *c = pbtl.add_chunk();
    c->set_index(std::get<0>(p));
    c->set_count(std::get<1>(p));
  }
  pbtl.SerializeToOstream(&ofs);
}

template class Timeline<unsigned int>;
template class Timeline<unsigned long>;
template class Timeline<unsigned long long>;

} // namespace cgGraph
