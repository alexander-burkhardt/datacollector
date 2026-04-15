#ifndef UTIL_JSON_JSON_OBJECT_HPP
#define UTIL_JSON_JSON_OBJECT_HPP

#include "json_value.hpp"
#include "json_array.hpp"
#include <functional>
#include <initializer_list>
#include <map>
#include <string>
#include <type_traits>
#include <vector>

namespace util::json
{

class json_object
{
public:
    using value_type = json_value;
    using container_type = std::map<std::string, value_type>;
    using iterator = container_type::iterator;
    using const_iterator = container_type::const_iterator;
    using size_type = container_type::size_type;

    json_object() = default;
    json_object(std::initializer_list<typename container_type::value_type> init);
    json_object(const container_type& values);
    json_object(container_type&& values) noexcept;

    bool empty() const noexcept;
    size_type size() const noexcept;

    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    iterator begin() noexcept;
    iterator end() noexcept;

    bool contains(const std::string& key) const noexcept;
    const value_type* get(const std::string& key) const noexcept;
    value_type* get(const std::string& key) noexcept;
    const value_type& at(const std::string& key) const;
    value_type& operator[](const std::string& key);

    std::string get_string(const std::string& key) const;
    std::int64_t get_int(const std::string& key) const;
    double get_double(const std::string& key) const;
    bool get_bool(const std::string& key) const;
    json_object get_object(const std::string& key) const;

    std::string get_string_or_default(const std::string& key, const std::string& default_value = {}) const;
    std::int64_t get_int_or_default(const std::string& key, std::int64_t default_value = 0) const;
    double get_double_or_default(const std::string& key, double default_value = 0.0) const;
    bool get_bool_or_default(const std::string& key, bool default_value = false) const;
    json_object get_object_or_default(const std::string& key, const json_object& default_value = {}) const;
    
    template <typename Target>
    std::vector<Target> get_array_property(
        const std::string& key,
        const std::function<Target(const json_value&)>& converter) const
    {
        std::vector<Target> result;

        const auto* ptr = get(key);
        if (!ptr)
        {
            return result;
        }
        if (!ptr->is_array())
        {
            throw json_exception("Property '" + key + "' is not an array");
        }

        const auto& array = ptr->as_array();
        result.reserve(array.size());
        for (const auto& item : array)
        {
            result.push_back(converter(item));
        }

        return result;
    }

    template <typename Target>
    Target get_object_property(
        const std::string& key,
        const std::function<Target(const json_object&)>& converter,
        const Target& default_value = {}) const
    {
        const auto* ptr = get(key);
        if (!ptr)
        {
            return default_value;
        }
        if (!ptr->is_object())
        {
            throw json_exception("Property '" + key + "' is not an object");
        }

        return converter(ptr->as_object());
    }

    template <typename... Args>
    std::pair<iterator, bool> emplace(Args&&... args)
    {
        return values_.emplace(std::forward<Args>(args)...);
    }

private:
    container_type values_;
};

} // namespace util::json

#endif // UTIL_JSON_JSON_OBJECT_HPP
