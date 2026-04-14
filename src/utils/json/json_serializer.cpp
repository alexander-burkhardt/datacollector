#include "json_serializer.hpp"
#include "json_array.hpp"
#include "json_object.hpp"
#include <iomanip>
#include <limits>
#include <sstream>

namespace util::json
{

namespace
{

std::string escape_string(const std::string& value)
{
    std::ostringstream out;
    for (char ch : value)
    {
        switch (ch)
        {
            case '"': out << "\\\""; break;
            case '\\': out << "\\\\"; break;
            case '\b': out << "\\b"; break;
            case '\f': out << "\\f"; break;
            case '\n': out << "\\n"; break;
            case '\r': out << "\\r"; break;
            case '\t': out << "\\t"; break;
            default:
                if (static_cast<unsigned char>(ch) < 0x20)
                {
                    out << "\\u" << std::hex << std::setw(4) << std::setfill('0')
                        << static_cast<int>(static_cast<unsigned char>(ch)) << std::dec;
                }
                else
                {
                    out << ch;
                }
                break;
        }
    }
    return out.str();
}

std::string serialize_value(const json_value& value, int indent_level, bool pretty);

std::string serialize_object(const json_object& object, int indent_level, bool pretty)
{
    if (object.empty())
    {
        return "{}";
    }

    std::ostringstream out;
    out << "{";
    if (pretty)
    {
        out << "\n";
    }

    bool first = true;
    for (const auto& [key, entry] : object)
    {
        if (!first)
        {
            out << ",";
            if (pretty)
            {
                out << "\n";
            }
        }
        first = false;
        if (pretty)
        {
            out << std::string((indent_level + 1) * 2, ' ');
        }
        out << '"' << escape_string(key) << "\": ";
        out << serialize_value(entry, indent_level + 1, pretty);
    }

    if (pretty)
    {
        out << "\n" << std::string(indent_level * 2, ' ');
    }
    out << "}";
    return out.str();
}

std::string serialize_array(const json_array& array, int indent_level, bool pretty)
{
    if (array.empty())
    {
        return "[]";
    }

    std::ostringstream out;
    out << "[";
    if (pretty)
    {
        out << "\n";
    }

    bool first = true;
    for (const auto& item : array)
    {
        if (!first)
        {
            out << ",";
            if (pretty)
            {
                out << "\n";
            }
        }
        first = false;
        if (pretty)
        {
            out << std::string((indent_level + 1) * 2, ' ');
        }
        out << serialize_value(item, indent_level + 1, pretty);
    }

    if (pretty)
    {
        out << "\n" << std::string(indent_level * 2, ' ');
    }
    out << "]";
    return out.str();
}

std::string serialize_value(const json_value& value, int indent_level, bool pretty)
{
    switch (value.value.index())
    {
        case 0:
            return "null";
        case 1:
            return value.as_bool() ? "true" : "false";
        case 2:
        {
            std::ostringstream out;
            out << value.as_int();
            return out.str();
        }
        case 3:
        {
            std::ostringstream out;
            out << std::setprecision(std::numeric_limits<double>::digits10 + 1) << value.as_double();
            return out.str();
        }
        case 4:
            return '"' + escape_string(value.as_string()) + '"';
        case 5:
            return serialize_array(value.as_array(), indent_level, pretty);
        case 6:
            return serialize_object(value.as_object(), indent_level, pretty);
        default:
            return "null";
    }
}

} // namespace

std::string serialize(const json_value& value, bool pretty)
{
    return serialize_value(value, 0, pretty);
}

} // namespace util::json
