
#include "Cover.hpp"
#include "cgGraph.pb.h"

#include <fstream>
#include <stdexcept>

namespace cgGraph {

std::vector<State> load_Omega(std::string filename) {
  std::ifstream ifs(filename, std::ios::in | std::ios::binary);
  if (!ifs)
    throw std::runtime_error("Cannot open file: " + filename);
  pb::Omega omega;
  omega.ParseFromIstream(&ifs);
  int M = omega.point_size();
  std::vector<State> Omega(M);

  for (int m = 0; m < M; ++m) {
    auto &p = omega.point(m);
    int N = p.x_size();
    Omega[m].resize(N);
    for (int n = 0; n < N; ++n) {
      Omega[m][n] = p.x(n);
    }
  }
  return std::move(Omega);
}

} // namespace cgGraph
