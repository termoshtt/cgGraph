#pragma once

#include <vector>
#include <map>

namespace cgGraph {

std::map<uint64_t, std::vector<uint64_t> >
num_accordance(const std::vector<uint64_t> &v, uint64_t N);

} // namespace cgGraph
