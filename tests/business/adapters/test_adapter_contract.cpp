#include <gtest/gtest.h>

#include <cstdint>
#include <optional>
#include <type_traits>
#include <vector>

#include "business/adapters/adapter.hpp"
#include "business/adapters/addressed_value.hpp"
#include "business/adapters/on_change_subscription.hpp"
#include "business/adapters/quality_code.hpp"
#include "business/adapters/read_request.hpp"
#include "business/adapters/read_result.hpp"
#include "business/adapters/trigger_event.hpp"
#include "business/config/adapters/adapter_configuration.hpp"
#include "utils/json/json_deserializer.hpp"

namespace tests::business::adapters
{

TEST(test_adapter_contract, configuration_and_message_types_are_immutable)
{
    static_assert(!std::is_copy_assignable_v<::business::config::adapters::adapter_configuration>);
    static_assert(!std::is_move_assignable_v<::business::config::adapters::adapter_configuration>);
    static_assert(!std::is_copy_assignable_v<::business::adapters::addressed_value>);
    static_assert(!std::is_move_assignable_v<::business::adapters::addressed_value>);
    static_assert(!std::is_copy_assignable_v<::business::adapters::trigger_event>);
    static_assert(!std::is_move_assignable_v<::business::adapters::trigger_event>);
    static_assert(!std::is_copy_assignable_v<::business::adapters::read_request>);
    static_assert(!std::is_move_assignable_v<::business::adapters::read_request>);
    static_assert(!std::is_copy_assignable_v<::business::adapters::read_result>);
    static_assert(!std::is_move_assignable_v<::business::adapters::read_result>);
    static_assert(!std::is_abstract_v<::business::adapters::adapter_event_sink>);

    SUCCEED();
}

TEST(test_adapter_contract, configuration_round_trip_preserves_json_payload)
{
    const ::business::config::adapters::adapter_configuration config(
        "MainPlc",
        "s7",
        R"({"rack":0,"slot":1,"endpoint":"192.168.0.10"})");

    const auto errors = config.validate();
    ASSERT_TRUE(errors.empty());

    const auto roundTrip = util::json::deserialize(config.to_json());
    ASSERT_TRUE(roundTrip.is_object());

    const ::business::config::adapters::adapter_configuration recreated(roundTrip.as_object());
    EXPECT_EQ(recreated.get_adapter_name(), "MainPlc");
    EXPECT_EQ(recreated.get_adapter_type(), "s7");
    EXPECT_EQ(recreated.get_adapter_configuration_json(), R"({"rack":0,"slot":1,"endpoint":"192.168.0.10"})");
}

TEST(test_adapter_contract, quality_codes_match_opc_ua_values)
{
    EXPECT_EQ(static_cast<std::uint32_t>(::business::adapters::quality_code::good), 0x00000000U);
    EXPECT_EQ(static_cast<std::uint32_t>(::business::adapters::quality_code::uncertain), 0x40000000U);
    EXPECT_EQ(static_cast<std::uint32_t>(::business::adapters::quality_code::bad), 0x80000000U);
    EXPECT_EQ(static_cast<std::uint32_t>(::business::adapters::quality_code::bad_timeout), 0x800A0000U);

    const auto parsedGood = ::business::adapters::quality_code_from_string("good");
    const auto parsedTimeout = ::business::adapters::quality_code_from_string("bad_timeout");
    const auto parsedUnknown = ::business::adapters::quality_code_from_string("not_a_real_status");

    ASSERT_TRUE(parsedGood.has_value());
    ASSERT_TRUE(parsedTimeout.has_value());
    EXPECT_EQ(parsedGood.value(), ::business::adapters::quality_code::good);
    EXPECT_EQ(parsedTimeout.value(), ::business::adapters::quality_code::bad_timeout);
    EXPECT_FALSE(parsedUnknown.has_value());
}

TEST(test_adapter_contract, trigger_and_read_messages_contain_triggering_value_and_addresses)
{
    const ::business::adapters::on_change_subscription subscription(
        "ChangeTrigger",
        std::vector<std::string>{"DB1.DBX0.0", "DB1.DBD4"});
    ASSERT_TRUE(subscription.validate().empty());

    const ::business::adapters::trigger_event event(
        "ChangeTrigger",
        "onchange",
        "2026-04-15T18:30:00Z",
        "DB1.DBX0.0",
        "true");

    const auto eventErrors = event.validate();
    ASSERT_TRUE(eventErrors.empty());

    const ::business::adapters::trigger_event cyclicEvent(
        "TimerTrigger",
        "cyclic",
        "2026-04-15T18:30:00Z",
        "",
        R"("2026-04-15T18:30:00Z")");

    const auto cyclicErrors = cyclicEvent.validate();
    ASSERT_TRUE(cyclicErrors.empty());

    const ::business::adapters::read_request request(
        "req-123",
        std::vector<std::string>{"DB1.DBX0.0", "DB1.DBD4"});

    const auto requestErrors = request.validate();
    ASSERT_TRUE(requestErrors.empty());

    const ::business::adapters::read_result result(
        "req-123",
        "2026-04-15T18:30:01Z",
        std::vector<::business::adapters::addressed_value>{
            ::business::adapters::addressed_value(
                "DB1.DBX0.0",
                "true",
                ::business::adapters::quality_code::good),
            ::business::adapters::addressed_value(
                "DB1.DBD4",
                "null",
                ::business::adapters::quality_code::bad_timeout,
                std::optional<std::string>("Read timeout"))});

    const auto resultErrors = result.validate();
    ASSERT_TRUE(resultErrors.empty());
    ASSERT_EQ(result.get_values().size(), 2U);
    EXPECT_EQ(result.get_values()[0].get_address(), "DB1.DBX0.0");
    EXPECT_EQ(result.get_values()[0].get_value_json(), "true");
    EXPECT_EQ(result.get_values()[0].get_quality_code(), ::business::adapters::quality_code::good);
    EXPECT_EQ(result.get_values()[1].get_quality_code(), ::business::adapters::quality_code::bad_timeout);
    ASSERT_TRUE(result.get_values()[1].get_error_message().has_value());
    EXPECT_EQ(result.get_values()[1].get_error_message().value(), "Read timeout");
}

} // namespace tests::business::adapters
