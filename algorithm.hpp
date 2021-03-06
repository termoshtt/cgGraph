#pragma once

#include "Cover.hpp"
#include "Timeline.hpp"
#include "TransitionMap.hpp"
#include "cgGraph.pb.h"

#include <iomanip>
#include <fstream>

namespace cgGraph {

/** 各ノードが最初に出現した時刻を返す
 *
 * @return 各インデックスの出現時刻
 */
std::vector<uint64_t> emergence_time(const Timeline &tl);

/** construct a transition map from a timeline. */
TransitionMap count_transition(const Timeline &tl);

/** count transitions in a timeline and append to an existing transition map. */
void count_transition(const Timeline &tl, TransitionMap &);

/** TransitionMapをDOT言語に出力する */
void output_dot(const TransitionMap &tmap, std::string prefix);

/** save coarse-grained graph */
void save_graph(const TransitionMap &tmap, std::string filename,
                const CoverI *cover = nullptr);

} // namespace cgGraph
