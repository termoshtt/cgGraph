
#include "../Timeline.hpp"
#include "../exception.hpp"

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <iostream>

/** 単純に追加した場合 */
BOOST_AUTO_TEST_CASE(ctimeline_push) {
  cgGraph::CompressedTimeline tl;
  tl.push_back(1);
  tl.push_back(1);
  tl.push_back(2);
  tl.push_back(2);
  tl.push_back(1);

  for (auto &&v : tl) {
    std::cout << v << std::endl;
  }

  std::cout << tl.chunk_size() << std::endl;
  BOOST_CHECK_EQUAL(tl.size(), 5);
}
