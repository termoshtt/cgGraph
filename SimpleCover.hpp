#pragma once

#include "Cover.hpp"

namespace cgGraph {

/*!
 * @class SimpleCover Cover.hpp "cgGraph/SimpleCover.hpp"
 * 線形に探査を行う、単純な被覆クラス
 *
 * SimpleCover::dist をオーバーライドする事で
 * 任意の比較関数を使用できる
 *
 * Tested
 * ------
 * test/simple_cover.cpp
 *  - 一直線に進む場合
 *  - @f$ \mathbb{R}^2 @f$ で半径1の円上を周る場合
 *  - save -> load_Omega で値が不変
 */
class SimpleCover : public CoverI {
  const unsigned int N;
  const double r;
  std::vector<State> Omega;

public:
  SimpleCover(unsigned int N /** 状態空間の次元 */, double r);
  virtual ~SimpleCover() {}

  uint64_t get_nearest(const State &x);
  const State &get_node(uint64_t i) const;
  uint64_t size() const;
  void save(std::string filename) const;
  double distance(uint64_t, uint64_t) const;
  unsigned int dim() const;
  double get_r() const;

  const std::vector<State> &get_Omega() const;

protected:
  /** 2つの状態間の距離、あるいは類似度を計算する */
  virtual double dist(const State &, const State &) const;
};

} // namespace cgGraph
