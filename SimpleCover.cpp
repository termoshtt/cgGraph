
#include "SimpleCover.hpp"
#include "cgGraph.pb.h"

#include <cmath>
#include <fstream>
#include <stdexcept>

namespace cgGraph {

SimpleCover::SimpleCover(unsigned int N, double r) : N(N), r(r), Omega(0) {}

uint64_t SimpleCover::get_nearest(const State &x) {
  uint64_t nearest_idx = 0;
  double min_dist = r;
  uint64_t M = Omega.size();

  for (unsigned int m = 0; m < M; ++m) {
    const State &x_m = Omega[m];
    double d = dist(x, x_m);
    if (d < min_dist) {
      nearest_idx = m;
      min_dist = d;
    }
  }
  if (min_dist < r) {
    return nearest_idx;
  }
  Omega.emplace_back(x);
  return M;
}

const State &SimpleCover::get_node(uint64_t i) const { return Omega.at(i); }
uint64_t SimpleCover::size() const { return Omega.size(); }

void SimpleCover::save(std::string filename) const {
  unsigned long M = Omega.size();
  pb::Omega omega;
  for (uint64_t m = 0; m < M; m++) {
    pb::Point *p = omega.add_point();
    for (uint64_t n = 0; n < N; ++n) {
      p->add_x(Omega[m][n]);
    }
  }
  std::ofstream ofs(filename,
                    std::ios::out | std::ios::binary | std::ios::trunc);
  if (!ofs)
    throw std::runtime_error("Cannot open file: " + filename);
  omega.SerializeToOstream(&ofs);
}

double SimpleCover::distance(uint64_t i, uint64_t j) const {
  return dist(Omega[i], Omega[j]);
}

unsigned int SimpleCover::dim() const { return N; }
double SimpleCover::get_r() const { return r; }
const std::vector<State> &SimpleCover::get_Omega() const { return Omega; }

double SimpleCover::dist(const State &x, const State &y) const {
  double sum = 0.0;
  auto bx = x.begin();
  auto by = y.begin();
  while (bx != x.end()) {
    sum += std::pow(*bx++ - *by++, 2);
  }
  return std::sqrt(sum);
}

} // namespace cgGraph
