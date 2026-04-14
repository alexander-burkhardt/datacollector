#ifndef UTIL_JSON_JSON_DESERIALIZER_HPP
#define UTIL_JSON_JSON_DESERIALIZER_HPP

#include "json_value.hpp"
#include <string>

namespace util::json
{

json_value deserialize(const std::string& json_string);

} // namespace util::json

#endif // UTIL_JSON_JSON_DESERIALIZER_HPP
