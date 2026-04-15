#include "adapter_configuration.hpp"

#include <exception>

#include "utils/json/json_deserializer.hpp"

namespace business::config::adapters
{

adapter_configuration::adapter_configuration(
    const std::string& adapterName,
    const std::string& adapterType,
    const std::string& adapterConfigurationJson)
    : _adapterName(adapterName),
      _adapterType(adapterType),
      _adapterConfigurationJson(adapterConfigurationJson)
{
}

adapter_configuration::adapter_configuration(const util::json::json_object& obj)
    : _adapterName(obj.get_string("adapterName")),
      _adapterType(obj.get_string("adapterType")),
      _adapterConfigurationJson(obj.get_string_or_default("configurationJson", "{}"))
{
}

const std::string& adapter_configuration::get_adapter_name() const
{
    return _adapterName;
}

const std::string& adapter_configuration::get_adapter_type() const
{
    return _adapterType;
}

const std::string& adapter_configuration::get_adapter_configuration_json() const
{
    return _adapterConfigurationJson;
}

std::vector<std::string> adapter_configuration::validate() const
{
    std::vector<std::string> errors;

    if (_adapterName.empty())
    {
        errors.push_back("Adapter name must not be empty.");
    }

    if (_adapterType.empty())
    {
        errors.push_back("Adapter type must not be empty.");
    }

    if (_adapterConfigurationJson.empty())
    {
        errors.push_back("Adapter configuration JSON must not be empty.");
    }
    else
    {
        try
        {
            (void)util::json::deserialize(_adapterConfigurationJson);
        }
        catch (const std::exception& ex)
        {
            errors.push_back("Adapter configuration JSON is invalid: " + std::string(ex.what()));
        }
    }

    return errors;
}

std::string adapter_configuration::to_json() const
{
    util::json::json_writer writer;
    write_json(writer);
    return writer.get_json();
}

void adapter_configuration::write_json(util::json::json_writer& writer) const
{
    writer.start_object();
    writer.write_property("adapterName", _adapterName);
    writer.write_property("adapterType", _adapterType);
    writer.write_property("configurationJson", _adapterConfigurationJson);
    writer.end_object();
}

} // namespace business::config::adapters
