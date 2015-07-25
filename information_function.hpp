#pragma once

#include <vector>
#include <map>

namespace cgGraph {

class NumAccordance : public std::map<uint64_t, std::vector<uint64_t> > {
public:
  void save(std::string filename);
};

NumAccordance num_accordance(const std::vector<uint64_t> &v);

namespace impl {

typedef std::vector<uint64_t>::const_iterator cIntIterator;
uint64_t num_head_same_elements(cIntIterator b1, cIntIterator e1,
                                cIntIterator b2);

} // namespace impl
} // namespace cgGraph
