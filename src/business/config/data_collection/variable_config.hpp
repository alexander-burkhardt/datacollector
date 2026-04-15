#ifndef BUSINESS_CONFIG_DATA_COLLECTION_VARIABLE_CONFIG_HPP
#define BUSINESS_CONFIG_DATA_COLLECTION_VARIABLE_CONFIG_HPP

#include <string>
#include <vector>

#include "utils/json/json_object.hpp"
#include "utils/json/json_writer.hpp"

namespace business::config::data_collection
{

class variable_config
{
private:
    std::string _variableName;
    std::string _targetAddress;

public:
    variable_config(const std::string& variableName, const std::string& targetAddress);
    explicit variable_config(const util::json::json_object& obj);

    variable_config(const variable_config&) = default;
    ~variable_config() = default;
    variable_config& operator=(const variable_config&) = default;
    variable_config(variable_config&&) = default;
    variable_config& operator=(variable_config&&) = default;

    const std::string& get_variable_name() const;
    const std::string& get_target_address() const;

    std::vector<std::string> validate() const;

    std::string to_json() const;

    void write_json(util::json::json_writer& writer) const;
};

} // namespace business::config::data_collection

#endif // BUSINESS_CONFIG_DATA_COLLECTION_VARIABLE_CONFIG_HPP