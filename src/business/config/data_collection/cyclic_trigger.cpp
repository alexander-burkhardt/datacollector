#include "cyclic_trigger.hpp"

namespace business::config::data_collection
{

cyclic_trigger_config::cyclic_trigger_config(const std::string& triggerName, std::int32_t frequencyInSeconds)
    : trigger_config(triggerName, type_name()),
      _frequencyInSeconds(frequencyInSeconds)
{
}

cyclic_trigger_config::cyclic_trigger_config(const util::json::json_object& obj)
    : trigger_config(obj),
      _frequencyInSeconds(obj.get_integer<std::int32_t>("frequencyInSeconds"))
{
    if (get_trigger_type() != type_name())
    {
        throw util::json::json_exception(
            "Trigger '" + get_trigger_name() + "' expected type '" + type_name() + "' but got '" + get_trigger_type() + "'");
    }
}

std::string cyclic_trigger_config::type_name()
{
    return "cyclic";
}

std::int32_t cyclic_trigger_config::get_frequency_in_seconds() const
{
    return _frequencyInSeconds;
}

void cyclic_trigger_config::write_json(util::json::json_writer& writer) const
{
    writer.start_object();
    write_json_properties(writer);
    writer.write_property("frequencyInSeconds", static_cast<std::int64_t>(_frequencyInSeconds));
    writer.end_object();
}

} // namespace business::config::data_collection
