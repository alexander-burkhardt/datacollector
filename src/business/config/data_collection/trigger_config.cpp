#include "trigger_config.hpp"

namespace business::config::data_collection
{

trigger_config::trigger_config(const std::string& triggerName, const std::string& triggerType)
    : _triggerName(triggerName),
      _triggerType(triggerType)
{
}

trigger_config::trigger_config(const util::json::json_object& obj)
    : _triggerName(obj.get_string("triggerName")),
      _triggerType(obj.get_string("triggerType"))
{
}

void trigger_config::write_json_properties(util::json::json_writer& writer) const
{
    writer.write_property("triggerName", _triggerName);
    writer.write_property("triggerType", _triggerType);
}

const std::string& trigger_config::get_trigger_name() const
{
    return _triggerName;
}

const std::string& trigger_config::get_trigger_type() const
{
    return _triggerType;
}

std::vector<std::string> trigger_config::validate() const
{
    std::vector<std::string> errors;

    if (_triggerName.empty())
    {
        errors.push_back("Trigger name must not be empty.");
    }

    if (_triggerType.empty())
    {
        errors.push_back("Trigger '" + _triggerName + "' must define a triggerType.");
    }

    return errors;
}

std::string trigger_config::to_json() const
{
    util::json::json_writer writer;
    write_json(writer);
    return writer.get_json();
}

} // namespace business::config::data_collection
