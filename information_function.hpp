#pragma once

#include <vector>
#include <map>

namespace cgGraph {

std::map<uint64_t, std::map<uint64_t, double> >
information_function(const std::vector<uint64_t> &v);

namespace impl {

typedef std::vector<uint64_t>::const_iterator cIntIterator;
uint64_t num_head_same_elements(cIntIterator b1, cIntIterator e1,
                                cIntIterator b2);
std::map<uint64_t, double> calc_info_func(uint64_t T, cIntIterator b,
                                          cIntIterator e);

} // namespace impl
} // namespace cgGraph
