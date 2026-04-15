#ifndef BUSINESS_CONFIG_DATA_COLLECTION_ONCHANGE_TRIGGER_CONFIG_HPP
#define BUSINESS_CONFIG_DATA_COLLECTION_ONCHANGE_TRIGGER_CONFIG_HPP

#include <string>
#include <vector>

#include "trigger_config.hpp"
#include "utils/json/json_exception.hpp"

namespace business::config::data_collection
{

class onchange_trigger_config : public trigger_config
{
private:
    std::vector<std::string> _targetAddresses;

public:
    onchange_trigger_config(const std::string& triggerName, const std::vector<std::string>& targetAddresses);
    explicit onchange_trigger_config(const util::json::json_object& obj);

    onchange_trigger_config(const onchange_trigger_config&) = default;
    ~onchange_trigger_config() override = default;
    onchange_trigger_config& operator=(const onchange_trigger_config&) = default;
    onchange_trigger_config(onchange_trigger_config&&) = default;
    onchange_trigger_config& operator=(onchange_trigger_config&&) = default;

    static std::string type_name();

    const std::vector<std::string>& get_target_addresses() const;

    void write_json(util::json::json_writer& writer) const override;
};

} // namespace business::config::data_collection

#endif // BUSINESS_CONFIG_DATA_COLLECTION_ONCHANGE_TRIGGER_CONFIG_HPP
