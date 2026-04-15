#ifndef UTIL_JSON_JSON_ARRAY_HPP
#define UTIL_JSON_JSON_ARRAY_HPP

#include "json_value.hpp"
#include "json_integer.hpp"
#include <initializer_list>
#include <string>
#include <vector>

namespace util::json
{

class json_array
{
public:
    json_array() = default;
    json_array(std::initializer_list<json_value> init);
    json_array(const std::vector<json_value>& values);
    json_array(std::vector<json_value>&& values) noexcept;

    bool empty() const noexcept;
    std::size_t size() const noexcept;

    std::vector<json_value>::const_iterator begin() const noexcept;
    std::vector<json_value>::const_iterator end() const noexcept;
    std::vector<json_value>::iterator begin() noexcept;
    std::vector<json_value>::iterator end() noexcept;

    const json_value& operator[](std::size_t index) const;
    json_value& operator[](std::size_t index);
    const json_value& at(std::size_t index) const;
    json_value& at(std::size_t index);

    void push_back(const json_value& value);
    void push_back(json_value&& value);

    const json_value* get(std::size_t index) const noexcept;
    json_value* get(std::size_t index) noexcept;

    std::string get_string(std::size_t index) const;
    std::int64_t get_int(std::size_t index) const;
    double get_double(std::size_t index) const;
    bool get_bool(std::size_t index) const;

    template <typename IntegerType>
    IntegerType get_integer(std::size_t index) const
    {
        return checked_integer_cast<IntegerType>(get_int(index), "JSON array element at index " + std::to_string(index));
    }

    std::string get_string_or_default(std::size_t index, const std::string& default_value = {}) const;
    std::int64_t get_int_or_default(std::size_t index, std::int64_t default_value = 0) const;
    double get_double_or_default(std::size_t index, double default_value = 0.0) const;
    bool get_bool_or_default(std::size_t index, bool default_value = false) const;

    template <typename IntegerType>
    IntegerType get_integer_or_default(std::size_t index, IntegerType default_value = 0) const
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

        return checked_integer_cast<IntegerType>(ptr->as_int(), "JSON array element at index " + std::to_string(index));
    }

private:
    std::vector<json_value> values_;
};

} // namespace util::json

#endif // UTIL_JSON_JSON_ARRAY_HPP
