
#include "../SimpleCover.hpp"
#include "../Timeline.hpp"
#include "../algorithm.hpp"
#include "../logger.hpp"
#include <iostream>

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
  cgGraph::init();
  auto &lg = cgGraph::logger::get();
  attrs::mutable_constant<unsigned int> counter(0);
  lg.add_attribute("Counter", counter);
  auto attr = cgGraph::get_default_attrs();
  attr.push_back({ "count", "Counter" });

  cgGraph::add_file_log("Lorenz.log", attr);

  std::vector<double> v = { 1, 0, 0 };
  double dt = 1e-3;

  for (unsigned int t = 0; t < 10000; t++) {
    Lorenz(v, dt);
  } // discards transient

  cgGraph::SimpleCover c(3, 3.0);
  cgGraph::Timeline tl;

  for (unsigned int t = 0; t < 10000000; t++) {
    counter.set(t);
    Lorenz(v, dt);
    unsigned long idx = c.get_nearest(v);
    tl.push(idx);
  }
  tl.finalize();
  auto tmap = cgGraph::count_transition(tl);
  cgGraph::save_graph(tmap, "pp.bin", &c);

  return 0;
}
