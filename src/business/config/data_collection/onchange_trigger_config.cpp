#include "onchange_trigger_config.hpp"

namespace business::config::data_collection
{

onchange_trigger_config::onchange_trigger_config(const std::string& triggerName, const std::vector<std::string>& targetAddresses)
    : trigger_config(triggerName, type_name()),
      _targetAddresses(targetAddresses)
{
}

onchange_trigger_config::onchange_trigger_config(const util::json::json_object& obj)
    : trigger_config(obj),
      _targetAddresses(obj.get_array_property<std::string>(
          "targetAddresses",
          [](const util::json::json_value& value)
          {
              return value.as_string();
          }))
{
    if (get_trigger_type() != type_name())
    {
        throw util::json::json_exception(
            "Trigger '" + get_trigger_name() + "' expected type '" + type_name() + "' but got '" + get_trigger_type() + "'");
    }
}

std::string onchange_trigger_config::type_name()
{
    return "onchange";
}

const std::vector<std::string>& onchange_trigger_config::get_target_addresses() const
{
    return _targetAddresses;
}

void onchange_trigger_config::write_json(util::json::json_writer& writer) const
{
    writer.start_object();
    write_json_properties(writer);
    writer.write_array_property(
        "targetAddresses",
        [this](util::json::json_writer& array_writer)
        {
            for (const auto& targetAddress : _targetAddresses)
            {
                array_writer.write_value(targetAddress);
            }
        });
    writer.end_object();
}

} // namespace business::config::data_collection
