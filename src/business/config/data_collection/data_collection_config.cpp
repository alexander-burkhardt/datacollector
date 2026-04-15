#include "data_collection_config.hpp"

#include "trigger_config_factory.hpp"

namespace business::config::data_collection
{

data_collection_config::data_collection_config(
    const std::string& configurationName,
    const std::vector<std::shared_ptr<trigger_config>>& triggers,
    const std::vector<std::shared_ptr<read_config>>& reads)
    : _configurationName(configurationName),
      _triggers(triggers),
      _reads(reads)
{
}

data_collection_config::data_collection_config(const util::json::json_object& obj)
    : _configurationName(obj.get_string("configurationName")),
      _triggers(obj.get_array_property<std::shared_ptr<trigger_config>>(
          "triggers",
          [](const util::json::json_value& value)
          {
              return trigger_config_factory::create(value.as_object());
          })),
      _reads(obj.get_array_property<std::shared_ptr<read_config>>(
          "reads",
          [](const util::json::json_value& value)
          {
              return std::make_shared<read_config>(value.as_object());
          }))
{
}

const std::string& data_collection_config::get_configuration_name() const
{
    return _configurationName;
}

const std::vector<std::shared_ptr<trigger_config>>& data_collection_config::get_triggers() const
{
    return _triggers;
}

const std::vector<std::shared_ptr<read_config>>& data_collection_config::get_reads() const
{
    return _reads;
}

std::string data_collection_config::to_json() const
{
    util::json::json_writer writer;
    write_json(writer);
    return writer.get_json();
}

void data_collection_config::write_json(util::json::json_writer& writer) const
{
    writer.start_object();
    writer.write_property("configurationName", _configurationName);
    writer.write_array_property(
        "triggers",
        [this](util::json::json_writer& array_writer)
        {
            for (const auto& trigger : _triggers)
            {
                trigger->write_json(array_writer);
            }
        });
    writer.write_array_property(
        "reads",
        [this](util::json::json_writer& array_writer)
        {
            for (const auto& read : _reads)
            {
                read->write_json(array_writer);
            }
        });
    writer.end_object();
}

} // namespace business::config::data_collection
