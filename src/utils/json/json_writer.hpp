#ifndef JSON_WRITER_HPP
#define JSON_WRITER_HPP

#include <cstdint>
#include <string>
#include <vector>

namespace util::json
{

class json_writer
{
public:
    json_writer() = default;
    ~json_writer() = default;

    void start_object();
    void end_object();
    void start_array();
    void end_array();

    void write_property_name(const std::string& name);

    template <typename Callback>
    void write_object_property(const std::string& name, Callback&& callback)
    {
        write_property_name(name);
        callback(*this);
    }

    template <typename Callback>
    void write_array_property(const std::string& name, Callback&& callback)
    {
        write_property_name(name);
        start_array();
        callback(*this);
        end_array();
    }

    void write_property(const std::string& name, const std::string& value);
    void write_property(const std::string& name, std::int64_t value);
    void write_property(const std::string& name, double value);
    void write_property(const std::string& name, bool value);

    void write_value(const std::string& value);
    void write_value(std::int64_t value);
    void write_value(double value);
    void write_value(bool value);

    std::string get_json() const;

private:
    enum class scope_type
    {
        object,
        array
    };

    struct scope_state
    {
        scope_type type;
        bool first_entry;
        bool expecting_value;
        std::string current_property_name;
    };

    std::string json_;
    std::vector<scope_state> stack_;

    void before_value();
    std::string current_property_context() const;
    void write_escaped_string(const std::string& value);
};

} // namespace util::json

#endif // JSON_WRITER_HPP