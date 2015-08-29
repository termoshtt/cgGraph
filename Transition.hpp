
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
  double operator()(uint64_t from, uint64_t to) const {
    return this->at(Transit(from, to));
  }
};

/*!
 * Count transitons from timeline
 */
template <typename Iter>
inline std::tuple<Prob, cProb> count_transition(Iter b, Iter e) {
  typedef typename std::iterator_traits<Iter>::value_type T;
  static_assert(std::is_integral<T>::value,
                "value_type must be unsigned integer");
  const double invN = 1.0 / (e - b);

  uint64_t idx = *b++;
  uint64_t pre = idx;
  std::vector<uint64_t> count(idx + 1, 0);
  count[idx] += 1;
  std::map<Transit, uint64_t> tcount;
  while (b != e) {
    idx = *b++;
    if (idx > count.size())
      count.resize(idx + 1, 0);
    count[idx] += 1;
    Transit t(pre, idx);
    tcount[t] += 1;
    pre = idx;
  }

  Prob p(count.size());
  std::transform(count.begin(), count.end(), p.begin(),
                 [=](uint64_t c) { return invN * c; });

  cProb cp;
  uint64_t psum = 0;
  uint64_t now = tcount.begin()->first.first;
  std::vector<Transit> ts;
  for (auto &&pair : tcount) {
    const Transit &t = pair.first;
    uint64_t from = t.first;
    uint64_t c = pair.second;

    if (from != now) {
      double invSum = 1.0 / psum;
      for (auto &&t : ts) {
        cp[t] *= invSum;
      }
      ts.clear();
      now = from;
      psum = 0.0;
    }
    psum += c;
    cp[t] = c;
    ts.push_back(t);
  }
  double invSum = 1.0 / psum;
  for (auto &&t : ts) {
    cp[t] *= invSum;
  }
  return std::make_tuple(p, cp);
}

} // namespace cgGraph
