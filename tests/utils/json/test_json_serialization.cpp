#include <gtest/gtest.h>

#include "utils/json/json_deserializer.hpp"
#include "utils/json/json_exception.hpp"
#include "utils/json/json_writer.hpp"
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

TEST(test_json_serialization, property_writing_outside_object_throws)
{
    util::json::json_writer writer;

    try
    {
        writer.write_property("name", "John Doe");
        FAIL() << "Expected util::json::json_exception";
    }
    catch (const util::json::json_exception& ex)
    {
        const std::string message = ex.what();
        EXPECT_NE(message.find("name"), std::string::npos);
        EXPECT_NE(message.find("object"), std::string::npos);
    }
}

TEST(test_json_serialization, value_writing_inside_object_without_property_throws)
{
    util::json::json_writer writer;
    writer.start_object();

    try
    {
        writer.write_value("John Doe");
        FAIL() << "Expected util::json::json_exception";
    }
    catch (const util::json::json_exception& ex)
    {
        const std::string message = ex.what();
        EXPECT_NE(message.find("property name"), std::string::npos);
    }
}

TEST(test_json_serialization, ending_wrong_or_missing_scope_throws)
{
    util::json::json_writer writer;

    EXPECT_THROW(writer.end_object(), util::json::json_exception);

    writer.start_object();
    writer.write_property_name("address");
    try
    {
        writer.end_object();
        FAIL() << "Expected util::json::json_exception";
    }
    catch (const util::json::json_exception& ex)
    {
        const std::string message = ex.what();
        EXPECT_NE(message.find("address"), std::string::npos);
        EXPECT_NE(message.find("value"), std::string::npos);
    }
}

TEST(test_json_serialization, nested_object_or_array_inside_object_requires_property_name)
{
    util::json::json_writer writer_for_object;
    writer_for_object.start_object();
    EXPECT_THROW(writer_for_object.start_object(), util::json::json_exception);

    util::json::json_writer writer_for_array;
    writer_for_array.start_object();
    EXPECT_THROW(writer_for_array.start_array(), util::json::json_exception);
}

} // namespace tests::utils::json
