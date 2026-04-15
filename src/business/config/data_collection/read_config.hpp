#ifndef BUSINESS_CONFIG_DATA_COLLECTION_READ_CONFIG_HPP
#define BUSINESS_CONFIG_DATA_COLLECTION_READ_CONFIG_HPP

#include <set>
#include <string>
#include <vector>

#include "variable_config.hpp"

namespace business::config::data_collection
{

class read_config
{
private:
    std::string _name;
    std::vector<variable_config> _variables;
    std::vector<std::string> _triggers;

public:
    read_config(
        const std::string& name,
        const std::vector<variable_config>& variables = {},
        const std::vector<std::string>& triggers = {});
    explicit read_config(const util::json::json_object& obj);

    read_config(const read_config&) = default;
    ~read_config() = default;
    read_config& operator=(const read_config&) = default;
    read_config(read_config&&) = default;
    read_config& operator=(read_config&&) = default;

    const std::string& get_name() const;
    const std::vector<variable_config>& get_variables() const;
    const std::vector<std::string>& get_triggers() const;

    std::vector<std::string> validate() const;
    std::vector<std::string> validate(const std::set<std::string>& availableTriggerNames) const;

    std::string to_json() const;

    void write_json(util::json::json_writer& writer) const;
};

} // namespace business::config::data_collection

#endif // BUSINESS_CONFIG_DATA_COLLECTION_READ_CONFIG_HPP
