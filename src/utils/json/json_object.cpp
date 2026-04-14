#include "json_object.hpp"

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

std::string json_object::get_string_or_default(const std::string& key, const std::string& default_value) const
{
    if (auto ptr = get(key); ptr && ptr->is_string())
    {
        return ptr->as_string();
    }
    return default_value;
}

std::int64_t json_object::get_int_or_default(const std::string& key, std::int64_t default_value) const
{
    if (auto ptr = get(key); ptr && ptr->is_int())
    {
        return ptr->as_int();
    }
    return default_value;
}

double json_object::get_double_or_default(const std::string& key, double default_value) const
{
    if (auto ptr = get(key); ptr && ptr->is_number())
    {
        return ptr->as_number();
    }
    return default_value;
}

bool json_object::get_bool_or_default(const std::string& key, bool default_value) const
{
    if (auto ptr = get(key); ptr && ptr->is_bool())
    {
        return ptr->as_bool();
    }
    return default_value;
}

json_object json_object::get_object_or_default(const std::string& key, const json_object& default_value) const
{
    if (auto ptr = get(key); ptr && ptr->is_object())
    {
        return ptr->as_object();
    }
    return default_value;
} // namespace util::json
}