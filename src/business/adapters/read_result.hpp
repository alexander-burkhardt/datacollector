#ifndef BUSINESS_ADAPTERS_READ_RESULT_HPP
#define BUSINESS_ADAPTERS_READ_RESULT_HPP

#include <string>
#include <vector>

#include "addressed_value.hpp"
#include "utils/json/json_object.hpp"
#include "utils/json/json_writer.hpp"

namespace business::adapters
{

class read_result
{
private:
    const std::string _requestId;
    const std::string _timestamp;
    const std::vector<addressed_value> _values;

public:
    read_result(const std::string& requestId, const std::string& timestamp, const std::vector<addressed_value>& values);
    explicit read_result(const util::json::json_object& obj);

    read_result(const read_result&) = default;
    ~read_result() = default;
    read_result& operator=(const read_result&) = delete;
    read_result(read_result&&) = default;
    read_result& operator=(read_result&&) = delete;

    const std::string& get_request_id() const;
    const std::string& get_timestamp() const;
    const std::vector<addressed_value>& get_values() const;

    std::vector<std::string> validate() const;

    std::string to_json() const;
    void write_json(util::json::json_writer& writer) const;
};

} // namespace business::adapters

#endif // BUSINESS_ADAPTERS_READ_RESULT_HPP
