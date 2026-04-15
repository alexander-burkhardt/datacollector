#include <gtest/gtest.h>
#include "utils/json/json_deserializer.hpp"
#include "utils/json/json_object.hpp"
#include "test_data.hpp"

namespace tests::utils::json {

class test_json_deserialization : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup if needed
    }

    void TearDown() override {
        // Cleanup if needed
    }
};

TEST_F(test_json_deserialization, deserialize_person) {
    std::string json_string = R"(
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

    auto json_value = util::json::deserialize(json_string);
    ASSERT_TRUE(json_value.is_object());

    auto json_object = json_value.as_object();

    person p(json_object);

    EXPECT_EQ(p.get_name(), "John Doe");
    EXPECT_EQ(p.get_age(), 30);
    EXPECT_DOUBLE_EQ(p.get_height(), 175.5);
    ASSERT_EQ(p.get_hobbies().size(), 3);
    EXPECT_EQ(p.get_hobbies()[0], "reading");
    EXPECT_EQ(p.get_hobbies()[1], "cycling");
    EXPECT_EQ(p.get_hobbies()[2], "coding");
    EXPECT_EQ(p.get_address().get_street(), "123 Main St");
    EXPECT_EQ(p.get_address().get_city(), "Anytown");
    EXPECT_EQ(p.get_address().get_zip_code(), 12345);
}

TEST_F(test_json_deserialization, missing_properties_use_defaults) {
    std::string json_string = R"(
    {
        "name": "Jane Doe"
    }
    )";

    auto json_value = util::json::deserialize(json_string);
    ASSERT_TRUE(json_value.is_object());

    person p(json_value.as_object());

    EXPECT_EQ(p.get_name(), "Jane Doe");
    EXPECT_EQ(p.get_age(), 0);
    EXPECT_DOUBLE_EQ(p.get_height(), 0.0);
    EXPECT_TRUE(p.get_hobbies().empty());
    EXPECT_EQ(p.get_address().get_street(), "");
    EXPECT_EQ(p.get_address().get_city(), "");
    EXPECT_EQ(p.get_address().get_zip_code(), 0);
}

TEST_F(test_json_deserialization, wrong_property_type_throws) {
    std::string json_string = R"(
    {
        "name": "John Doe",
        "age": "thirty",
        "height": 175.5,
        "hobbies": ["reading"],
        "address": {
            "street": "123 Main St",
            "city": "Anytown",
            "zipCode": 12345
        }
    }
    )";

    auto json_value = util::json::deserialize(json_string);
    ASSERT_TRUE(json_value.is_object());

    EXPECT_THROW({
        person p(json_value.as_object());
        (void)p;
    }, util::json::json_exception);
}

TEST_F(test_json_deserialization, wrong_property_type_exception_message_contains_details) {
    std::string json_string = R"(
    {
        "name": "John Doe",
        "age": "thirty"
    }
    )";

    auto json_value = util::json::deserialize(json_string);
    ASSERT_TRUE(json_value.is_object());

    try
    {
        person p(json_value.as_object());
        (void)p;
        FAIL() << "Expected util::json::json_exception";
    }
    catch (const util::json::json_exception& ex)
    {
        const std::string message = ex.what();
        EXPECT_NE(message.find("age"), std::string::npos);
        EXPECT_NE(message.find("integer"), std::string::npos);
        EXPECT_NE(message.find("string"), std::string::npos);
    }
}

} // namespace tests::utils::json