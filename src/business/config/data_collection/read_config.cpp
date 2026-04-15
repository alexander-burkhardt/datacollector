#include "read_config.hpp"

namespace business::config::data_collection
{

read_config::read_config(
    const std::string& name,
    const std::vector<variable_config>& variables,
    const std::vector<std::string>& triggers)
    : _variables(variables),
      _name(name),
      _triggers(triggers)
{
}

read_config::read_config(const util::json::json_object& obj)
    : _variables(obj.get_array_property<variable_config>(
          "variables",
          [](const util::json::json_value& value)
          {
              return variable_config(value.as_object());
          })),
      _name(obj.get_string("name")),
      _triggers(obj.get_array_property<std::string>(
          "triggers",
          [](const util::json::json_value& value)
          {
              return value.as_string();
          }))
{
}

const std::string& read_config::get_name() const
{
    return _name;
}

const std::vector<variable_config>& read_config::get_variables() const
{
    return _variables;
}

const std::vector<std::string>& read_config::get_triggers() const
{
    return _triggers;
}

std::string read_config::to_json() const
{
    util::json::json_writer writer;
    write_json(writer);
    return writer.get_json();
}

void read_config::write_json(util::json::json_writer& writer) const
{
    writer.start_object();
    writer.write_property("name", _name);
    writer.write_array_property(
        "triggers",
        [this](util::json::json_writer& array_writer)
        {
            for (const auto& trigger : _triggers)
            {
                array_writer.write_value(trigger);
            }
        });
    writer.write_array_property(
        "variables",
        [this](util::json::json_writer& array_writer)
        {
            for (const auto& variable : _variables)
            {
                variable.write_json(array_writer);
            }
        });
    writer.end_object();
}

} // namespace business::config::data_collection
