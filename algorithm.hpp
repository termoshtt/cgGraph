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

/** Timeline から TransitionMap を構成する */
TransitionMap count_transition(const Timeline &tl);

/** TransitionMapをDOT言語に出力する */
void output_dot(const TransitionMap &tmap, std::string prefix);

void save_cgGraph(const TransitionMap &tmap, std::string filename,
                  const CoverI *cover = nullptr);

} // namespace cgGraph
