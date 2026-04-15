#ifndef BUSINESS_ADAPTERS_ADAPTER_HPP
#define BUSINESS_ADAPTERS_ADAPTER_HPP

#include <string>
#include <vector>

#include "business/config/adapters/adapter_configuration.hpp"
#include "on_change_subscription.hpp"
#include "read_request.hpp"
#include "read_result.hpp"
#include "trigger_event.hpp"

namespace business::adapters
{

class adapter_event_sink
{
public:
    adapter_event_sink() = default;
    virtual ~adapter_event_sink() = default;

    adapter_event_sink(const adapter_event_sink&) = default;
    adapter_event_sink& operator=(const adapter_event_sink&) = default;
    adapter_event_sink(adapter_event_sink&&) = default;
    adapter_event_sink& operator=(adapter_event_sink&&) = default;

    virtual void on_trigger(const trigger_event& event);
    virtual void on_read_result(const read_result& result);
    virtual void on_adapter_error(const std::string& adapterName, const std::string& errorMessage);
};

class adapter
{
public:
    adapter() = default;
    virtual ~adapter() = default;

    adapter(const adapter&) = delete;
    adapter& operator=(const adapter&) = delete;
    adapter(adapter&&) = delete;
    adapter& operator=(adapter&&) = delete;

    virtual std::string get_adapter_name() const = 0;
    virtual std::string get_adapter_type() const = 0;

    virtual std::vector<std::string> configure(
        const business::config::adapters::adapter_configuration& configuration) = 0;

    virtual std::vector<std::string> configure_on_change_triggers(
        const std::vector<on_change_subscription>& subscriptions) = 0;

    virtual std::vector<std::string> read_variables_async(
        const read_request& request,
        adapter_event_sink& eventSink) = 0;

    virtual void stop() = 0;
};

} // namespace business::adapters

#endif // BUSINESS_ADAPTERS_ADAPTER_HPP
