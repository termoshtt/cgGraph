
#include "information_function.hpp"
#include "cgGraph.pb.h"
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

} // namespace impl

NumAccordance num_accordance(const std::vector<uint64_t> &v) {
  std::map<uint64_t, impl::cIntIterator> first_emerge;
  NumAccordance num_accord;
  for (auto it = v.begin(); it != v.end(); ++it) {
    auto idx = *it;
    if (first_emerge.find(idx) == first_emerge.end()) {
      first_emerge[idx] = it;
      continue;
    }
    auto fit = first_emerge[idx];
    auto n_m = impl::num_head_same_elements(fit, v.end(), it);
    num_accord[idx].push_back(n_m);
  }
  return num_accord;
}

void NumAccordance::save(std::string filename) {
  pb::NumAccordance pbacc;
  for (auto &p : *this) {
    auto pp = pbacc.add_pair();
    pp->set_index(p.first);
    for (auto n : p.second) {
      pp->add_n(n);
    }
  }
  std::ofstream ofs(filename,
                    std::ios::out | std::ios::binary | std::ios::trunc);
  if (!ofs)
    throw std::runtime_error("Cannot open file: " + filename);
  pbacc.SerializeToOstream(&ofs);
}

} // namespace cgGraph
