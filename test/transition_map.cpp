
#include "../TransitionMap.hpp"

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <iostream>

typedef cgGraph::TransitionMap TMap;

const double TH = 1e-13;

void show(TMap &tmap) {
  std::cout << "-- State --\n";
  auto vert = tmap.get_states();
  for (unsigned int i = 0; i < vert.size(); i++) {
    std::cout << i << " " << vert[i].duration << " " << vert[i].prob
              << std::endl;
  }
  std::cout << "--- Transit ----\n";
  auto transits = tmap.get_transits();
  for (auto e : transits) {
    std::cout << "(" << std::get<0>(e.first) << "," << std::get<1>(e.first)
              << ") " << e.second.count << " " << e.second.prob << std::endl;
  }
}

BOOST_AUTO_TEST_CASE(tmap_push) {
  TMap tmap;

  tmap.push(0, 1, 3);
  tmap.push(1, 4, 2);
  tmap.push(4, 2, 2);
  tmap.push(2, 3, 5);
  tmap.push(3, 0, 1);
  tmap.push(0, 2, 2);
  tmap.push(2, 3, 4);
  tmap.push(3, 0, 2);

  tmap.finalize();
  show(tmap);

  BOOST_CHECK_EQUAL(tmap.transition_count(), 8);
  BOOST_CHECK_EQUAL(tmap.combination_count(), 6);
  BOOST_CHECK_EQUAL(tmap.time(), 3 + 2 + 2 + 5 + 1 + 2 + 4 + 2);

  auto vert = tmap.get_states();
  BOOST_CHECK_EQUAL(vert.size(), 5);

  BOOST_CHECK_EQUAL(vert[0].duration, 3 + 2);
  BOOST_CHECK_EQUAL(vert[0].count, 2);
  BOOST_CHECK_CLOSE(vert[0].prob, (double)(3 + 2) / tmap.time(), TH);
  BOOST_CHECK_CLOSE(vert[0].rate, 2.0 / (3 + 2), TH);
  BOOST_CHECK_CLOSE(vert[0].entropy, 1.0, TH);

  BOOST_CHECK_EQUAL(vert[1].duration, 2);
  BOOST_CHECK_EQUAL(vert[1].count, 1);
  BOOST_CHECK_CLOSE(vert[1].prob, (double)(2) / tmap.time(), TH);
  BOOST_CHECK_CLOSE(vert[1].rate, 1.0 / 2, TH);
  BOOST_CHECK_CLOSE(vert[1].entropy, 0.0, TH);

  BOOST_CHECK_EQUAL(vert[2].duration, 5 + 4);
  BOOST_CHECK_EQUAL(vert[2].count, 2);
  BOOST_CHECK_CLOSE(vert[2].prob, (double)(5 + 4) / tmap.time(), TH);
  BOOST_CHECK_CLOSE(vert[2].rate, 2.0 / (5 + 4), TH);
  BOOST_CHECK_CLOSE(vert[2].entropy, 0.0, TH);

  BOOST_CHECK_EQUAL(vert[3].duration, 1 + 2);
  BOOST_CHECK_EQUAL(vert[3].count, 2);
  BOOST_CHECK_CLOSE(vert[3].prob, (double)(1 + 2) / tmap.time(), TH);
  BOOST_CHECK_CLOSE(vert[3].rate, 2.0 / (1 + 2), TH);
  BOOST_CHECK_CLOSE(vert[3].entropy, 0.0, TH);

  BOOST_CHECK_EQUAL(vert[4].duration, 2);
  BOOST_CHECK_EQUAL(vert[4].count, 1);
  BOOST_CHECK_CLOSE(vert[4].prob, (double)(2) / tmap.time(), TH);
  BOOST_CHECK_CLOSE(vert[4].rate, 1.0 / 2, TH);
  BOOST_CHECK_CLOSE(vert[4].entropy, 0.0, TH);

  try {
    tmap.get_transit(0, 0);
  } catch (std::out_of_range &e) {
    std::cout << "transit (0,0) does not exist" << std::endl;
  }

  BOOST_CHECK_EQUAL(tmap.get_transit(0, 1).count, 1);
  BOOST_CHECK_CLOSE(tmap.get_transit(0, 1).prob, 0.5, TH);
  BOOST_CHECK_EQUAL(tmap.get_transit(0, 2).count, 1);
  BOOST_CHECK_CLOSE(tmap.get_transit(0, 2).prob, 0.5, TH);
  BOOST_CHECK_EQUAL(tmap.get_transit(1, 4).count, 1);
  BOOST_CHECK_CLOSE(tmap.get_transit(1, 4).prob, 1.0, TH);
  BOOST_CHECK_EQUAL(tmap.get_transit(4, 2).count, 1);
  BOOST_CHECK_CLOSE(tmap.get_transit(4, 2).prob, 1.0, TH);
  BOOST_CHECK_EQUAL(tmap.get_transit(2, 3).count, 2);
  BOOST_CHECK_CLOSE(tmap.get_transit(2, 3).prob, 1.0, TH);
  BOOST_CHECK_EQUAL(tmap.get_transit(3, 0).count, 2);
  BOOST_CHECK_CLOSE(tmap.get_transit(3, 0).prob, 1.0, TH);
}
