#include <gtest/gtest.h>

#include "utils/json/json_deserializer.hpp"
#include "test_data.hpp"

namespace tests::utils::json
{

TEST(test_json_serialization, serialize_address_round_trip)
{
    const std::string json_string = R"(
    {
        "street": "123 Main St",
        "city": "Anytown",
        "zipCode": 12345
    }
    )";

    const auto json_value = util::json::deserialize(json_string);
    ASSERT_TRUE(json_value.is_object());

    const address a(json_value.as_object());
    const std::string serialized = a.to_json();

    const auto serialized_value = util::json::deserialize(serialized);
    ASSERT_TRUE(serialized_value.is_object());

    const auto& obj = serialized_value.as_object();
    EXPECT_EQ(obj.get_string("street"), "123 Main St");
    EXPECT_EQ(obj.get_string("city"), "Anytown");
    EXPECT_EQ(obj.get_int("zipCode"), 12345);
}

TEST(test_json_serialization, serialize_person_round_trip)
{
    const std::string json_string = R"(
    {
        "name": "John Doe",
        "age": 30,
        "height": 175.5,
        "hobbies": ["reading", "cycling", "coding"],
        "address": {
            "street": "123 Main St",
            "city": "Anytown",
            "zipCode": 12345
        }
    }
    )";

    const auto json_value = util::json::deserialize(json_string);
    ASSERT_TRUE(json_value.is_object());

    const person p(json_value.as_object());
    const std::string serialized = p.to_json();

    const auto serialized_value = util::json::deserialize(serialized);
    ASSERT_TRUE(serialized_value.is_object());

    const auto& obj = serialized_value.as_object();
    EXPECT_EQ(obj.get_string("name"), "John Doe");
    EXPECT_EQ(obj.get_int("age"), 30);
    EXPECT_DOUBLE_EQ(obj.get_double("height"), 175.5);

    const auto hobbies = obj.get_array_property<std::string>(
        "hobbies",
        [](const util::json::json_value& value)
        {
            return value.as_string();
        });

    ASSERT_EQ(hobbies.size(), 3U);
    EXPECT_EQ(hobbies[0], "reading");
    EXPECT_EQ(hobbies[1], "cycling");
    EXPECT_EQ(hobbies[2], "coding");

    const auto address_object = obj.get_object("address");
    EXPECT_EQ(address_object.get_string("street"), "123 Main St");
    EXPECT_EQ(address_object.get_string("city"), "Anytown");
    EXPECT_EQ(address_object.get_int("zipCode"), 12345);
}

} // namespace tests::utils::json
