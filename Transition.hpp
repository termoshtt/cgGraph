#pragma once

#include <map>
#include <vector>
#include <iterator>
#include <algorithm>

namespace cgGraph {

typedef std::vector<double> Prob; ///< probability density @f$ p(x) @f$
typedef std::pair<uint64_t, uint64_t> Transit;

/** conditional probability @f$ p(y|x) @f$ */
class cProb : public std::map<Transit, double> {
public:
  /** return @f$p(y|x)@f$ */
  double operator()(uint64_t from /** x */, uint64_t to /** y */) const {
    return this->at(Transit(from, to));
  }
};

/*!
 * Count transitons from timeline
 *
 * - Last value `*(e-1)` is not counted.
 *
 * Test
 * ----
 *  - test/Transition.cpp
 */
template <typename Iter>
inline std::tuple<Prob, cProb> count_transition(Iter b, Iter e) {
  typedef typename std::iterator_traits<Iter>::value_type T;
  static_assert(std::is_integral<T>::value,
                "value_type must be unsigned integer");
  const double invN = 1.0 / (e - b - 1);
  Prob p;
  cProb cp;
  if (b == e)
    return std::make_tuple(p, cp);
  while (true) {
    uint64_t idx = *b++;
    if (b == e)
      break;
    uint64_t next = *b;
    if (idx >= p.size())
      p.resize(idx + 1, 0);
    p[idx] += 1;
    cp[Transit(idx, next)] += 1;
  }
  for (auto &pair : cp) {
    uint64_t from, to;
    std::tie(from, to) = pair.first;
    pair.second /= p[from];
  }
  std::transform(p.begin(), p.end(), p.begin(),
                 [=](double c) { return invN * c; });
  return std::make_tuple(p, cp);
}

} // namespace cgGraph
