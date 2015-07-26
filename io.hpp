
#include <fstream>
#include <msgpack.hpp>

namespace cgGraph {

template <typename T> inline void save(const T &obj, std::string filename) {
  std::ofstream ofs(filename,
                    std::ios::out | std::ios::binary | std::ios::trunc);
  if (!ofs)
    throw std::runtime_error("Cannot open file: " + filename);
  msgpack::pack(ofs, obj);
}

} // namespace cgGraph
