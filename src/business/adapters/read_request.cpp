#include "read_request.hpp"

namespace business::adapters
{

read_request::read_request(const std::string& requestId, const std::vector<std::string>& addresses)
    : _requestId(requestId),
      _addresses(addresses)
{
}

read_request::read_request(const util::json::json_object& obj)
    : _requestId(obj.get_string("requestId")),
      _addresses(obj.get_array_property<std::string>(
          "addresses",
          [](const util::json::json_value& value)
          {
              return value.as_string();
          }))
{
}

const std::string& read_request::get_request_id() const
{
    return _requestId;
}

const std::vector<std::string>& read_request::get_addresses() const
{
    return _addresses;
}

std::vector<std::string> read_request::validate() const
{
    std::vector<std::string> errors;

    if (_requestId.empty())
    {
        errors.push_back("Read request id must not be empty.");
    }

    if (_addresses.empty())
    {
        errors.push_back("Read request '" + _requestId + "' must contain at least one address.");
    }

    for (const auto& address : _addresses)
    {
        if (address.empty())
        {
            errors.push_back("Read request '" + _requestId + "' contains an empty address.");
        }
    }

    return errors;
}

std::string read_request::to_json() const
{
    util::json::json_writer writer;
    write_json(writer);
    return writer.get_json();
}

void read_request::write_json(util::json::json_writer& writer) const
{
    writer.start_object();
    writer.write_property("requestId", _requestId);
    writer.write_array_property(
        "addresses",
        [this](util::json::json_writer& arrayWriter)
        {
            for (const auto& address : _addresses)
            {
                arrayWriter.write_value(address);
            }
        });
    writer.end_object();
}

} // namespace business::adapters
