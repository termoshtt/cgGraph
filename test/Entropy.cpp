
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
  auto &&res = cgGraph::count_transition(v.begin(), v.end());
  BOOST_CHECK_CLOSE(cgGraph::conditional_entropy(res), 0.0, TH);
}

BOOST_AUTO_TEST_CASE(entropy_simple) {
  std::vector<uint64_t> v = { 1, 2, 1, 2, 1, 3 };
  cgGraph::Prob p;
  cgGraph::cProb cp;
  std::tie(p, cp) = cgGraph::count_transition(v.begin(), v.end());
  BOOST_CHECK_CLOSE(p[1], 3.0 / 5.0, TH);
  double en1 = -2. / 3. * log(2. / 3.) - 1. / 3. * log(1. / 3.);
  BOOST_CHECK_CLOSE(cgGraph::conditional_entropy(p, cp), p[1] * en1, TH);
}
