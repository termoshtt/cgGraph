
#include "Entropy.hpp"
#include <algorithm>

namespace cgGraph {

double xlogx(double x) { return (x > 0 ? x * log(x) : 0.0); }

double conditional_entropy(const Prob &p, const cProb &cp) {
  std::vector<double> c_en(p.size());
  for (auto &&pair : cp) {
    uint64_t from, to;
    std::tie(from, to) = std::get<0>(pair);
    double tprob = std::get<1>(pair);
    c_en[from] += p[from] * xlogx(tprob);
  }
  return std::accumulate(c_en.begin(), c_en.end(), 0);
}

double conditional_entropy(std::tuple<Prob, cProb> t) {
  const Prob &p = std::get<0>(t);
  const cProb &cp = std::get<1>(t);
  return conditional_entropy(p, cp);
}

} // namespace cgGraph
