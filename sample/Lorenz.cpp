
#include "../SimpleCover.hpp"
#include "../Timeline.hpp"
#include "../algorithm.hpp"

#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/log/attributes/mutable_constant.hpp>

namespace attrs = boost::log::attributes;

const double p = 10.0;
const double r = 28.0;
const double b = 8. / 3.;

void Lorenz(std::vector<double> &v, double dt) {
  double x = v[0];
  double y = v[1];
  double z = v[2];
  v[0] += dt * p * (y - x);
  v[1] += dt * (-x * z + r * x - y);
  v[2] += dt * (x * y - b * z);
}

int main(int argc, char const *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " [radius > 0]" << std::endl;
    return 1;
  }

  // parameters
  double dt = 1.0e-3;
  double r = atof(argv[1]);
  if (r <= 0) {
    std::cerr << "Invalid radius : " << r << " (<=0)" << std::endl;
    return 1;
  }

  // initial
  std::vector<double> v = { 1, 0, 0 };

  // discards transient
  for (unsigned int t = 0; t < 10000; t++) {
    Lorenz(v, dt);
  }

  // Ready for cgGraph
  cgGraph::SimpleCover c(3, r);
  cgGraph::Timeline tl;

  // Main
  for (unsigned int t = 0; t < 10000000; t++) {
    Lorenz(v, dt);
    unsigned long idx = c.get_nearest(v);
    tl.push_back(idx);
  }
  auto tmap = cgGraph::count_transition(tl);
  cgGraph::save_graph(tmap, "pp.bin", &c);

  return 0;
}
