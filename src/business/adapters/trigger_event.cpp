#include "trigger_event.hpp"

#include <exception>

#include "utils/json/json_deserializer.hpp"

namespace business::adapters
{

trigger_event::trigger_event(
    const std::string& triggerName,
    const std::string& triggerType,
    const std::string& timestamp,
    const std::string& triggeringAddress,
    const std::string& triggeringValueJson)
    : _triggerName(triggerName),
      _triggerType(triggerType),
      _timestamp(timestamp),
      _triggeringAddress(triggeringAddress),
      _triggeringValueJson(triggeringValueJson)
{
}

trigger_event::trigger_event(const util::json::json_object& obj)
    : _triggerName(obj.get_string("triggerName")),
      _triggerType(obj.get_string("triggerType")),
      _timestamp(obj.get_string("timestamp")),
      _triggeringAddress(obj.get_string_or_default("triggeringAddress", "")),
      _triggeringValueJson(obj.get_string("triggeringValueJson"))
{
}

const std::string& trigger_event::get_trigger_name() const
{
    return _triggerName;
}

const std::string& trigger_event::get_trigger_type() const
{
    return _triggerType;
}

const std::string& trigger_event::get_timestamp() const
{
    return _timestamp;
}

const std::string& trigger_event::get_triggering_address() const
{
    return _triggeringAddress;
}

const std::string& trigger_event::get_triggering_value_json() const
{
    return _triggeringValueJson;
}

std::vector<std::string> trigger_event::validate() const
{
    std::vector<std::string> errors;

    if (_triggerName.empty())
    {
        errors.push_back("Trigger event name must not be empty.");
    }

    if (_triggerType.empty())
    {
        errors.push_back("Trigger event '" + _triggerName + "' must define a trigger type.");
    }

    if (_timestamp.empty())
    {
        errors.push_back("Trigger event '" + _triggerName + "' must contain a timestamp.");
    }

    if (_triggerType != "cyclic" && _triggeringAddress.empty())
    {
        errors.push_back(
            "Trigger event '" + _triggerName + "' must contain a triggering address for non-cyclic triggers.");
    }

    if (_triggeringValueJson.empty())
    {
        errors.push_back("Trigger event '" + _triggerName + "' must contain a triggeringValueJson payload.");
    }
    else
    {
        try
        {
            (void)util::json::deserialize(_triggeringValueJson);
        }
        catch (const std::exception& ex)
        {
            errors.push_back(
                "Trigger event '" + _triggerName + "' contains invalid triggeringValueJson: " +
                std::string(ex.what()));
        }
    }

    return errors;
}

std::string trigger_event::to_json() const
{
    util::json::json_writer writer;
    write_json(writer);
    return writer.get_json();
}

void trigger_event::write_json(util::json::json_writer& writer) const
{
    writer.start_object();
    writer.write_property("triggerName", _triggerName);
    writer.write_property("triggerType", _triggerType);
    writer.write_property("timestamp", _timestamp);
    writer.write_property("triggeringAddress", _triggeringAddress);
    writer.write_property("triggeringValueJson", _triggeringValueJson);
    writer.end_object();
}

} // namespace business::adapters
