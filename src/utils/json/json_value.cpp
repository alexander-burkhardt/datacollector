#include "json_value.hpp"
#include "json_array.hpp"
#include "json_object.hpp"

namespace util::json
{

json_value::json_value() noexcept : value(nullptr)
{
}
json_value::json_value(std::nullptr_t) noexcept : value(nullptr)
{
}
json_value::json_value(bool b) : value(b)
{
}
json_value::json_value(std::int64_t i) : value(i)
{
}
json_value::json_value(double d) : value(d)
{
}
json_value::json_value(int i) : value(static_cast<std::int64_t>(i))
{
}
json_value::json_value(const char* s) : value(std::string(s))
{
}
json_value::json_value(const std::string& s) : value(s)
{
}
json_value::json_value(std::string&& s) : value(std::move(s))
{
}
json_value::json_value(const json_array& a) : value(std::make_shared<json_array>(a))
{
}
json_value::json_value(json_array&& a) : value(std::make_shared<json_array>(std::move(a)))
{
}
json_value::json_value(const json_object& o) : value(std::make_shared<json_object>(o))
{
}
json_value::json_value(json_object&& o) : value(std::make_shared<json_object>(std::move(o)))
{
}

bool json_value::is_null() const noexcept
{
    return std::holds_alternative<std::nullptr_t>(value);
}
bool json_value::is_bool() const noexcept
{
    return std::holds_alternative<bool>(value);
}
bool json_value::is_int() const noexcept
{
    return std::holds_alternative<std::int64_t>(value);
}
bool json_value::is_double() const noexcept
{
    return std::holds_alternative<double>(value);
}
bool json_value::is_number() const noexcept
{
    return is_int() || is_double();
}
bool json_value::is_string() const noexcept
{
    return std::holds_alternative<std::string>(value);
}
bool json_value::is_array() const noexcept
{
    return std::holds_alternative<std::shared_ptr<json_array>>(value);
}
bool json_value::is_object() const noexcept
{
    return std::holds_alternative<std::shared_ptr<json_object>>(value);
}

bool json_value::as_bool() const
{
    return std::get<bool>(value);
}
std::int64_t json_value::as_int() const
{
    return std::get<std::int64_t>(value);
}
double json_value::as_double() const
{
    return std::get<double>(value);
}
double json_value::as_number() const
{
    return is_int() ? static_cast<double>(as_int()) : as_double();
}
const std::string& json_value::as_string() const
{
    return std::get<std::string>(value);
}
const json_array& json_value::as_array() const
{
    return *std::get<std::shared_ptr<json_array>>(value);
}
const json_object& json_value::as_object() const
{
    return *std::get<std::shared_ptr<json_object>>(value);
}
json_array& json_value::as_array()
{
    return *std::get<std::shared_ptr<json_array>>(value);
}
json_object& json_value::as_object()
{
    return *std::get<std::shared_ptr<json_object>>(value);
}

} // namespace util::json
