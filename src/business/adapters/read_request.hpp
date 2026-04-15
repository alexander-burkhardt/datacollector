#ifndef BUSINESS_ADAPTERS_READ_REQUEST_HPP
#define BUSINESS_ADAPTERS_READ_REQUEST_HPP

#include <string>
#include <vector>

#include "utils/json/json_object.hpp"
#include "utils/json/json_writer.hpp"

namespace business::adapters
{

class read_request
{
private:
    const std::string _requestId;
    const std::vector<std::string> _addresses;

public:
    read_request(const std::string& requestId, const std::vector<std::string>& addresses);
    explicit read_request(const util::json::json_object& obj);

    read_request(const read_request&) = default;
    ~read_request() = default;
    read_request& operator=(const read_request&) = delete;
    read_request(read_request&&) = default;
    read_request& operator=(read_request&&) = delete;

    const std::string& get_request_id() const;
    const std::vector<std::string>& get_addresses() const;

    std::vector<std::string> validate() const;

    std::string to_json() const;
    void write_json(util::json::json_writer& writer) const;
};

} // namespace business::adapters

#endif // BUSINESS_ADAPTERS_READ_REQUEST_HPP
