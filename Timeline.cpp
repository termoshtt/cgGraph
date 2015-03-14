
#include "Timeline.hpp"
#include "exception.hpp"
#include "cgGraph.pb.h"

#include <fstream>
#include <iomanip>

namespace cgGraph {

Timeline::Timeline() : is_finalized(false), is_first(true) {}

Timeline::Timeline(std::string filename)
    : is_finalized(false), is_first(false) /* 読み出した最後から続ける */
{
  load(filename);
}

void Timeline::push(uint64_t i) {
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

void Timeline::finalize() {
  if (!is_finalized) {
    tl.emplace_back(last_index, count);
    is_finalized = true;
  }
}

Timeline::Iterator Timeline::begin() const { return tl.begin(); }

Timeline::Iterator Timeline::end() const { return tl.end(); }

const Timeline::Pair &Timeline::get(uint64_t i) const { return tl[i]; }

uint64_t Timeline::size() const { return tl.size(); }

void Timeline::check_finalize() const {
  if (is_finalized) {
    throw cgGraphError("This instance has been finialized");
  }
}

void Timeline::load(std::string filename) {
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

void Timeline::save(std::string filename) {
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

} // namespace cgGraph
