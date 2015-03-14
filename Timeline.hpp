#pragma once

#include <vector>
#include <string>
#include <tuple>

namespace cgGraph {

/*!
 * \class Timeline Timeline.hpp "cgGraph/Timeline.hpp"
 * 抽象化した時系列を管理する(時系列を保持)
 *
 * - 時系列を全てメモリ上に保持する。
 * - 1,1,1,1,2,2,2,3,3,4,4,4,... のような系列は
 *   (1,4),(2,3),(3,2),(4,3),... のように圧縮して保持する。
 * - 最後の分、例えば 1,1,2,2,3 のように終る場合は最後の3は廃棄する。
 *   保存したい場合は `finalize()` を呼ぶ。
 *
 * Tested
 * ------
 *  test/timeline.cpp
 *  - pushした列がが上述の通り圧縮される
 *  - save/loadで変らない
 *  \todo load後追加できるかを確認する
 *
 */
class Timeline {
public:
  Timeline();
  /** `load(filename)`を使用して初期化する */
  explicit Timeline(std::string filename);

  /** 書き込む前に`finalize()`を実行する */
  void save(std::string filename);
  /** バイナリ形式を読みこむ */
  void load(std::string filename);

  /** (index, count)のペア */
  typedef std::tuple<uint64_t, uint64_t> Pair;
  /** 保存した時系列へのアクセスのためのイテレータクラス */
  typedef typename std::vector<Pair>::const_iterator Iterator;

  /** インデックスを追加する */
  void push(uint64_t i);

  /** 個々の保存した時系列のペア(index, count)へのアクセス */
  const Pair &get(uint64_t) const;
  /** 保存した時系列のペア(index, count)へのアクセス(begin) */
  Iterator begin() const;
  /** 保存した時系列のペア(index, count)へのアクセス(end) */
  Iterator end() const;

  /** 保存されているPairの数を返す */
  uint64_t size() const;
  /** 保存されている時系列の個数を返す
   *  \todo implement */
  uint64_t accumulate_size() const;

  /** 最後のペアを登録する
   *
   * 1,2,3,4,4,4 なら(4,3)を登録する
   */
  void finalize();

private:
  std::vector<Pair> tl;
  uint64_t last_index, count;
  bool is_finalized, is_first;
  void check_finalize() const;
};

/*!
 * \class DisposableTimeline
 * \headerfile Timeline.hpp "cgGraph/Timeline.hpp"
 * 抽象化した時系列を管理する(使い捨て)
 *
 * 時系列データをonlineで処理して、保存したくない場合は
 * Timeline の変りにこちらを使用する。
 *
 * \todo 詳細設計
 */
class DisposableTimeline {};

} // namespace cgGraph
