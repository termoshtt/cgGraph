#pragma once

#include <vector>
#include <string>

/** アトラクタ上のダイナミクスを表現する確率過程を数値的に導出する */
namespace cgGraph {

typedef std::vector<double> State; ///< 状態点

/*!
 * \interface CoverI Cover.hpp "cgGraph/Cover.hpp"
 * アトラクタ@f$ \mathcal{A} \subset \mathbb{R}^N @f$ (コンパクトと仮定) の被覆
 * @f$ B_r(\Omega) = \cup_{x \in \Omega} B_r(x) @f$
 * を構成、管理する
 *
 * - @f$ \mathcal{A}@f$のコンパクト性から、
 *   @f$ \Omega = \{ x_0, x_1, \dots, x_{M-1} \} @f$は有限個の点からなる
 * - @f$ B_r(x) = \{ x^\prime : \| x - x^\prime \| \leq r \} @f$
 *
 */
class CoverI {
public:
  /** 引数のxに一番近い @f$x_i \in \Omega@f$ のインデックスを返す。
   *  すべての @f$ i @f$ で @f$\| x - x_i \| > r@f$
   *  の時は新しく追加し、そのインデックスを返す */
  virtual uint64_t get_nearest(const State &x) = 0;
  /** @f$ x_i @f$ を返す */
  virtual const State &get_node(uint64_t i) const = 0;
  /** @f$ \Omega @f$ の個数を返す */
  virtual uint64_t size() const = 0;
  /** 次元Nを返す */
  virtual unsigned int dim() const = 0;
  /** 被覆の直径@f$ r @f$を取得する */
  virtual double get_r() const = 0;

  /** @f$ \Omega @f$ を保存する */
  virtual void save(std::string filename) const = 0;
  /** 登録してある2つの点の距離を返す */
  virtual double distance(uint64_t, uint64_t) const = 0;
};

/** CoverI::save で保存した @f$ \Omega @f$ を読み出す
 *
 * Tested
 * ------
 * test/simple_cover.cpp
 *  - SimpleCover::save -> load_Omega で値が不変
 */
std::vector<State> load_Omega(std::string filename);

} // namespace cgGraph
