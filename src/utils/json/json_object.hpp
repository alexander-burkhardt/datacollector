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
    json_object() = default;
    json_object(std::initializer_list<std::pair<const std::string, json_value>> init);
    json_object(const std::map<std::string, json_value>& values);
    json_object(std::map<std::string, json_value>&& values) noexcept;

    bool empty() const noexcept;
    std::size_t size() const noexcept;

    std::map<std::string, json_value>::const_iterator begin() const noexcept;
    std::map<std::string, json_value>::const_iterator end() const noexcept;
    std::map<std::string, json_value>::iterator begin() noexcept;
    std::map<std::string, json_value>::iterator end() noexcept;

    bool contains(const std::string& key) const noexcept;
    const json_value* get(const std::string& key) const noexcept;
    json_value* get(const std::string& key) noexcept;
    const json_value& at(const std::string& key) const;
    json_value& operator[](const std::string& key);

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
    std::pair<std::map<std::string, json_value>::iterator, bool> emplace(Args&&... args)
    {
        return values_.emplace(std::forward<Args>(args)...);
    }

private:
    std::map<std::string, json_value> values_;
};

} // namespace util::json

#endif // UTIL_JSON_JSON_OBJECT_HPP
