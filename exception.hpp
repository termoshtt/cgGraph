
#include <stdexcept>

namespace cgGraph {

class cgGraphError : public std::runtime_error {
public:
  explicit cgGraphError(const std::string &msg) : std::runtime_error(msg) {}
};

} // namespace cgGraph
