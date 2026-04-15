#include "json_value.hpp"
#include "json_array.hpp"
#include "json_object.hpp"

namespace
{

[[noreturn]] void throw_json_type_error(const util::json::json_value& value, const char* expected)
{
    throw util::json::json_exception(
        std::string("Expected ") + expected + ", but got " + value.type_name());
}

} // namespace


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
json_value::json_value(std::int32_t i) : value(static_cast<std::int64_t>(i))
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

std::string json_value::type_name() const
{
    if (is_null())
    {
        return "null";
    }
    if (is_bool())
    {
        return "bool";
    }
    if (is_int())
    {
        return "integer";
    }
    if (is_double())
    {
        return "double";
    }
    if (is_string())
    {
        return "string";
    }
    if (is_array())
    {
        return "array";
    }
    if (is_object())
    {
        return "object";
    }

    return "unknown";
}

bool json_value::as_bool() const
{
    if (!is_bool())
    {
        throw_json_type_error(*this, "bool");
    }

    return std::get<bool>(value);
}
std::int64_t json_value::as_int() const
{
    if (!is_int())
    {
        throw_json_type_error(*this, "integer");
    }

    return std::get<std::int64_t>(value);
}
double json_value::as_double() const
{
    if (!is_double())
    {
        throw_json_type_error(*this, "double");
    }

    return std::get<double>(value);
}
double json_value::as_number() const
{
    if (!is_number())
    {
        throw_json_type_error(*this, "number");
    }

    return is_int() ? static_cast<double>(as_int()) : as_double();
}
const std::string& json_value::as_string() const
{
    if (!is_string())
    {
        throw_json_type_error(*this, "string");
    }

    return std::get<std::string>(value);
}
const json_array& json_value::as_array() const
{
    if (!is_array())
    {
        throw_json_type_error(*this, "array");
    }

    return *std::get<std::shared_ptr<json_array>>(value);
}
const json_object& json_value::as_object() const
{
    if (!is_object())
    {
        throw_json_type_error(*this, "object");
    }

    return *std::get<std::shared_ptr<json_object>>(value);
}
json_array& json_value::as_array()
{
    if (!is_array())
    {
        throw_json_type_error(*this, "array");
    }

    return *std::get<std::shared_ptr<json_array>>(value);
}
json_object& json_value::as_object()
{
    if (!is_object())
    {
        throw_json_type_error(*this, "object");
    }

    return *std::get<std::shared_ptr<json_object>>(value);
}

} // namespace util::json
