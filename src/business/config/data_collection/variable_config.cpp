#include "variable_config.hpp"

namespace business::config::data_collection
{

variable_config::variable_config(const std::string& variableName, const std::string& targetAddress)
    : _variableName(variableName),
      _targetAddress(targetAddress)
{
}

variable_config::variable_config(const util::json::json_object& obj)
    : _variableName(obj.get_string("variableName")),
      _targetAddress(obj.get_string("targetAddress"))
{
}

const std::string& variable_config::get_variable_name() const
{
    return _variableName;
}

const std::string& variable_config::get_target_address() const
{
    return _targetAddress;
}

std::vector<std::string> variable_config::validate() const
{
    std::vector<std::string> errors;

    if (_variableName.empty())
    {
        errors.push_back("Variable name must not be empty.");
    }

    if (_targetAddress.empty())
    {
        errors.push_back("Variable '" + _variableName + "' must define a targetAddress.");
    }

    return errors;
}

std::string variable_config::to_json() const
{
    util::json::json_writer writer;
    write_json(writer);
    return writer.get_json();
}

void variable_config::write_json(util::json::json_writer& writer) const
{
    writer.start_object();
    writer.write_property("variableName", _variableName);
    writer.write_property("targetAddress", _targetAddress);
    writer.end_object();
}

} // namespace business::config::data_collection
