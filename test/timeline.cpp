
#include "../Timeline.hpp"
#include "../exception.hpp"

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <iostream>

typedef cgGraph::Timeline<unsigned long> TL;

/** begin/endのテスト */
void show(const TL &tl, std::ostream &ost) {
  for (auto p : tl) {
    ost << std::get<0>(p) << " " << std::get<1>(p) << std::endl;
  }
}

/** 単純に追加した場合 */
BOOST_AUTO_TEST_CASE(timeline_push) {
  TL tl;
  tl.push(1);
  tl.push(2);
  tl.push(2);
  tl.push(2);
  tl.push(2);
  tl.push(3);
  tl.push(3);

  show(tl, std::cout);

  // before finalize
  BOOST_CHECK_EQUAL(tl.size(), 2);
  auto itr = tl.begin();
  auto t = *itr++;
  BOOST_CHECK_EQUAL(std::get<0>(t), 1);
  BOOST_CHECK_EQUAL(std::get<1>(t), 1);
  t = *itr++;
  BOOST_CHECK_EQUAL(std::get<0>(t), 2);
  BOOST_CHECK_EQUAL(std::get<1>(t), 4);
  BOOST_CHECK(itr == tl.end());

  // after finalize
  tl.finalize();
  BOOST_CHECK_EQUAL(tl.size(), 3);
  itr = tl.begin();
  t = *itr++;
  BOOST_CHECK_EQUAL(std::get<0>(t), 1);
  BOOST_CHECK_EQUAL(std::get<1>(t), 1);
  t = *itr++;
  BOOST_CHECK_EQUAL(std::get<0>(t), 2);
  BOOST_CHECK_EQUAL(std::get<1>(t), 4);
  t = *itr++;
  BOOST_CHECK_EQUAL(std::get<0>(t), 3);
  BOOST_CHECK_EQUAL(std::get<1>(t), 2);
  BOOST_CHECK(itr == tl.end());

  // cannot push after `finalize()`
  try {
    tl.push(4);
  } catch (cgGraph::cgGraphError &e) {
    std::cout << e.what() << std::endl;
    BOOST_CHECK(true);
  }
}

BOOST_AUTO_TEST_CASE(timeline_io) {
  const unsigned int N = 10;
  std::random_device rnd;
  std::mt19937 gen(rnd());
  std::uniform_int_distribution<unsigned int> dist(0, 9);

  TL tl1;
  for (unsigned int t = 0; t < N; ++t) {
    tl1.push(dist(gen));
  }
  tl1.save("tl.bin");

  TL tl2("tl.bin");
  tl2.finalize();

  auto itr1 = tl1.begin();
  auto itr2 = tl2.begin();
  for (unsigned int t = 0; t < N; ++t) {
    BOOST_CHECK(*itr1 == *itr2);
  }
}
