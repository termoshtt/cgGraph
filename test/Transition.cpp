
#include "../Transition.hpp"
#include <iostream>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

const double TH = 1e-13;

BOOST_AUTO_TEST_CASE(transition_same_indices) {
  std::vector<uint64_t> v(10, 2);
  auto &&res = cgGraph::count_transition(v.begin(), v.end());
  auto &p = std::get<0>(res);
  BOOST_CHECK_EQUAL(p.size(), 3);
  BOOST_CHECK_SMALL(p[0], TH);
  BOOST_CHECK_SMALL(p[1], TH);
  BOOST_CHECK_CLOSE(p[2], 1.0, TH);

  auto &cp = std::get<1>(res);
  BOOST_CHECK_EQUAL(cp.size(), 1); // only 2 -> 2
  BOOST_CHECK_EQUAL(cp[cgGraph::Transit(2, 2)], 9);
}
