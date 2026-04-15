#include "on_change_subscription.hpp"

namespace business::adapters
{

on_change_subscription::on_change_subscription(
    const std::string& triggerName,
    const std::vector<std::string>& targetAddresses)
    : _triggerName(triggerName),
      _targetAddresses(targetAddresses)
{
}

on_change_subscription::on_change_subscription(const util::json::json_object& obj)
    : _triggerName(obj.get_string("triggerName")),
      _targetAddresses(obj.get_array_property<std::string>(
          "targetAddresses",
          [](const util::json::json_value& value)
          {
              return value.as_string();
          }))
{
}

const std::string& on_change_subscription::get_trigger_name() const
{
    return _triggerName;
}

const std::vector<std::string>& on_change_subscription::get_target_addresses() const
{
    return _targetAddresses;
}

std::vector<std::string> on_change_subscription::validate() const
{
    std::vector<std::string> errors;

    if (_triggerName.empty())
    {
        errors.push_back("On-change subscription trigger name must not be empty.");
    }

    if (_targetAddresses.empty())
    {
        errors.push_back("On-change subscription '" + _triggerName + "' must contain at least one target address.");
    }

    for (const auto& address : _targetAddresses)
    {
        if (address.empty())
        {
            errors.push_back("On-change subscription '" + _triggerName + "' contains an empty target address.");
        }
    }

    return errors;
}

std::string on_change_subscription::to_json() const
{
    util::json::json_writer writer;
    write_json(writer);
    return writer.get_json();
}

void on_change_subscription::write_json(util::json::json_writer& writer) const
{
    writer.start_object();
    writer.write_property("triggerName", _triggerName);
    writer.write_array_property(
        "targetAddresses",
        [this](util::json::json_writer& arrayWriter)
        {
            for (const auto& address : _targetAddresses)
            {
                arrayWriter.write_value(address);
            }
        });
    writer.end_object();
}

} // namespace business::adapters
