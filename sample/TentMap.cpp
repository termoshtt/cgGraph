
#include "../information_function.hpp"
#include "../io.hpp"
#include <iostream>

std::vector<uint64_t> iterate(double mu, double x, uint64_t N) {
  std::vector<uint64_t> tl;
  for (uint64_t t = 0; t < N; ++t) {
    if (x < 0.5) {
      x = mu * x;
      tl.push_back(0);
    } else {
      x = mu * (1 - x);
      tl.push_back(1);
    }
  }
  return tl;
}

int main(int argc, char const *argv[]) {
  auto tl = iterate(1.9999, 0.123, 1 << 20);
  auto info_func = cgGraph::information_function(tl);
  cgGraph::save(info_func, "TentMap.msg");
  return 0;
}
