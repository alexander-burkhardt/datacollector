#ifndef UTIL_JSON_JSON_VALUE_HPP
#define UTIL_JSON_JSON_VALUE_HPP

#include "json_exception.hpp"
#include <cstdint>
#include <memory>
#include <string>
#include <variant>

namespace util::json
{

class json_array;
class json_object;

class json_value
{
public:
    using value_type = std::variant<std::nullptr_t, bool, std::int64_t, double, std::string, std::shared_ptr<json_array>, std::shared_ptr<json_object>>;
    value_type value;

    json_value() noexcept;
    json_value(std::nullptr_t) noexcept;
    json_value(bool b);
    json_value(std::int64_t i);
    json_value(double d);
    json_value(int i);
    json_value(const char* s);
    json_value(const std::string& s);
    json_value(std::string&& s);
    json_value(const json_array& a);
    json_value(json_array&& a);
    json_value(const json_object& o);
    json_value(json_object&& o);

    bool is_null() const noexcept;
    bool is_bool() const noexcept;
    bool is_int() const noexcept;
    bool is_double() const noexcept;
    bool is_number() const noexcept;
    bool is_string() const noexcept;
    bool is_array() const noexcept;
    bool is_object() const noexcept;

    std::string type_name() const;

    bool as_bool() const;
    std::int64_t as_int() const;
    double as_double() const;
    double as_number() const;
    const std::string& as_string() const;
    const json_array& as_array() const;
    const json_object& as_object() const;
    json_array& as_array();
    json_object& as_object();
};

} // namespace util::json

#endif // UTIL_JSON_JSON_VALUE_HPP
