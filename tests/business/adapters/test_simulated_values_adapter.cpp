#include <gtest/gtest.h>

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "business/adapters/read_result.hpp"
#include "business/adapters/simulated_values/simulated_values_adapter.hpp"
#include "business/adapters/trigger_event.hpp"
#include "business/config/adapters/adapter_configuration.hpp"

namespace tests::business::adapters
{

namespace
{

class collecting_event_sink : public ::business::adapters::adapter_event_sink
{
public:
    std::vector<::business::adapters::trigger_event> triggers;
    std::vector<::business::adapters::read_result> readResults;
    std::vector<std::string> errors;

    void on_trigger(const ::business::adapters::trigger_event& event) override
    {
        triggers.push_back(event);
    }

    void on_read_result(const ::business::adapters::read_result& result) override
    {
        readResults.push_back(result);
    }

    void on_adapter_error(const std::string& adapterName, const std::string& errorMessage) override
    {
        errors.push_back(adapterName + ":" + errorMessage);
    }
};

} // namespace

TEST(test_simulated_values_adapter, configure_and_read_returns_simulated_values)
{
    ::business::adapters::simulated_values_adapter adapter;
    collecting_event_sink sink;

    const ::business::config::adapters::adapter_configuration configuration(
        "Sim1",
        "simulated_values",
        R"({
            "values": {
                "SIM.temp": 21,
                "SIM.ready": true
            }
        })");

    const auto configErrors = adapter.configure(configuration);
    ASSERT_TRUE(configErrors.empty());

    const ::business::adapters::read_request request(
        "read-1",
        std::vector<std::string>{"SIM.temp", "SIM.ready", "SIM.missing"});

    const auto readErrors = adapter.read_variables_async(request, sink);
    ASSERT_TRUE(readErrors.empty());
    ASSERT_EQ(sink.readResults.size(), 1U);
    ASSERT_TRUE(sink.errors.empty());

    const auto& result = sink.readResults[0];
    ASSERT_EQ(result.get_values().size(), 3U);
    EXPECT_EQ(result.get_values()[0].get_address(), "SIM.temp");
    EXPECT_EQ(result.get_values()[0].get_value_json(), "21");
    EXPECT_EQ(result.get_values()[1].get_value_json(), "true");
    EXPECT_EQ(result.get_values()[2].get_quality_code(), ::business::adapters::quality_code::bad_no_communication);
    ASSERT_TRUE(result.get_values()[2].get_error_message().has_value());
}

TEST(test_simulated_values_adapter, on_change_configuration_emits_trigger_event_for_known_address)
{
    ::business::adapters::simulated_values_adapter adapter;
    collecting_event_sink sink;

    const ::business::config::adapters::adapter_configuration configuration(
        "Sim2",
        "simulated_values",
        R"({
            "values": {
                "SIM.switch": false
            }
        })");

    ASSERT_TRUE(adapter.configure(configuration).empty());

    const auto triggerErrors = adapter.configure_on_change_triggers(
        std::vector<::business::adapters::on_change_subscription>{
            ::business::adapters::on_change_subscription("SwitchTrigger", {"SIM.switch"})});
    ASSERT_TRUE(triggerErrors.empty());

    ASSERT_EQ(sink.triggers.size(), 0U);

    const ::business::adapters::read_request request("read-2", std::vector<std::string>{"SIM.switch"});
    ASSERT_TRUE(adapter.read_variables_async(request, sink).empty());

    ASSERT_EQ(sink.triggers.size(), 1U);
    EXPECT_EQ(sink.triggers[0].get_trigger_name(), "SwitchTrigger");
    EXPECT_EQ(sink.triggers[0].get_triggering_address(), "SIM.switch");
    EXPECT_EQ(sink.triggers[0].get_triggering_value_json(), "false");
}

} // namespace tests::business::adapters
