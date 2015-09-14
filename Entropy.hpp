#pragma once

#include "Transition.hpp"

namespace cgGraph {

double conditional_entropy(const Prob &p, const cProb &cp);
double conditional_entropy(std::tuple<Prob, cProb>);

} // namespace cgGraph
