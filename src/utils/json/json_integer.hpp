#ifndef UTIL_JSON_JSON_INTEGER_HPP
#define UTIL_JSON_JSON_INTEGER_HPP

#include "json_exception.hpp"

#include <cstdint>
#include <limits>
#include <string>
#include <type_traits>

namespace util::json
{

template <typename IntegerType>
std::string integer_type_name()
{
    static_assert(std::is_integral_v<IntegerType> && !std::is_same_v<IntegerType, bool>, "IntegerType must be a non-bool integral type");

    if constexpr (std::is_same_v<IntegerType, std::int8_t>)
    {
        return "int8";
    }
    else if constexpr (std::is_same_v<IntegerType, std::int16_t>)
    {
        return "int16";
    }
    else if constexpr (std::is_same_v<IntegerType, std::int32_t>)
    {
        return "int32";
    }
    else if constexpr (std::is_same_v<IntegerType, std::int64_t>)
    {
        return "int64";
    }
    else if constexpr (std::is_same_v<IntegerType, std::uint8_t>)
    {
        return "uint8";
    }
    else if constexpr (std::is_same_v<IntegerType, std::uint16_t>)
    {
        return "uint16";
    }
    else if constexpr (std::is_same_v<IntegerType, std::uint32_t>)
    {
        return "uint32";
    }
    else if constexpr (std::is_same_v<IntegerType, std::uint64_t>)
    {
        return "uint64";
    }

    return "requested integer type";
}

template <typename IntegerType>
IntegerType checked_integer_cast(std::int64_t value, const std::string& context)
{
    static_assert(std::is_integral_v<IntegerType> && !std::is_same_v<IntegerType, bool>, "IntegerType must be a non-bool integral type");

    if constexpr (std::is_signed_v<IntegerType>)
    {
        const auto minValue = static_cast<std::int64_t>(std::numeric_limits<IntegerType>::min());
        const auto maxValue = static_cast<std::int64_t>(std::numeric_limits<IntegerType>::max());

        if (value < minValue || value > maxValue)
        {
            throw json_exception(context + " is out of range for " + integer_type_name<IntegerType>());
        }
    }
    else
    {
        if (value < 0 || static_cast<std::uint64_t>(value) > std::numeric_limits<IntegerType>::max())
        {
            throw json_exception(context + " is out of range for " + integer_type_name<IntegerType>());
        }
    }

    return static_cast<IntegerType>(value);
}

} // namespace util::json

#endif // UTIL_JSON_JSON_INTEGER_HPP
