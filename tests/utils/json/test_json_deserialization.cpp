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

} // namespace tests::utils::json