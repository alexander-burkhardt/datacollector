#include "json_writer.hpp"

#include "json_exception.hpp"

#include <iomanip>
#include <limits>
#include <sstream>

namespace util::json
{

void json_writer::start_object()
{
    before_value();
    json_ += "{";
    stack_.push_back({ scope_type::object, true, false });
}

void json_writer::end_object()
{
    if (stack_.empty() || stack_.back().type != scope_type::object)
    {
        throw json_exception("JSON writer is not currently inside an object");
    }
    if (stack_.back().expecting_value)
    {
        throw json_exception("JSON writer expected a value for the current property");
    }

    stack_.pop_back();
    json_ += "}";
}

void json_writer::start_array()
{
    before_value();
    json_ += "[";
    stack_.push_back({ scope_type::array, true, false });
}

void json_writer::end_array()
{
    if (stack_.empty() || stack_.back().type != scope_type::array)
    {
        throw json_exception("JSON writer is not currently inside an array");
    }

    stack_.pop_back();
    json_ += "]";
}

void json_writer::write_property_name(const std::string& name)
{
    if (stack_.empty() || stack_.back().type != scope_type::object)
    {
        throw json_exception("JSON property names can only be written inside an object");
    }

    auto& state = stack_.back();
    if (state.expecting_value)
    {
        throw json_exception("JSON writer expected a property value before another property name");
    }

    if (!state.first_entry)
    {
        json_ += ",";
    }
    state.first_entry = false;

    write_escaped_string(name);
    json_ += ":";
    state.expecting_value = true;
}

void json_writer::write_property(const std::string& name, const std::string& value)
{
    write_property_name(name);
    write_value(value);
}

void json_writer::write_property(const std::string& name, std::int64_t value)
{
    write_property_name(name);
    write_value(value);
}

void json_writer::write_property(const std::string& name, double value)
{
    write_property_name(name);
    write_value(value);
}

void json_writer::write_property(const std::string& name, bool value)
{
    write_property_name(name);
    write_value(value);
}

void json_writer::write_value(const std::string& value)
{
    before_value();
    write_escaped_string(value);
}

void json_writer::write_value(std::int64_t value)
{
    before_value();
    json_ += std::to_string(value);
}

void json_writer::write_value(double value)
{
    before_value();

    std::ostringstream out;
    out << std::setprecision(std::numeric_limits<double>::digits10 + 1) << value;
    json_ += out.str();
}

void json_writer::write_value(bool value)
{
    before_value();
    json_ += value ? "true" : "false";
}

std::string json_writer::get_json() const
{
    return json_;
}

void json_writer::before_value()
{
    if (stack_.empty())
    {
        if (!json_.empty())
        {
            throw json_exception("JSON writer only supports one top-level value");
        }
        return;
    }

    auto& state = stack_.back();
    if (state.type == scope_type::object)
    {
        if (!state.expecting_value)
        {
            throw json_exception("JSON writer expected a property name before the value");
        }

        state.expecting_value = false;
        return;
    }

    if (!state.first_entry)
    {
        json_ += ",";
    }
    state.first_entry = false;
}

void json_writer::write_escaped_string(const std::string& value)
{
    json_ += '"';

    for (char ch : value)
    {
        switch (ch)
        {
            case '"': json_ += "\\\""; break;
            case '\\': json_ += "\\\\"; break;
            case '\b': json_ += "\\b"; break;
            case '\f': json_ += "\\f"; break;
            case '\n': json_ += "\\n"; break;
            case '\r': json_ += "\\r"; break;
            case '\t': json_ += "\\t"; break;
            default:
                if (static_cast<unsigned char>(ch) < 0x20)
                {
                    std::ostringstream out;
                    out << "\\u"
                        << std::hex
                        << std::setw(4)
                        << std::setfill('0')
                        << static_cast<std::uint32_t>(static_cast<unsigned char>(ch));
                    json_ += out.str();
                }
                else
                {
                    json_ += ch;
                }
                break;
        }
    }

    json_ += '"';
}

} // namespace util::json
