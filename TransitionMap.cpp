
#include "TransitionMap.hpp"
#include "cgGraph.pb.h"

#include <algorithm>
#include <fstream>

namespace cgGraph {

TransitionMap::TransitionMap() : finalized_flag(false) {}

bool TransitionMap::is_finalized() const { return finalized_flag; }

void TransitionMap::push(uint64_t from, uint64_t to, uint64_t stay_time) {
  /* transit */
  auto itr = transit_map.find(std::make_tuple(from, to));
  if (itr == transit_map.end()) {
    transit_map[std::make_tuple(from, to)] = { 1, 0.0 };
  } else {
    (itr->second).count += 1;
  }
  /* state */
  if (from >= state_list.size()) {
    state_list.resize(from + 1, { 0 });
  }
  state_list[from].duration += stay_time;
  state_list[from].count++;

  finalized_flag = false;
}

void TransitionMap::finalize() {
  /* transit : (0,1) (0,2) (1,0) (1,2) (2,3) のように整列している事を使う */
  auto itr = transit_map.begin();
  auto end = transit_map.end();
  uint64_t now = 0;
  std::vector<TransitMapIterator> neighbors;
  while (itr != end || !neighbors.empty()) {
    if (itr != end) {
      uint64_t from = std::get<0>(itr->first);
      if (from == now) {
        neighbors.push_back(itr++);
        continue;
      }
    }
    // fromが切り替わった or 最後のfrom(切り替わる先がない)
    const uint64_t count =
        std::accumulate(neighbors.begin(), neighbors.end(), 0,
                        [](uint64_t sum, const TransitMapIterator &itr) {
          return sum + itr->second.count;
        }); // fromを通った全回数
    if (count > 0) {
      for (TransitMapIterator &itr : neighbors) {
        TransitP &e = itr->second;
        e.prob = (double)e.count / count;
        state_list[now].entropy -= e.prob * log2(e.prob);
      }
    }
    neighbors.clear();
    now++;
  }

  /* state */
  const uint64_t T = time();
  std::transform(state_list.begin(), state_list.end(), state_list.begin(),
                 [=](StateP v) {
    v.prob = (double)v.duration / T;
    v.rate = (double)v.count / v.duration;
    return v;
  });
  finalized_flag = true;
}

uint64_t TransitionMap::transition_count() const {
  return std::accumulate(
      transit_map.begin(), transit_map.end(), 0,
      [](uint64_t sum, const std::pair<Transit, TransitP> &p) {
        return sum + p.second.count;
      });
}

uint64_t TransitionMap::combination_count() const { return transit_map.size(); }

uint64_t TransitionMap::time() const {
  return std::accumulate(
      state_list.begin(), state_list.end(), 0,
      [](uint64_t sum, const StateP &v) { return sum + v.duration; });
}

const TransitionMap::StateList &TransitionMap::get_states() const {
  return state_list;
}

const TransitionMap::StateP &TransitionMap::get_state(uint64_t i) const {
  return state_list.at(i);
}

const TransitionMap::TransitMap &TransitionMap::get_transits() const {
  return transit_map;
}

const TransitionMap::TransitP &TransitionMap::get_transit(uint64_t from,
                                                          uint64_t to) const {
  return transit_map.at(std::make_tuple(from, to));
}

} // namespace cgGraph
