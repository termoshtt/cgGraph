
#include "../SimpleCover.hpp"
#include <yaml-cpp/yaml.h>
#include <msgpack.hpp>
#include <fstream>
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
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " [configure.yml]" << std::endl;
    return 1;
  }

  auto cfg = YAML::LoadFile(argv[1]);
  std::cout << cfg << std::endl;

  // parameters
  double dt = cfg["dt"].as<double>();
  double r = cfg["r"].as<double>();
  uint64_t T = cfg["T"].as<uint64_t>();
  uint64_t interval = cfg["interval"].as<uint64_t>();
  if (r <= 0) {
    std::cerr << "Invalid radius : " << r << " (<=0)" << std::endl;
    return 1;
  }

  // initial
  std::vector<double> v = { 1, 0, 0 };

  // discards transient
  for (unsigned int t = 0; t < T / 100; t++) {
    Lorenz(v, dt);
  }

  // Ready for cgGraph
  cgGraph::SimpleCover c(3, r);
  std::vector<uint64_t> tl;

  // Main
  for (unsigned int t = 0; t < T; t++) {
    Lorenz(v, dt);
    if (t % interval == 0) {
      uint64_t idx = c.get_nearest(v);
      tl.push_back(idx);
    }
  }

  std::ofstream ofs("Lorenz.msg");
  msgpack::pack(ofs, tl);

  return 0;
}
