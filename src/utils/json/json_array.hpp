#ifndef UTIL_JSON_JSON_ARRAY_HPP
#define UTIL_JSON_JSON_ARRAY_HPP

#include "json_value.hpp"
#include <initializer_list>
#include <string>
#include <vector>

namespace util::json
{

class json_array
{
public:
    using value_type = json_value;
    using container_type = std::vector<value_type>;
    using iterator = container_type::iterator;
    using const_iterator = container_type::const_iterator;
    using size_type = container_type::size_type;

    json_array() = default;
    json_array(std::initializer_list<value_type> init);
    json_array(const container_type& values);
    json_array(container_type&& values) noexcept;

    bool empty() const noexcept;
    size_type size() const noexcept;

    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    iterator begin() noexcept;
    iterator end() noexcept;

    const value_type& operator[](size_type index) const;
    value_type& operator[](size_type index);
    const value_type& at(size_type index) const;
    value_type& at(size_type index);

    void push_back(const value_type& value);
    void push_back(value_type&& value);

    const value_type* get(size_type index) const noexcept;
    value_type* get(size_type index) noexcept;

    std::string get_string_or_default(size_type index, const std::string& default_value = {}) const;
    std::int64_t get_int_or_default(size_type index, std::int64_t default_value = 0) const;
    double get_double_or_default(size_type index, double default_value = 0.0) const;
    bool get_bool_or_default(size_type index, bool default_value = false) const;

private:
    container_type values_;
};

} // namespace util::json

#endif // UTIL_JSON_JSON_ARRAY_HPP
