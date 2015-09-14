
#include "../Transition.hpp"
#include "../Entropy.hpp"
#include <iostream>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

const double TH = 1e-13;

BOOST_AUTO_TEST_CASE(entropy_zero) {
  std::vector<uint64_t> v(10);
  uint64_t count = 0;
  for (auto &i : v) {
    i = count++;
  }
  auto res = cgGraph::count_transition(v.begin(), v.end());
  // std::cout << cgGraph::conditional_entropy(res);
}
