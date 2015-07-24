
#include "../information_function.hpp"

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <iostream>

BOOST_AUTO_TEST_CASE(num_head_same_elements) {
  using cgGraph::impl::num_head_same_elements;

  std::vector<uint64_t> a = { 1, 2, 3, 4 };

  std::vector<uint64_t> b = { 1, 2, 3, 4, 5, 6 };
  BOOST_CHECK_EQUAL(num_head_same_elements(a.begin(), a.end(), b.begin()), 4);

  std::vector<uint64_t> c = { 1, 3 };
  BOOST_CHECK_EQUAL(num_head_same_elements(a.begin(), a.end(), c.begin()), 1);

  std::vector<uint64_t> d = { 2, 3 };
  BOOST_CHECK_EQUAL(num_head_same_elements(a.begin(), a.end(), d.begin()), 0);
}
