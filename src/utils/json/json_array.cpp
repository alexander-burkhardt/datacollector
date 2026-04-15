#include "json_array.hpp"

namespace
{

[[noreturn]] void throw_missing_element(std::size_t index, const char* expected)
{
    throw util::json::json_exception("JSON array element at index " + std::to_string(index) + " expected " + expected + " but got missing");
}

} // namespace

namespace util::json
{

json_array::json_array(std::initializer_list<json_value> init) : values_(init)
{
}
json_array::json_array(const std::vector<json_value>& values) : values_(values)
{
}
json_array::json_array(std::vector<json_value>&& values) noexcept : values_(std::move(values))
{
}

bool json_array::empty() const noexcept
{
    return values_.empty();
}
std::size_t json_array::size() const noexcept
{
    return values_.size();
}

std::vector<json_value>::const_iterator json_array::begin() const noexcept
{
    return values_.begin();
}
std::vector<json_value>::const_iterator json_array::end() const noexcept
{
    return values_.end();
}
std::vector<json_value>::iterator json_array::begin() noexcept
{
    return values_.begin();
}
std::vector<json_value>::iterator json_array::end() noexcept
{
    return values_.end();
}

const json_value& json_array::operator[](std::size_t index) const
{
    return values_[index];
}
json_value& json_array::operator[](std::size_t index)
{
    return values_[index];
}
const json_value& json_array::at(std::size_t index) const
{
    return values_.at(index);
}
json_value& json_array::at(std::size_t index)
{
    return values_.at(index);
}

void json_array::push_back(const json_value& value)
{
    values_.push_back(value);
}
void json_array::push_back(json_value&& value)
{
    values_.push_back(std::move(value));
}

const json_value* json_array::get(std::size_t index) const noexcept
{
    return index < values_.size() ? &values_[index] : nullptr;
}
json_value* json_array::get(std::size_t index) noexcept
{
    return index < values_.size() ? &values_[index] : nullptr;
}

std::string json_array::get_string(std::size_t index) const
{
    const auto* ptr = get(index);
    if (!ptr)
    {
        throw_missing_element(index, "string");
    }
    if (!ptr->is_string())
    {
        throw json_exception("JSON array element at index " + std::to_string(index) + " expected string but got " + ptr->type_name());
    }

    return ptr->as_string();
}

std::int64_t json_array::get_int(std::size_t index) const
{
    const auto* ptr = get(index);
    if (!ptr)
    {
        throw_missing_element(index, "integer");
    }
    if (!ptr->is_int())
    {
        throw json_exception("JSON array element at index " + std::to_string(index) + " expected integer but got " + ptr->type_name());
    }

    return ptr->as_int();
}

double json_array::get_double(std::size_t index) const
{
    const auto* ptr = get(index);
    if (!ptr)
    {
        throw_missing_element(index, "number");
    }
    if (!ptr->is_number())
    {
        throw json_exception("JSON array element at index " + std::to_string(index) + " expected number but got " + ptr->type_name());
    }

    return ptr->as_number();
}

bool json_array::get_bool(std::size_t index) const
{
    const auto* ptr = get(index);
    if (!ptr)
    {
        throw_missing_element(index, "bool");
    }
    if (!ptr->is_bool())
    {
        throw json_exception("JSON array element at index " + std::to_string(index) + " expected bool but got " + ptr->type_name());
    }

    return ptr->as_bool();
}

std::string json_array::get_string_or_default(std::size_t index, const std::string& default_value) const
{
    const auto* ptr = get(index);
    if (!ptr)
    {
        return default_value;
    }
    if (!ptr->is_string())
    {
        throw json_exception("JSON array element at index " + std::to_string(index) + " expected string but got " + ptr->type_name());
    }

    return ptr->as_string();
}

std::int64_t json_array::get_int_or_default(std::size_t index, std::int64_t default_value) const
{
    const auto* ptr = get(index);
    if (!ptr)
    {
        return default_value;
    }
    if (!ptr->is_int())
    {
        throw json_exception("JSON array element at index " + std::to_string(index) + " expected integer but got " + ptr->type_name());
    }

    return ptr->as_int();
}

double json_array::get_double_or_default(std::size_t index, double default_value) const
{
    const auto* ptr = get(index);
    if (!ptr)
    {
        return default_value;
    }
    if (!ptr->is_number())
    {
        throw json_exception("JSON array element at index " + std::to_string(index) + " expected number but got " + ptr->type_name());
    }

    return ptr->as_number();
}

bool json_array::get_bool_or_default(std::size_t index, bool default_value) const
{
    const auto* ptr = get(index);
    if (!ptr)
    {
        return default_value;
    }
    if (!ptr->is_bool())
    {
        throw json_exception("JSON array element at index " + std::to_string(index) + " expected bool but got " + ptr->type_name());
    }

    return ptr->as_bool();
}

} // namespace util::json
