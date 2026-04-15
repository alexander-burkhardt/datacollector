#include "addressed_value.hpp"

#include <exception>

#include "utils/json/json_deserializer.hpp"

namespace business::adapters
{

addressed_value::addressed_value(
    const std::string& address,
    const std::string& valueJson,
    quality_code qualityCode,
    const std::optional<std::string>& errorMessage)
    : _address(address),
      _valueJson(valueJson),
      _qualityCode(qualityCode),
      _errorMessage(errorMessage)
{
}

addressed_value::addressed_value(const util::json::json_object& obj)
    : _address(obj.get_string("address")),
      _valueJson(obj.get_string("valueJson")),
      _qualityCode(static_cast<quality_code>(
          obj.get_integer_or_default<std::uint32_t>(
              "qualityCode",
              static_cast<std::uint32_t>(quality_code::good)))),
      _errorMessage(
          obj.contains("errorMessage")
              ? std::optional<std::string>(obj.get_string("errorMessage"))
              : std::nullopt)
{
}

const std::string& addressed_value::get_address() const
{
    return _address;
}

const std::string& addressed_value::get_value_json() const
{
    return _valueJson;
}

quality_code addressed_value::get_quality_code() const
{
    return _qualityCode;
}

const std::optional<std::string>& addressed_value::get_error_message() const
{
    return _errorMessage;
}

std::vector<std::string> addressed_value::validate() const
{
    std::vector<std::string> errors;

    if (_address.empty())
    {
        errors.push_back("Address must not be empty.");
    }

    if (_valueJson.empty())
    {
        errors.push_back("Value JSON for address '" + _address + "' must not be empty.");
    }
    else
    {
        try
        {
            (void)util::json::deserialize(_valueJson);
        }
        catch (const std::exception& ex)
        {
            errors.push_back(
                "Value JSON for address '" + _address + "' is invalid JSON: " + std::string(ex.what()));
        }
    }

    if (!is_known_quality_code(_qualityCode))
    {
        errors.push_back(
            "Quality code for address '" + _address + "' is not part of the supported OPC UA status set.");
    }

    if (_errorMessage.has_value() && _errorMessage->empty())
    {
        errors.push_back("Error message for address '" + _address + "' must not be empty when present.");
    }

    return errors;
}

std::string addressed_value::to_json() const
{
    util::json::json_writer writer;
    write_json(writer);
    return writer.get_json();
}

void addressed_value::write_json(util::json::json_writer& writer) const
{
    writer.start_object();
    writer.write_property("address", _address);
    writer.write_property("valueJson", _valueJson);
    writer.write_property("qualityCode", static_cast<std::int64_t>(static_cast<std::uint32_t>(_qualityCode)));
    if (_errorMessage.has_value())
    {
        writer.write_property("errorMessage", *_errorMessage);
    }
    writer.end_object();
}

} // namespace business::adapters
