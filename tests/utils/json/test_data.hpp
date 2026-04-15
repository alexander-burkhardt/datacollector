#ifndef TESTS_UTILS_JSON_TEST_DATA_HPP
#define TESTS_UTILS_JSON_TEST_DATA_HPP

#include <cstdint>
#include <string>
#include <vector>
#include "utils/json/json_object.hpp"
#include "utils/json/json_writer.hpp"

namespace tests::utils::json {

class address {
private:
    std::string _street;
    std::string _city;
    std::int64_t _zip_code;

public:
    address() = default;
    address(const util::json::json_object& obj):
        _street(obj.get_string_or_default("street")),
        _city(obj.get_string_or_default("city")),
        _zip_code(obj.get_int_or_default("zipCode"))
    {}
    address(const address&) = default;
    address(address&&) noexcept = default;
    address& operator=(const address&) = default;
    address& operator=(address&&) noexcept = default;

public:
    const std::string& get_street() const { return _street; }
    const std::string& get_city() const { return _city; }
    std::int64_t get_zip_code() const { return _zip_code; }
    std::string to_json() const {
        util::json::json_writer writer;
        write_json(writer);
        return writer.get_json();
    }

    void write_json(util::json::json_writer& writer) const {
        writer.start_object();
            writer.write_property("street", _street);
            writer.write_property("city", _city);
            writer.write_property("zipCode", _zip_code);
        writer.end_object();
    }
};

class person {
private:
    std::string _name;
    std::int64_t _age;
    double _height;
    std::vector<std::string> _hobbies;
    address _address;

public:
    person() = default;
    person(const util::json::json_object& obj):
        _name(obj.get_string_or_default("name")),
        _age(obj.get_int_or_default("age")),
        _height(obj.get_double_or_default("height")),
        _hobbies(obj.get_array_property<std::string>("hobbies", [](const util::json::json_value& v) { return v.as_string(); })),
        _address(obj.get_object_or_default("address"))
    {}
    person(const person&) = default;
    person(person&&) noexcept = default;
    person& operator=(const person&) = default;
    person& operator=(person&&) noexcept = default;

public:
    const std::string& get_name() const { return _name; }
    std::int64_t get_age() const { return _age; }
    double get_height() const { return _height; }
    const std::vector<std::string>& get_hobbies() const { return _hobbies; }
    const address& get_address() const { return _address; }

    std::string to_json() const {
        util::json::json_writer writer;
        write_json(writer);
        return writer.get_json();
    }

    void write_json(util::json::json_writer& writer) const {
        writer.start_object();
            writer.write_property("name", _name);
            writer.write_property("age", _age);
            writer.write_property("height", _height);
            writer.write_property_name("hobbies");
            writer.start_array();
                for (const auto& hobby : _hobbies) {
                    writer.write_value(hobby);
                }
            writer.end_array();
            writer.write_object_property("address", [this](util::json::json_writer& w) {
                _address.write_json(w);
            });
        writer.end_object();
    }
};

} // namespace tests::utils::json

#endif // TESTS_UTILS_JSON_TEST_DATA_HPP