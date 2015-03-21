#include "Cover.hpp"
#include "Timeline.hpp"
#include "TransitionMap.hpp"
#include "cgGraph.pb.h"

#include <iomanip>
#include <fstream>

namespace cgGraph {

std::vector<uint64_t> emergence_time(const Timeline &tl) {
  uint64_t idx0 = std::get<0>(tl.get(0));
  std::vector<uint64_t> e(idx0 + 1, 0);
  e[idx0] = 0;
  uint64_t time = std::get<1>(tl.get(0));
  for (uint64_t n = 1; n < tl.size(); n++) {
    uint64_t idx = std::get<0>(tl.get(n));
    if (idx >= e.size()) {
      e.resize(idx + 1, 0);
    }
    e[idx] = time;
    time += std::get<1>(tl.get(n));
  }
  return std::move(e);
}

void count_transition(const Timeline &tl, TransitionMap &tmap) {
  auto itr = tl.begin();
  uint64_t from, to, count;
  for (std::tie(from, count) = *itr++; itr != tl.end(); itr++) {
    to = std::get<0>(*itr);
    tmap.push(from, to, count);
    /* for next */
    from = to;
    count = std::get<1>(*itr);
  }
}

TransitionMap count_transition(const Timeline &tl) {
  TransitionMap tmap;
  count_transition(tl, tmap);
  tmap.finalize();
  return std::move(tmap);
}

void output_dot(const TransitionMap &tmap, std::string prefix) {
  std::ofstream ofs(prefix + ".dot");

  ofs << "digraph TransitionMap {\n";
  auto transits = tmap.get_transits();
  for (auto &p : transits) {
    auto transit = std::get<0>(p);
    uint64_t from = std::get<0>(transit);
    uint64_t to = std::get<1>(transit);
    ofs << from << " -> " << to << ";\n";
  }
  ofs << "}" << std::endl;
}

void save_cgGraph(const TransitionMap &tmap, std::string filename,
                  const CoverI *cover = nullptr) {
  pb::TransitionMap pb_tmap;
  bool finalized = tmap.is_finalized();
  auto &state_list = tmap.get_states();
  for (size_t i = 0; i < state_list.size(); ++i) {
    auto *s = pb_tmap.add_state();
    auto &st = state_list[i];
    s->set_index(i);
    if (finalized) {
      s->set_duration(st.duration);
      s->set_count(st.count);
      s->set_prob(st.prob);
      s->set_entropy(st.entropy);
      s->set_rate(st.rate);
    }
    if (cover) {
      auto *coor = s->mutable_coordinate();
      for (double x : cover->get_node(i)) {
        coor->add_x(x);
      }
    }
  }
  auto &transit_map = tmap.get_transits();
  for (auto &pair : transit_map) {
    uint64_t from = std::get<0>(pair.first);
    uint64_t to = std::get<1>(pair.first);
    auto *t = pb_tmap.add_transit();
    auto &p = pair.second;
    t->set_from_index(from);
    t->set_to_index(to);
    if (finalized) {
      t->set_count(p.count);
      t->set_prob(p.prob);
    }
  }
  std::ofstream ofs(filename,
                    std::ios::out | std::ios::binary | std::ios::trunc);
  if (!ofs)
    throw std::runtime_error("Cannot open file: " + filename);
  pb_tmap.SerializeToOstream(&ofs);
}

} // namespace cgGraph
