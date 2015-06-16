
#include "logger.hpp"

namespace cgGraph {

void init() {
  auto &lg = logger::get();
  logging::add_common_attributes();
  lg.add_attribute("Tag", log::attrs::make_constant("cgGraph"));
  lg.add_attribute("Scope", log::attrs::named_scope());
}

} // namespace cgGraph
