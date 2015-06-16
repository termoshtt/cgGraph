
#include "logger.hpp"
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/file.hpp>

namespace logging = boost::log;
namespace attrs = logging::attributes;
namespace keywords = logging::keywords;

namespace cgGraph {

Attrs get_default_attrs() {
  return { { "time", "TimeStamp" },
           { "tag", "Tag" },
           { "run_id", "RunID" },
           { "scope", "Scope" },
           { "severity", "Severity" },
           { "message", "Message" } };
}

void init() {
  logging::add_common_attributes();
  logging::register_simple_formatter_factory<severity, char>("Severity");

  auto &lg = logger::get();
  logging::add_common_attributes();
  lg.add_attribute("Tag", log::attrs::make_constant("cgGraph"));
  lg.add_attribute("Scope", log::attrs::named_scope());
}

void set_run_id(std::string id) {
  auto &lg = logger::get();
  lg.add_attribute("RunID", attrs::make_constant(id));
}

void add_file_log(std::string filename, const Attrs &attr, bool auto_flush) {
  std::stringstream ss;
  for (auto &&pair : attr) {
    const std::string &key = pair.first;
    const std::string &val = pair.second;
    if (!ss.str().empty())
      ss << "\t";
    ss << key << ":%" << val << "%";
  }
  logging::add_file_log(keywords::file_name = filename,
                        keywords::format = ss.str(),
                        keywords::auto_flush = auto_flush);
}

} // namespace cgGraph
