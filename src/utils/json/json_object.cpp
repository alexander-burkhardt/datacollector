#include "json_object.hpp"

namespace
{

[[noreturn]] void throw_missing_property(const std::string& key, const char* expected)
{
    throw util::json::json_exception("Property '" + key + "' expected " + expected + " but got missing");
}

} // namespace

namespace util::json
{

json_object::json_object(std::initializer_list<container_type::value_type> init) : values_(init)
{
}
json_object::json_object(const container_type& values) : values_(values)
{
}
json_object::json_object(container_type&& values) noexcept : values_(std::move(values))
{
}

bool json_object::empty() const noexcept
{
    return values_.empty();
}
json_object::size_type json_object::size() const noexcept
{
    return values_.size();
}

json_object::const_iterator json_object::begin() const noexcept
{
    return values_.begin();
}
json_object::const_iterator json_object::end() const noexcept
{
    return values_.end();
}
json_object::iterator json_object::begin() noexcept
{
    return values_.begin();
}
json_object::iterator json_object::end() noexcept
{
    return values_.end();
}

bool json_object::contains(const std::string& key) const noexcept
{
    return values_.find(key) != values_.end();
}

const json_object::value_type* json_object::get(const std::string& key) const noexcept
{
    auto it = values_.find(key);
    return it == values_.end() ? nullptr : &it->second;
}

json_object::value_type* json_object::get(const std::string& key) noexcept
{
    auto it = values_.find(key);
    return it == values_.end() ? nullptr : &it->second;
}

const json_object::value_type& json_object::at(const std::string& key) const
{
    return values_.at(key);
}

json_object::value_type& json_object::operator[](const std::string& key)
{
    return values_[key];
}

std::string json_object::get_string(const std::string& key) const
{
    const auto* ptr = get(key);
    if (!ptr)
    {
        throw_missing_property(key, "string");
    }
    if (!ptr->is_string())
    {
        throw json_exception("Property '" + key + "' expected string but got " + ptr->type_name());
    }

    return ptr->as_string();
}

std::int64_t json_object::get_int(const std::string& key) const
{
    const auto* ptr = get(key);
    if (!ptr)
    {
        throw_missing_property(key, "integer");
    }
    if (!ptr->is_int())
    {
        throw json_exception("Property '" + key + "' expected integer but got " + ptr->type_name());
    }

    return ptr->as_int();
}

double json_object::get_double(const std::string& key) const
{
    const auto* ptr = get(key);
    if (!ptr)
    {
        throw_missing_property(key, "number");
    }
    if (!ptr->is_number())
    {
        throw json_exception("Property '" + key + "' expected number but got " + ptr->type_name());
    }

    return ptr->as_number();
}

bool json_object::get_bool(const std::string& key) const
{
    const auto* ptr = get(key);
    if (!ptr)
    {
        throw_missing_property(key, "bool");
    }
    if (!ptr->is_bool())
    {
        throw json_exception("Property '" + key + "' expected bool but got " + ptr->type_name());
    }

    return ptr->as_bool();
}

json_object json_object::get_object(const std::string& key) const
{
    const auto* ptr = get(key);
    if (!ptr)
    {
        throw_missing_property(key, "object");
    }
    if (!ptr->is_object())
    {
        throw json_exception("Property '" + key + "' expected object but got " + ptr->type_name());
    }

    return ptr->as_object();
}

std::string json_object::get_string_or_default(const std::string& key, const std::string& default_value) const
{
    const auto* ptr = get(key);
    if (!ptr)
    {
        return default_value;
    }
    if (!ptr->is_string())
    {
        throw json_exception("Property '" + key + "' expected string but got " + ptr->type_name());
    }

    return ptr->as_string();
}

std::int64_t json_object::get_int_or_default(const std::string& key, std::int64_t default_value) const
{
    const auto* ptr = get(key);
    if (!ptr)
    {
        return default_value;
    }
    if (!ptr->is_int())
    {
        throw json_exception("Property '" + key + "' expected integer but got " + ptr->type_name());
    }

    return ptr->as_int();
}

double json_object::get_double_or_default(const std::string& key, double default_value) const
{
    const auto* ptr = get(key);
    if (!ptr)
    {
        return default_value;
    }
    if (!ptr->is_number())
    {
        throw json_exception("Property '" + key + "' expected number but got " + ptr->type_name());
    }

    return ptr->as_number();
}

bool json_object::get_bool_or_default(const std::string& key, bool default_value) const
{
    const auto* ptr = get(key);
    if (!ptr)
    {
        return default_value;
    }
    if (!ptr->is_bool())
    {
        throw json_exception("Property '" + key + "' expected bool but got " + ptr->type_name());
    }

    return ptr->as_bool();
}

json_object json_object::get_object_or_default(const std::string& key, const json_object& default_value) const
{
    const auto* ptr = get(key);
    if (!ptr)
    {
        return default_value;
    }
    if (!ptr->is_object())
    {
        throw json_exception("Property '" + key + "' expected object but got " + ptr->type_name());
    }

    return ptr->as_object();
}

} // namespace util::json