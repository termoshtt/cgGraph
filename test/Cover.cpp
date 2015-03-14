
#include "../Cover.hpp"

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <iostream>

namespace BT = boost::unit_test;

typedef cgGraph::State State;
typedef cgGraph::CoverI CoverI;
typedef cgGraph::SimpleCover sCover;

/** 一直線に進む場合 */
BOOST_AUTO_TEST_CASE(Cover_line_1d) {
  sCover cover(5, .1);

  auto N = cover.dim();
  auto r = cover.get_r();

  State u(N, 0.0);

  const int T0 = 100;
  const int T = T0 * 100;
  const double dr = r / T0 + r / T; // to avoid d(i,j) == r
  BOOST_CHECK_EQUAL(cover.size(), 0);
  for (int t = 0; t < T; ++t) {
    u[0] = dr * t;
    unsigned int idx = cover.get_nearest(u);
    BOOST_CHECK_EQUAL(idx, t / T0);
    BOOST_CHECK_EQUAL(cover.size(), t / T0 + 1);
  }

  unsigned int M = cover.size();
  for (unsigned int m = 0; m < M; ++m) {
    auto &u_m = cover.get_node(m);
    BOOST_CHECK_CLOSE(u_m[0], m * dr * T0, 1e-13);
    for (unsigned int i = 1; i < N; ++i) {
      BOOST_CHECK_CLOSE(u_m[i], 0.0, 1e-13);
    }
  }
  cover.save("omega.bin");

  auto omega = cgGraph::load_Omega("omega.bin");

  unsigned int M_ = omega.size();
  unsigned int N_ = omega[0].size();
  BOOST_CHECK_EQUAL(M_, M);
  BOOST_CHECK_EQUAL(N_, N);

  for (unsigned int m = 0; m < M; ++m) {
    const double *u_m = &omega[m][0];
    BOOST_CHECK_CLOSE(u_m[0], T0 * dr * m, 1e-13);
    for (unsigned int i = 1; i < N; ++i) {
      BOOST_CHECK_CLOSE(u_m[i], 0.0, 1e-13);
    }
  }
}

/** @f$ R^2 @f$ で半径1の円上を周る場合 */
BOOST_AUTO_TEST_CASE(Cover_cycle) {
  sCover cover(5, 0.1);
  auto N = cover.dim();
  BOOST_CHECK(N >= 2); // need R^2

  State u(N);

  const int T0 = 100;
  const int T = T0 * 100;
  const double dth = 2 * M_PI / T0;
  unsigned int count = 0;
  for (int t = 0; t < T; ++t) {
    u[0] = cos(t * dth);
    u[1] = sin(t * dth);
    cover.get_nearest(u);
    if (t == T0 - 1) {
      count = cover.size();
    } // 一周した後は点を生成しないはず
    if (t > T0) {
      BOOST_CHECK_EQUAL(cover.size(), count);
    }
  }
}
