#ifndef BUSINESS_ADAPTERS_ADDRESSED_VALUE_HPP
#define BUSINESS_ADAPTERS_ADDRESSED_VALUE_HPP

#include <optional>
#include <string>
#include <vector>

#include "quality_code.hpp"
#include "utils/json/json_object.hpp"
#include "utils/json/json_writer.hpp"

namespace business::adapters
{

class addressed_value
{
private:
    const std::string _address;
    const std::string _valueJson;
    const quality_code _qualityCode;
    const std::optional<std::string> _errorMessage;

public:
    addressed_value(
        const std::string& address,
        const std::string& valueJson,
        quality_code qualityCode = quality_code::good,
        const std::optional<std::string>& errorMessage = std::nullopt);
    explicit addressed_value(const util::json::json_object& obj);

    addressed_value(const addressed_value&) = default;
    ~addressed_value() = default;
    addressed_value& operator=(const addressed_value&) = delete;
    addressed_value(addressed_value&&) = default;
    addressed_value& operator=(addressed_value&&) = delete;

    const std::string& get_address() const;
    const std::string& get_value_json() const;
    quality_code get_quality_code() const;
    const std::optional<std::string>& get_error_message() const;

    std::vector<std::string> validate() const;

    std::string to_json() const;
    void write_json(util::json::json_writer& writer) const;
};

} // namespace business::adapters

#endif // BUSINESS_ADAPTERS_ADDRESSED_VALUE_HPP
