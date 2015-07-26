
#include "information_function.hpp"
#include <algorithm>
#include <fstream>

namespace cgGraph {
namespace impl {

uint64_t num_head_same_elements(cIntIterator b1, cIntIterator e1,
                                cIntIterator b2) {
  uint64_t count = 0;
  while (b1 != e1) {
    if (*b1++ != *b2++)
      break;
    count++;
  }
  return count;
}

std::map<uint64_t, double> calc_info_func(uint64_t T, cIntIterator b,
                                          cIntIterator e) {
  uint64_t pre = 0; // n=0はありえないので
  uint64_t N = e - b;
  std::map<uint64_t, double> mu;
  double invT = 1.0 / T;
  for (; b != e; N--) {
    uint64_t now = *b++;
    if (now == pre)
      continue;
    mu[now] = -log(N * invT) / now;
    pre = now;
  }
  return mu;
}

} // namespace impl

std::map<uint64_t, std::map<uint64_t, double> >
information_function(const std::vector<uint64_t> &v) {
  std::map<uint64_t, impl::cIntIterator> first_emerge;
  std::map<uint64_t, std::vector<uint64_t> > num_accord;
  for (auto it = v.begin(); it != v.end(); ++it) {
    auto idx = *it;
    if (first_emerge.find(idx) == first_emerge.end()) {
      first_emerge[idx] = it;
      continue;
    }
    auto fit = first_emerge[idx];
    auto n_m = impl::num_head_same_elements(it, v.end(), fit);
    num_accord[idx].push_back(n_m);
  }
  std::map<uint64_t, std::map<uint64_t, double> > info_func;
  for (auto &p : num_accord) {
    uint64_t idx = p.first;
    auto &ns = p.second;
    std::sort(ns.begin(), ns.end());
    info_func[idx] = impl::calc_info_func(v.size(), ns.begin(), ns.end());
  }
  return info_func;
}

} // namespace cgGraph
