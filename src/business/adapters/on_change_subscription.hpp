#ifndef BUSINESS_ADAPTERS_ON_CHANGE_SUBSCRIPTION_HPP
#define BUSINESS_ADAPTERS_ON_CHANGE_SUBSCRIPTION_HPP

#include <string>
#include <vector>

#include "utils/json/json_object.hpp"
#include "utils/json/json_writer.hpp"

namespace business::adapters
{

class on_change_subscription
{
private:
    const std::string _triggerName;
    const std::vector<std::string> _targetAddresses;

public:
    on_change_subscription(const std::string& triggerName, const std::vector<std::string>& targetAddresses);
    explicit on_change_subscription(const util::json::json_object& obj);

    on_change_subscription(const on_change_subscription&) = default;
    ~on_change_subscription() = default;
    on_change_subscription& operator=(const on_change_subscription&) = delete;
    on_change_subscription(on_change_subscription&&) = default;
    on_change_subscription& operator=(on_change_subscription&&) = delete;

    const std::string& get_trigger_name() const;
    const std::vector<std::string>& get_target_addresses() const;

    std::vector<std::string> validate() const;

    std::string to_json() const;
    void write_json(util::json::json_writer& writer) const;
};

} // namespace business::adapters

#endif // BUSINESS_ADAPTERS_ON_CHANGE_SUBSCRIPTION_HPP
