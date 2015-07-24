
#include "information_function.hpp"
#include <algorithm>

namespace cgGraph {

typedef std::vector<uint64_t>::const_iterator Iterator;

uint64_t first_same_elements(Iterator b1, Iterator e1, Iterator b2) {
  uint64_t count = 0;
  while (b1 != e1) {
    if (*b1++ != *b2++)
      break;
    count++;
  }
  return count;
}

std::map<uint64_t, std::vector<uint64_t> >
num_accordance(const std::vector<uint64_t> &v, uint64_t N) {
  if (N == 0)
    N = v.size() / 10;

  std::map<uint64_t, Iterator> first_emerge;
  std::map<uint64_t, std::vector<uint64_t> > num_accord;

  for (auto it = v.begin(); it != v.end(); ++it) {
    auto idx = *it;
    if (first_emerge.find(idx) == first_emerge.end()) {
      first_emerge[idx] = it;
      continue;
    }
    auto fit = first_emerge[idx];
    auto n_m = first_same_elements(fit, fit + N, it);
    num_accord[idx].push_back(n_m);
  }

  for (auto p : num_accord) {
    auto nums = p.second;
    std::sort(nums.begin(), nums.end());
  }
  return num_accord;
}

} // namespace cgGraph
