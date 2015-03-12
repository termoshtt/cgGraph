
#include "Cover.hpp"
#include "cgGraph.pb.h"

#include "utility/algorithm.hpp"
#include "utility/binary.hpp"
#include "utility/range.hpp"

#include <fstream>
#include <stdexcept>

namespace U = utility;
namespace R = U::range;

namespace DS {
namespace ProbProcess {

template <typename Int, typename Real>
SimpleCover<Int, Real>::SimpleCover(unsigned int N, double r)
    : N(N), r(r), Omega(0) {}

template <typename Int, typename Real>
Int SimpleCover<Int, Real>::get_nearest(const State<Real> &x) {
  Int nearest_idx = 0;
  double min_dist = r;
  Int M = Omega.size();
  for (unsigned int m = 0; m < M; ++m) {
    const State<Real> &x_m = Omega[m];
    double dist = R::euclid_distance(x, x_m);
    if (dist < min_dist) {
      nearest_idx = m;
      min_dist = dist;
    }
  }
  if (min_dist < r) {
    return nearest_idx;
  }
  Omega.emplace_back(x);
  return M;
}

template <typename Int, typename Real>
const State<Real> &SimpleCover<Int, Real>::get_node(Int i) const {
  return Omega.at(i);
}

template <typename Int, typename Real>
Int SimpleCover<Int, Real>::size() const {
  return Omega.size();
}

template <typename Int, typename Real>
void SimpleCover<Int, Real>::save(std::string filename) const {
  unsigned long M = Omega.size();
  pb::Omega omega;
  for (Int m = 0; m < M; m++) {
    pb::Point *p = omega.add_point();
    for (Int n = 0; n < N; ++n) {
      p->add_x(Omega[m][n]);
    }
  }
  auto ofs = U::binary::ostream(filename);
  omega.SerializeToOstream(ofs.get());
}

template <typename Int, typename Real>
double SimpleCover<Int, Real>::distance(Int i, Int j) const {
  return U::euclid_distance(Omega[i].begin(), Omega[i].end(), Omega[j].begin());
}

template <typename Int, typename Real>
unsigned int SimpleCover<Int, Real>::dim() const {
  return N;
}

template <typename Int, typename Real>
double SimpleCover<Int, Real>::get_r() const {
  return r;
}

template <typename Int, typename Real>
const std::vector<State<Real> > &SimpleCover<Int, Real>::get_Omega() const {
  return Omega;
}

template <typename Real = double>
std::vector<State<Real> > load_Omega(std::string filename) {
  auto ifs = U::binary::istream(filename);
  pb::Omega omega;
  omega.ParseFromIstream(ifs.get());
  int M = omega.point_size();
  std::vector<State<Real> > Omega(M);

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

template class SimpleCover<unsigned int, float>;
template class SimpleCover<unsigned int, double>;
template class SimpleCover<unsigned long, float>;
template class SimpleCover<unsigned long, double>;

template std::vector<std::vector<float> >
load_Omega<float>(std::string filename);
template std::vector<std::vector<double> >
load_Omega<double>(std::string filename);

} // namespace ProbProcess
} // namespace DS
