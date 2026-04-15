#include "data_collection_config.hpp"

#include <set>

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

std::vector<std::string> data_collection_config::validate() const
{
    std::vector<std::string> errors;

    if (_configurationName.empty())
    {
        errors.push_back("Configuration name must not be empty.");
    }

    std::set<std::string> availableTriggerNames;
    for (const auto& trigger : _triggers)
    {
        if (!trigger)
        {
            errors.push_back(
                "Configuration '" + _configurationName + "' contains a null trigger entry.");
            continue;
        }

        const auto triggerErrors = trigger->validate();
        errors.insert(errors.end(), triggerErrors.begin(), triggerErrors.end());

        const auto [_, inserted] = availableTriggerNames.insert(trigger->get_trigger_name());
        if (!inserted)
        {
            errors.push_back(
                "Configuration '" + _configurationName + "' contains duplicate trigger name '" +
                trigger->get_trigger_name() + "'.");
        }
    }

    std::set<std::string> readNames;
    for (const auto& read : _reads)
    {
        if (!read)
        {
            errors.push_back(
                "Configuration '" + _configurationName + "' contains a null read entry.");
            continue;
        }

        const auto readErrors = read->validate(availableTriggerNames);
        errors.insert(errors.end(), readErrors.begin(), readErrors.end());

        const auto [_, inserted] = readNames.insert(read->get_name());
        if (!inserted)
        {
            errors.push_back(
                "Configuration '" + _configurationName + "' contains duplicate read name '" +
                read->get_name() + "'.");
        }
    }

    return errors;
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
