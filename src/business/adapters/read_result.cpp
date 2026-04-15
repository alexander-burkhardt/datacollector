#include "read_result.hpp"

namespace business::adapters
{

read_result::read_result(
    const std::string& requestId,
    const std::string& timestamp,
    const std::vector<addressed_value>& values)
    : _requestId(requestId),
      _timestamp(timestamp),
      _values(values)
{
}

read_result::read_result(const util::json::json_object& obj)
    : _requestId(obj.get_string("requestId")),
      _timestamp(obj.get_string("timestamp")),
      _values(obj.get_array_property<addressed_value>(
          "values",
          [](const util::json::json_value& value)
          {
              return addressed_value(value.as_object());
          }))
{
}

const std::string& read_result::get_request_id() const
{
    return _requestId;
}

const std::string& read_result::get_timestamp() const
{
    return _timestamp;
}

const std::vector<addressed_value>& read_result::get_values() const
{
    return _values;
}

std::vector<std::string> read_result::validate() const
{
    std::vector<std::string> errors;

    if (_requestId.empty())
    {
        errors.push_back("Read result request id must not be empty.");
    }

    if (_timestamp.empty())
    {
        errors.push_back("Read result '" + _requestId + "' must contain a timestamp.");
    }

    for (const auto& value : _values)
    {
        const auto valueErrors = value.validate();
        errors.insert(errors.end(), valueErrors.begin(), valueErrors.end());
    }

    return errors;
}

std::string read_result::to_json() const
{
    util::json::json_writer writer;
    write_json(writer);
    return writer.get_json();
}

void read_result::write_json(util::json::json_writer& writer) const
{
    writer.start_object();
    writer.write_property("requestId", _requestId);
    writer.write_property("timestamp", _timestamp);
    writer.write_array_property(
        "values",
        [this](util::json::json_writer& arrayWriter)
        {
            for (const auto& value : _values)
            {
                value.write_json(arrayWriter);
            }
        });
    writer.end_object();
}

} // namespace business::adapters
