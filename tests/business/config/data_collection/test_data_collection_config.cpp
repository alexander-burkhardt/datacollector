#include <gtest/gtest.h>

#include <algorithm>
#include <memory>
#include <string>

#include "business/config/data_collection/cyclic_trigger.hpp"
#include "business/config/data_collection/data_collection_config.hpp"
#include "business/config/data_collection/onchange_trigger_config.hpp"
#include "utils/json/json_deserializer.hpp"
#include "utils/json/json_exception.hpp"

namespace tests::business::config::data_collection
{

namespace
{

std::string sample_config_json()
{
    return R"(
    {
        "configurationName": "MainConfig",
        "triggers": [
            {
                "triggerName": "TimerTrigger",
                "triggerType": "cyclic",
                "frequencyInSeconds": 60
            },
            {
                "triggerName": "ChangeTrigger",
                "triggerType": "onchange",
                "targetAddresses": ["DB1.DBX0.0", "DB1.DBX0.1"]
            }
        ],
        "reads": [
            {
                "name": "MotorRead",
                "triggers": ["TimerTrigger", "ChangeTrigger"],
                "variables": [
                    {
                        "variableName": "MotorSpeed",
                        "targetAddress": "DB10.DBD0"
                    },
                    {
                        "variableName": "MotorTemp",
                        "targetAddress": "DB10.DBD4"
                    }
                ]
            }
        ]
    }
    )";
}

bool contains_message_fragment(const std::vector<std::string>& errors, const std::string& fragment)
{
    return std::any_of(
        errors.begin(),
        errors.end(),
        [&fragment](const std::string& error)
        {
            return error.find(fragment) != std::string::npos;
        });
}

} // namespace

TEST(test_data_collection_config, deserialize_creates_correct_trigger_types)
{
    const auto json_value = util::json::deserialize(sample_config_json());
    ASSERT_TRUE(json_value.is_object());

    const ::business::config::data_collection::data_collection_config config(json_value.as_object());

    const auto errors = config.validate();
    EXPECT_TRUE(errors.empty());
    EXPECT_EQ(config.get_configuration_name(), "MainConfig");
    ASSERT_EQ(config.get_triggers().size(), 2U);
    ASSERT_EQ(config.get_reads().size(), 1U);

    const auto cyclic = std::dynamic_pointer_cast<::business::config::data_collection::cyclic_trigger_config>(config.get_triggers()[0]);
    ASSERT_NE(cyclic, nullptr);
    EXPECT_EQ(cyclic->get_trigger_name(), "TimerTrigger");
    EXPECT_EQ(cyclic->get_trigger_type(), "cyclic");
    EXPECT_EQ(cyclic->get_frequency_in_seconds(), 60);

    const auto onchange = std::dynamic_pointer_cast<::business::config::data_collection::onchange_trigger_config>(config.get_triggers()[1]);
    ASSERT_NE(onchange, nullptr);
    EXPECT_EQ(onchange->get_trigger_name(), "ChangeTrigger");
    EXPECT_EQ(onchange->get_trigger_type(), "onchange");
    ASSERT_EQ(onchange->get_target_addresses().size(), 2U);
    EXPECT_EQ(onchange->get_target_addresses()[0], "DB1.DBX0.0");
    EXPECT_EQ(onchange->get_target_addresses()[1], "DB1.DBX0.1");

    ASSERT_EQ(config.get_reads()[0]->get_variables().size(), 2U);
    EXPECT_EQ(config.get_reads()[0]->get_name(), "MotorRead");
    ASSERT_EQ(config.get_reads()[0]->get_triggers().size(), 2U);
    EXPECT_EQ(config.get_reads()[0]->get_triggers()[0], "TimerTrigger");
    EXPECT_EQ(config.get_reads()[0]->get_triggers()[1], "ChangeTrigger");
    EXPECT_EQ(config.get_reads()[0]->get_variables()[0].get_variable_name(), "MotorSpeed");
    EXPECT_EQ(config.get_reads()[0]->get_variables()[0].get_target_address(), "DB10.DBD0");
}

TEST(test_data_collection_config, serialize_round_trip_preserves_configuration)
{
    const auto json_value = util::json::deserialize(sample_config_json());
    ASSERT_TRUE(json_value.is_object());

    const ::business::config::data_collection::data_collection_config config(json_value.as_object());
    const std::string serialized = config.to_json();

    const auto round_trip_value = util::json::deserialize(serialized);
    ASSERT_TRUE(round_trip_value.is_object());

    const ::business::config::data_collection::data_collection_config round_trip_config(round_trip_value.as_object());

    const auto errors = round_trip_config.validate();
    EXPECT_TRUE(errors.empty());
    EXPECT_EQ(round_trip_config.get_configuration_name(), "MainConfig");
    ASSERT_EQ(round_trip_config.get_triggers().size(), 2U);
    ASSERT_EQ(round_trip_config.get_reads().size(), 1U);
    ASSERT_EQ(round_trip_config.get_reads()[0]->get_triggers().size(), 2U);
    EXPECT_EQ(round_trip_config.get_reads()[0]->get_triggers()[0], "TimerTrigger");
    EXPECT_EQ(round_trip_config.get_reads()[0]->get_triggers()[1], "ChangeTrigger");
}

TEST(test_data_collection_config, read_references_unknown_trigger_returns_validation_error)
{
    const std::string json_string = R"(
    {
        "configurationName": "BadConfig",
        "triggers": [
            {
                "triggerName": "TimerTrigger",
                "triggerType": "cyclic",
                "frequencyInSeconds": 60
            }
        ],
        "reads": [
            {
                "name": "MotorRead",
                "triggers": ["MissingTrigger"],
                "variables": [
                    {
                        "variableName": "MotorSpeed",
                        "targetAddress": "DB10.DBD0"
                    }
                ]
            }
        ]
    }
    )";

    const auto json_value = util::json::deserialize(json_string);
    ASSERT_TRUE(json_value.is_object());

    const ::business::config::data_collection::data_collection_config config(json_value.as_object());

    const auto errors = config.validate();

    EXPECT_FALSE(errors.empty());
    EXPECT_TRUE(contains_message_fragment(errors, "MotorRead"));
    EXPECT_TRUE(contains_message_fragment(errors, "MissingTrigger"));
}

TEST(test_data_collection_config, validate_collects_multiple_errors_in_one_pass)
{
    const auto first_trigger = std::make_shared<::business::config::data_collection::cyclic_trigger_config>("SharedTrigger", 0);
    const auto second_trigger = std::make_shared<::business::config::data_collection::cyclic_trigger_config>("SharedTrigger", -5);
    const auto invalid_read = std::make_shared<::business::config::data_collection::read_config>(
        "",
        std::vector<::business::config::data_collection::variable_config>{
            ::business::config::data_collection::variable_config("", "")},
        std::vector<std::string>{"", "MissingTrigger"});

    const ::business::config::data_collection::data_collection_config config(
        "",
        {first_trigger, second_trigger},
        {invalid_read});

    const auto errors = config.validate();

    EXPECT_GE(errors.size(), 6U);
    EXPECT_TRUE(contains_message_fragment(errors, "Configuration name must not be empty"));
    EXPECT_TRUE(contains_message_fragment(errors, "positive frequencyInSeconds"));
    EXPECT_TRUE(contains_message_fragment(errors, "duplicate trigger name"));
    EXPECT_TRUE(contains_message_fragment(errors, "Read configuration name must not be empty"));
    EXPECT_TRUE(contains_message_fragment(errors, "empty trigger reference"));
    EXPECT_TRUE(contains_message_fragment(errors, "unknown trigger 'MissingTrigger'"));
}

TEST(test_data_collection_config, unsupported_trigger_type_throws)
{
    const std::string json_string = R"(
    {
        "configurationName": "BadConfig",
        "triggers": [
            {
                "triggerName": "UnknownTrigger",
                "triggerType": "unsupported"
            }
        ],
        "reads": []
    }
    )";

    const auto json_value = util::json::deserialize(json_string);
    ASSERT_TRUE(json_value.is_object());

    EXPECT_THROW(
        {
            ::business::config::data_collection::data_collection_config config(json_value.as_object());
            (void)config;
        },
        util::json::json_exception);
}

} // namespace tests::business::config::data_collection
