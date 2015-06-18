#pragma once

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/attributes/attribute.hpp>

namespace cgGraph {

namespace logging = boost::log;

namespace log {
namespace attrs = logging::attributes;
namespace src = logging::sources;
}

using severity = logging::trivial::severity_level;

BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(logger,
                                       log::src::severity_logger_mt<severity>);
BOOST_LOG_ATTRIBUTE_KEYWORD(line_id, "LineID", unsigned int)
BOOST_LOG_ATTRIBUTE_KEYWORD(severity_attr, "Severity", severity)
BOOST_LOG_ATTRIBUTE_KEYWORD(tag_attr, "Tag", std::string)
BOOST_LOG_ATTRIBUTE_KEYWORD(run_id, "RunID", std::string)
BOOST_LOG_ATTRIBUTE_KEYWORD(scope, "Scope",
                            boost::log::attributes::named_scope::value_type)

typedef std::vector<std::pair<std::string, std::string> > Attrs;
Attrs get_default_attrs();
void init();
void set_run_id(std::string id);
void add_file_log(std::string filename, const Attrs &attr = get_default_attrs(),
                  bool append = false, bool auto_flush = false);

} // namespace cgGraph
