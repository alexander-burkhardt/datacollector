#include <gtest/gtest.h>

#include <memory>
#include <type_traits>
#include <vector>

#include "business/config/configuration_registry.hpp"
#include "business/config/data_collection/cyclic_trigger.hpp"
#include "business/config/data_collection/data_collection_config.hpp"
#include "business/config/data_collection/read_config.hpp"
#include "business/config/data_collection/variable_config.hpp"

namespace tests::business::config
{

TEST(test_configuration_registry, configuration_types_are_immutable)
{
    static_assert(!std::is_copy_assignable_v<::business::config::data_collection::data_collection_config>);
    static_assert(!std::is_move_assignable_v<::business::config::data_collection::data_collection_config>);
    static_assert(!std::is_copy_assignable_v<::business::config::data_collection::read_config>);
    static_assert(!std::is_move_assignable_v<::business::config::data_collection::read_config>);
    static_assert(!std::is_copy_assignable_v<::business::config::data_collection::variable_config>);
    static_assert(!std::is_move_assignable_v<::business::config::data_collection::variable_config>);
    static_assert(!std::is_copy_assignable_v<::business::config::data_collection::cyclic_trigger_config>);
    static_assert(!std::is_move_assignable_v<::business::config::data_collection::cyclic_trigger_config>);

    SUCCEED();
}

TEST(test_configuration_registry, replacement_keeps_existing_snapshot_alive)
{
    auto& registry = ::business::config::configuration_registry::instance();

    const auto firstConfig = std::make_shared<const ::business::config::data_collection::data_collection_config>(
        "FirstConfig",
        std::vector<std::shared_ptr<const ::business::config::data_collection::trigger_config>>{
            std::make_shared<::business::config::data_collection::cyclic_trigger_config>("TimerA", 5)});

    const auto firstErrors = registry.set_data_collection_config(firstConfig);
    ASSERT_TRUE(firstErrors.empty());

    const auto snapshot = registry.get_data_collection_config();
    ASSERT_NE(snapshot, nullptr);
    EXPECT_EQ(snapshot->get_configuration_name(), "FirstConfig");

    const auto secondConfig = std::make_shared<const ::business::config::data_collection::data_collection_config>(
        "SecondConfig",
        std::vector<std::shared_ptr<const ::business::config::data_collection::trigger_config>>{
            std::make_shared<::business::config::data_collection::cyclic_trigger_config>("TimerB", 10)});

    const auto secondErrors = registry.set_data_collection_config(secondConfig);
    ASSERT_TRUE(secondErrors.empty());

    const auto current = registry.get_data_collection_config();
    ASSERT_NE(current, nullptr);
    EXPECT_EQ(current->get_configuration_name(), "SecondConfig");
    EXPECT_EQ(snapshot->get_configuration_name(), "FirstConfig");
    EXPECT_NE(snapshot, current);
}

TEST(test_configuration_registry, invalid_configuration_is_rejected_and_previous_one_stays_active)
{
    auto& registry = ::business::config::configuration_registry::instance();

    const auto validConfig = std::make_shared<const ::business::config::data_collection::data_collection_config>("StableConfig");
    ASSERT_TRUE(registry.set_data_collection_config(validConfig).empty());

    const auto invalidConfig = std::make_shared<const ::business::config::data_collection::data_collection_config>(
        "",
        std::vector<std::shared_ptr<const ::business::config::data_collection::trigger_config>>{
            std::make_shared<::business::config::data_collection::cyclic_trigger_config>("BrokenTimer", 0)});

    const auto errors = registry.set_data_collection_config(invalidConfig);

    EXPECT_FALSE(errors.empty());
    const auto current = registry.get_data_collection_config();
    ASSERT_NE(current, nullptr);
    EXPECT_EQ(current->get_configuration_name(), "StableConfig");
}

} // namespace tests::business::config
