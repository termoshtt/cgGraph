
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
  BOOST_CHECK_EQUAL(cp(2, 2), 1.0);
}

BOOST_AUTO_TEST_CASE(transition_sequence) {
  std::vector<uint64_t> v(10);
  uint64_t count = 0;
  for (auto &i : v) {
    i = count++;
  }
  auto &&res = cgGraph::count_transition(v.begin(), v.end());
  auto &p = std::get<0>(res);
  BOOST_CHECK_EQUAL(p.size(), 10);

  auto &cp = std::get<1>(res);
  BOOST_CHECK_EQUAL(cp.size(), 9);
  for (int i = 0; i < 9; ++i) {
    BOOST_CHECK_CLOSE(cp(i, i + 1), 1.0, TH);
  }
}

BOOST_AUTO_TEST_CASE(transition_simple) {
  std::vector<uint64_t> v(10, 2);
  for (int i = 0; i < 10; ++i) {
    v[i] = i * i;
  }
  auto &&res = cgGraph::count_transition(v.begin(), v.end());
  auto &p = std::get<0>(res);
  BOOST_CHECK_EQUAL(p.size(), 81 + 1);

  auto &cp = std::get<1>(res);
  BOOST_CHECK_EQUAL(cp.size(), 9);
  for (auto &p : cp) {
    double cprob = std::get<1>(p);
    BOOST_CHECK_CLOSE(cprob, 1.0, TH);
  }
}

BOOST_AUTO_TEST_CASE(transiton_manual) {
  std::vector<uint64_t> v = { 1, 2, 2, 1, 3, 1, 2, 3, 1 };
  auto &&res = cgGraph::count_transition(v.begin(), v.end());
  auto &p = std::get<0>(res);
  BOOST_CHECK_EQUAL(p.size(), 4);

  auto &cp = std::get<1>(res);
  BOOST_CHECK_EQUAL(cp.size(), 6);
  BOOST_CHECK_CLOSE(cp(1, 2), 2.0 / 3.0, TH);
  BOOST_CHECK_CLOSE(cp(1, 3), 1.0 / 3.0, TH);
  BOOST_CHECK_CLOSE(cp(2, 2), 1.0 / 3.0, TH);
  BOOST_CHECK_CLOSE(cp(2, 1), 1.0 / 3.0, TH);
  BOOST_CHECK_CLOSE(cp(2, 3), 1.0 / 3.0, TH);
  BOOST_CHECK_CLOSE(cp(3, 1), 1.0, TH);
}
