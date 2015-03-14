
#include "../Cover.hpp"
#include "../Timeline.hpp"
#include "../algorithm.hpp"
#include <iostream>

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
  std::vector<double> v = { 1, 0, 0 };
  double dt = 1e-3;

  for (unsigned int t = 0; t < 10000; t++) {
    Lorenz(v, dt);
  } // discards transient

  cgGraph::SimpleCover c(3, 3.0);
  cgGraph::Timeline tl;
  for (unsigned int t = 0; t < 1000000; t++) {
    Lorenz(v, dt);
    unsigned long idx = c.get_nearest(v);
    tl.push(idx);
  }
  tl.finalize();
  auto tmap = cgGraph::count_transition(tl);
  cgGraph::save_cgGraph(tmap, "pp.bin", &c);

  return 0;
}
