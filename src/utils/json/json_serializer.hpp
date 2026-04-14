#ifndef UTIL_JSON_JSON_SERIALIZER_HPP
#define UTIL_JSON_JSON_SERIALIZER_HPP

#include "json_value.hpp"
#include <string>

namespace util::json
{

std::string serialize(const json_value& value, bool pretty = false);

} // namespace util::json

#endif // UTIL_JSON_JSON_SERIALIZER_HPP
