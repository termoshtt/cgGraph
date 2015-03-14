#pragma once

#include <map>
#include <vector>
#include <tuple>

namespace cgGraph {

/*!
 * \class TransitionMap
 * \headerfile TransitionMap.hpp "cgGraph/TransitionMap.hpp"
 * 有限状態空間における遷移図を生成・管理する
 *
 * 遷移は限られた状態間でしか生じないと思われるので密行列で保存せず、
 * Map (from, to) -> count で管理する。
 *
 * Tested
 * ------
 *  test/transition_map.cpp
 *  - pushで数個追加した際の以下を確認
 *    - StateP.{duration,count,prob,rate,entropy}
 *    - TransitP.{count,prob}
 */
template <typename Int = unsigned long> class TransitionMap {
public:
  TransitionMap();

  /** 状態のプロパティ
   * \headerfile TransitionMap.hpp "DS/cgGraph/TransitionMap.hpp" */
  struct StateP {
    Int duration;   ///< 滞在時間の合計
    Int count;      ///< 滞在した回数
    double prob;    ///< 状態の不変確率測度
    double entropy; ///< 遷移の不確実性
    ///  @f$ H_i = \sum_{j \neq i} w(i \to j) \log_2 w(i \to j) @f$
    double rate; ///< 遷移率 @f$ w_i = \sum_{j \neq i} w(i \to j) @f$
                 ///  (Poisson過程と仮定する)
  };
  typedef std::vector<StateP> StateList;

  /** 遷移のプロパティ
   * \headerfile TransitionMap.hpp "DS/cgGraph/TransitionMap.hpp" */
  struct TransitP {
    Int count;
    double prob; ///< 遷移確率
  };
  typedef std::tuple<Int, Int> Transit;
  typedef std::map<Transit, TransitP> TransitMap;
  typedef typename TransitMap::iterator TransitMapIterator;

  /** 遷移 (from, to) を登録する */
  void push(Int from, Int to, Int stay_time /** fromに滞在した時間 */);

  /** 保持したカウントから各状態の存在確率とその間の遷移確率を計算する */
  void finalize();
  bool is_finalized() const;

  /** 登録された遷移の総数 */
  Int transition_count() const;
  /** 実現した (from, to) の組の個数 */
  Int combination_count() const;
  /** 経過時間 */
  Int time() const;

  /** 状態の詳細を取得する */
  const StateList &get_states() const;
  /** 個々の状態の詳細を取得する */
  const StateP &get_state(Int) const;
  /** 遷移の詳細を取得する */
  const TransitMap &get_transits() const;
  /** 個々の遷移の詳細を取得する */
  const TransitP &get_transit(Int from, Int to) const;

private:
  bool finalized_flag;
  StateList state_list;
  TransitMap transit_map;
};

} // namespace cgGraph
