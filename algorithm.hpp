#pragma once

#include "Cover.hpp"
#include "Timeline.hpp"
#include "TransitionMap.hpp"
#include "cgGraph.pb.h"

#include "EOM/interface.hpp"
#include "utility/log.hpp"

#include <iomanip>
#include <fstream>
#include <climits> // ULONG_MAX

namespace cgGraph {

/** 力学系を確率化する
 *
 * 時系列を生成し被覆を形成しインデックスの時系列を生成する。
 * 終了時刻を与える。
 */
template <typename Int = unsigned long, typename Real = double>
void run_finite_time(EOM::TimeEvolutionFunctorI<Real> &F /** 時間発展演算子 */,
                     Real *x0 /** 初期値 */,
                     unsigned long T /** 終了時刻(イテレーション回数) */,
                     CoverI<Int, Real> &c /** 被覆を構成・保存する */,
                     Timeline<Int> &tl /** 時系列を管理する */) {
  for (unsigned long t = 0; t < T; ++t) {
    Int idx = c.get_nearest(x0);
    tl.push(idx);
    F(x0);
  }
  tl.finalize();
}

/** 力学系を確率化する
 *
 * 時系列を生成し被覆を形成しインデックスの時系列を生成する。
 * duration だけ $\Omega$ の要素数が変化しなくなるまで継続する。
 */
template <typename Int = unsigned long, typename Real = double>
void
run_until_converge(EOM::TimeEvolutionFunctorI<Real> &F /** 時間発展演算子 */,
                   State<Real> &x0 /** 初期値 */,
                   unsigned long duration /** 収束判定の継続時間 */,
                   CoverI<Int, Real> &c /** 被覆を構成・保存する */,
                   Timeline<Int> &tl /** 時系列を管理する */) {
  unsigned long last = c.size();
  for (unsigned long t = 0; t < ULONG_MAX; ++t) {
    Int idx = c.get_nearest(x0);
    tl.push(idx);
    F(&x0[0]);
    if (t % duration == 0) {
      utility::log("#Omega", c.size(), std::clog);
      if (c.size() == last) {
        break;
      } else {
        last = c.size();
      }
    }
  }
  tl.finalize();
}

/** 各ノードが最初に出現した時刻を返す
 *
 * @return 各インデックスの出現時刻
 */
template <typename Int = unsigned long, typename LongInt = unsigned long long>
std::vector<LongInt> emergence_time(const Timeline<Int> &tl) {
  Int idx0 = std::get<0>(tl.get(0));
  std::vector<LongInt> e(idx0 + 1, 0);
  e[idx0] = 0;
  LongInt time = std::get<1>(tl.get(0));
  for (Int n = 1; n < tl.size(); n++) {
    Int idx = std::get<0>(tl.get(n));
    if (idx >= e.size()) {
      e.resize(idx + 1, 0);
    }
    e[idx] = time;
    time += std::get<1>(tl.get(n));
  }
  return std::move(e);
}

/** Timeline から TransitionMap を構成する */
template <typename Int = unsigned long>
TransitionMap<Int> count_transition(const Timeline<Int> &tl) {
  TransitionMap<Int> tmap;
  auto itr = tl.begin();
  Int from, to, count;
  for (std::tie(from, count) = *itr++; itr != tl.end(); itr++) {
    to = std::get<0>(*itr);
    tmap.push(from, to, count);
    /* for next */
    from = to;
    count = std::get<1>(*itr);
  }
  tmap.finalize();
  return std::move(tmap);
}

/** TransitionMapをDOT言語に出力する */
template <typename Int = unsigned long>
void output_dot(const TransitionMap<Int> &tmap, std::string prefix) {
  std::ofstream ofs(prefix + ".dot");

  ofs << "digraph TransitionMap {\n";
  auto transits = tmap.get_transits();
  for (auto &p : transits) {
    auto transit = std::get<0>(p);
    Int from = std::get<0>(transit);
    Int to = std::get<1>(transit);
    ofs << from << " -> " << to << ";\n";
  }
  ofs << "}" << std::endl;
}

template <typename Int, typename Real>
void save_cgGraph(const TransitionMap<Int> &tmap, std::string filename,
                  const CoverI<Int, Real> *cover = nullptr) {
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
      for (Real x : cover->get_node(i)) {
        coor->add_x(x);
      }
    }
  }
  auto &transit_map = tmap.get_transits();
  for (auto &pair : transit_map) {
    Int from = std::get<0>(pair.first);
    Int to = std::get<1>(pair.first);
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
