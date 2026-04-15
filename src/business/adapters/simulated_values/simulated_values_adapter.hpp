#ifndef BUSINESS_ADAPTERS_SIMULATED_VALUES_SIMULATED_VALUES_ADAPTER_HPP
#define BUSINESS_ADAPTERS_SIMULATED_VALUES_SIMULATED_VALUES_ADAPTER_HPP

#include <map>
#include <string>
#include <vector>

#include "business/adapters/adapter.hpp"

namespace business::adapters
{

class simulated_values_adapter : public adapter
{
private:
    std::string _adapterName;
    std::map<std::string, std::string> _valuesByAddress;
    std::vector<on_change_subscription> _subscriptions;

public:
    simulated_values_adapter() = default;
    ~simulated_values_adapter() override = default;

    std::string get_adapter_name() const override;
    std::string get_adapter_type() const override;

    std::vector<std::string> configure(
        const business::config::adapters::adapter_configuration& configuration) override;

    std::vector<std::string> configure_on_change_triggers(
        const std::vector<on_change_subscription>& subscriptions) override;

    std::vector<std::string> read_variables_async(
        const read_request& request,
        adapter_event_sink& eventSink) override;

    void stop() override;
};

} // namespace business::adapters

#endif // BUSINESS_ADAPTERS_SIMULATED_VALUES_SIMULATED_VALUES_ADAPTER_HPP
