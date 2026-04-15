#include "simulated_values_adapter.hpp"

#include "utils/json/json_deserializer.hpp"

namespace business::adapters
{

namespace
{

std::string build_timestamp()
{
    return "1970-01-01T00:00:00Z";
}

} // namespace

std::string simulated_values_adapter::get_adapter_name() const
{
    return _adapterName;
}

std::string simulated_values_adapter::get_adapter_type() const
{
    return "simulated_values";
}

std::vector<std::string> simulated_values_adapter::configure(
    const business::config::adapters::adapter_configuration& configuration)
{
    std::vector<std::string> errors = configuration.validate();
    if (!errors.empty())
    {
        return errors;
    }

    if (configuration.get_adapter_type() != get_adapter_type())
    {
        return {"Simulated values adapter received unsupported adapter type '" +
                configuration.get_adapter_type() + "'."};
    }

    _adapterName = configuration.get_adapter_name();
    _valuesByAddress.clear();

    const auto configJson = util::json::deserialize(configuration.get_adapter_configuration_json());
    if (!configJson.is_object())
    {
        return {"Simulated values adapter configuration must be a JSON object."};
    }

    const auto valuesObject = configJson.as_object().get_object_or_default("values", {});
    for (const auto& [address, value] : valuesObject)
    {
        _valuesByAddress[address] = util::json::json_writer().get_json();
        if (value.is_string())
        {
            util::json::json_writer writer;
            writer.write_value(value.as_string());
            _valuesByAddress[address] = writer.get_json();
        }
        else if (value.is_bool())
        {
            util::json::json_writer writer;
            writer.write_value(value.as_bool());
            _valuesByAddress[address] = writer.get_json();
        }
        else if (value.is_int())
        {
            util::json::json_writer writer;
            writer.write_value(value.as_int());
            _valuesByAddress[address] = writer.get_json();
        }
        else if (value.is_double())
        {
            util::json::json_writer writer;
            writer.write_value(value.as_double());
            _valuesByAddress[address] = writer.get_json();
        }
        else if (value.is_null())
        {
            _valuesByAddress[address] = "null";
        }
        else
        {
            return {"Simulated value for address '" + address + "' must be a scalar JSON value."};
        }
    }

    return {};
}

std::vector<std::string> simulated_values_adapter::configure_on_change_triggers(
    const std::vector<on_change_subscription>& subscriptions)
{
    std::vector<std::string> errors;
    _subscriptions.clear();

    for (const auto& subscription : subscriptions)
    {
        const auto subscriptionErrors = subscription.validate();
        errors.insert(errors.end(), subscriptionErrors.begin(), subscriptionErrors.end());
        _subscriptions.push_back(subscription);
    }

    return errors;
}

std::vector<std::string> simulated_values_adapter::read_variables_async(
    const read_request& request,
    adapter_event_sink& eventSink)
{
    std::vector<std::string> errors = request.validate();
    if (!errors.empty())
    {
        return errors;
    }

    std::vector<addressed_value> values;
    values.reserve(request.get_addresses().size());

    for (const auto& address : request.get_addresses())
    {
        const auto it = _valuesByAddress.find(address);
        if (it != _valuesByAddress.end())
        {
            values.emplace_back(address, it->second, quality_code::good);

            for (const auto& subscription : _subscriptions)
            {
                for (const auto& subscribedAddress : subscription.get_target_addresses())
                {
                    if (subscribedAddress == address)
                    {
                        eventSink.on_trigger(trigger_event(
                            subscription.get_trigger_name(),
                            "onchange",
                            build_timestamp(),
                            address,
                            it->second));
                    }
                }
            }
        }
        else
        {
            values.emplace_back(
                address,
                "null",
                quality_code::bad_no_communication,
                std::optional<std::string>("No simulated value configured for address."));
        }
    }

    eventSink.on_read_result(read_result(request.get_request_id(), build_timestamp(), values));
    return {};
}

void simulated_values_adapter::stop()
{
    _subscriptions.clear();
}

} // namespace business::adapters
