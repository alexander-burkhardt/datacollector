#include "json_array.hpp"

namespace util::json
{

json_array::json_array(std::initializer_list<value_type> init) : values_(init)
{
}
json_array::json_array(const container_type& values) : values_(values)
{
}
json_array::json_array(container_type&& values) noexcept : values_(std::move(values))
{
}

bool json_array::empty() const noexcept
{
    return values_.empty();
}
json_array::size_type json_array::size() const noexcept
{
    return values_.size();
}

json_array::const_iterator json_array::begin() const noexcept
{
    return values_.begin();
}
json_array::const_iterator json_array::end() const noexcept
{
    return values_.end();
}
json_array::iterator json_array::begin() noexcept
{
    return values_.begin();
}
json_array::iterator json_array::end() noexcept
{
    return values_.end();
}

const json_array::value_type& json_array::operator[](size_type index) const
{
    return values_[index];
}
json_array::value_type& json_array::operator[](size_type index)
{
    return values_[index];
}
const json_array::value_type& json_array::at(size_type index) const
{
    return values_.at(index);
}
json_array::value_type& json_array::at(size_type index)
{
    return values_.at(index);
}

void json_array::push_back(const value_type& value)
{
    values_.push_back(value);
}
void json_array::push_back(value_type&& value)
{
    values_.push_back(std::move(value));
}

const json_array::value_type* json_array::get(size_type index) const noexcept
{
    return index < values_.size() ? &values_[index] : nullptr;
}
json_array::value_type* json_array::get(size_type index) noexcept
{
    return index < values_.size() ? &values_[index] : nullptr;
}

std::string json_array::get_string_or_default(size_type index, const std::string& default_value) const
{
    if (auto ptr = get(index); ptr && ptr->is_string())
    {
        return ptr->as_string();
    }
    return default_value;
}

std::int64_t json_array::get_int_or_default(size_type index, std::int64_t default_value) const
{
    if (auto ptr = get(index); ptr && ptr->is_int())
    {
        return ptr->as_int();
    }
    return default_value;
}

double json_array::get_double_or_default(size_type index, double default_value) const
{
    if (auto ptr = get(index); ptr && ptr->is_number())
    {
        return ptr->as_number();
    }
    return default_value;
}

bool json_array::get_bool_or_default(size_type index, bool default_value) const
{
    if (auto ptr = get(index); ptr && ptr->is_bool())
    {
        return ptr->as_bool();
    }
    return default_value;
}

} // namespace util::json
