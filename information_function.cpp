
#include "information_function.hpp"
#include <algorithm>
#include <fstream>
#include <msgpack.hpp>

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

void save(const NumAccordance &acc, std::string filename) {
  std::ofstream ofs(filename,
                    std::ios::out | std::ios::binary | std::ios::trunc);
  if (!ofs)
    throw std::runtime_error("Cannot open file: " + filename);
  msgpack::pack(ofs, acc);
}

} // namespace cgGraph
