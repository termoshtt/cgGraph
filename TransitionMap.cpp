
#include "TransitionMap.hpp"
#include "cgGraph.pb.h"

#include <algorithm>
#include <fstream>

namespace cgGraph {

template <typename Int>
TransitionMap<Int>::TransitionMap()
    : finalized_flag(false) {}

template <typename Int> bool TransitionMap<Int>::is_finalized() const {
  return finalized_flag;
}

template <typename Int>
void TransitionMap<Int>::push(Int from, Int to, Int stay_time) {
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

template <typename Int> void TransitionMap<Int>::finalize() {
  /* transit : (0,1) (0,2) (1,0) (1,2) (2,3) のように整列している事を使う */
  auto itr = transit_map.begin();
  auto end = transit_map.end();
  Int now = 0;
  std::vector<TransitMapIterator> neighbors;
  while (itr != end || !neighbors.empty()) {
    if (itr != end) {
      Int from = std::get<0>(itr->first);
      if (from == now) {
        neighbors.push_back(itr++);
        continue;
      }
    }
    // fromが切り替わった or 最後のfrom(切り替わる先がない)
    const Int count =
        std::accumulate(neighbors.begin(), neighbors.end(), 0,
                        [](Int sum, const TransitMapIterator &itr) {
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
  const Int T = time();
  std::transform(state_list.begin(), state_list.end(), state_list.begin(),
                 [=](StateP v) {
    v.prob = (double)v.duration / T;
    v.rate = (double)v.count / v.duration;
    return v;
  });
  finalized_flag = true;
}

template <typename Int> Int TransitionMap<Int>::transition_count() const {
  return std::accumulate(transit_map.begin(), transit_map.end(), 0,
                         [](Int sum, const std::pair<Transit, TransitP> &p) {
    return sum + p.second.count;
  });
}

template <typename Int> Int TransitionMap<Int>::combination_count() const {
  return transit_map.size();
}

template <typename Int> Int TransitionMap<Int>::time() const {
  return std::accumulate(
      state_list.begin(), state_list.end(), 0,
      [](Int sum, const StateP &v) { return sum + v.duration; });
}

template <typename Int>
const typename TransitionMap<Int>::StateList &
TransitionMap<Int>::get_states() const {
  return state_list;
}

template <typename Int>
const typename TransitionMap<Int>::StateP &
TransitionMap<Int>::get_state(Int i) const {
  return state_list.at(i);
}

template <typename Int>
const typename TransitionMap<Int>::TransitMap &
TransitionMap<Int>::get_transits() const {
  return transit_map;
}

template <typename Int>
const typename TransitionMap<Int>::TransitP &
TransitionMap<Int>::get_transit(Int from, Int to) const {
  return transit_map.at(std::make_tuple(from, to));
}

template class TransitionMap<unsigned int>;
template class TransitionMap<unsigned long>;
template class TransitionMap<unsigned long long>;

} // namespace cgGraph
