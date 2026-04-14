#include "json_deserializer.hpp"
#include "json_array.hpp"
#include "json_object.hpp"
#include <cctype>
#include <stdexcept>

namespace util::json
{

namespace
{

void skip_whitespace(const std::string& text, size_t& index)
{
    while (index < text.size() && std::isspace(static_cast<unsigned char>(text[index])))
    {
        ++index;
    }
}

std::string parse_string(const std::string& text, size_t& index)
{
    if (text[index] != '"')
    {
        throw std::runtime_error("Expected string opening quote");
    }
    ++index;

    std::string result;
    while (index < text.size())
    {
        char ch = text[index++];
        if (ch == '"')
        {
            return result;
        }
        if (ch == '\\')
        {
            if (index >= text.size())
            {
                throw std::runtime_error("Invalid escape sequence");
            }
            char escape = text[index++];
            switch (escape)
            {
                case '"': result += '"'; break;
                case '\\': result += '\\'; break;
                case '/': result += '/'; break;
                case 'b': result += '\b'; break;
                case 'f': result += '\f'; break;
                case 'n': result += '\n'; break;
                case 'r': result += '\r'; break;
                case 't': result += '\t'; break;
                default:
                    throw std::runtime_error("Unsupported JSON escape sequence");
            }
        }
        else
        {
            result += ch;
        }
    }
    throw std::runtime_error("Unterminated JSON string");
}

json_value parse_value(const std::string& text, size_t& index);

json_value parse_number(const std::string& text, size_t& index)
{
    const size_t start = index;
    if (text[index] == '-')
    {
        ++index;
    }
    while (index < text.size() && std::isdigit(static_cast<unsigned char>(text[index])))
    {
        ++index;
    }

    bool has_fraction = false;
    bool has_exponent = false;
    if (index < text.size() && text[index] == '.')
    {
        has_fraction = true;
        ++index;
        while (index < text.size() && std::isdigit(static_cast<unsigned char>(text[index])))
        {
            ++index;
        }
    }
    if (index < text.size() && (text[index] == 'e' || text[index] == 'E'))
    {
        has_exponent = true;
        ++index;
        if (index < text.size() && (text[index] == '+' || text[index] == '-'))
        {
            ++index;
        }
        while (index < text.size() && std::isdigit(static_cast<unsigned char>(text[index])))
        {
            ++index;
        }
    }

    const std::string token = text.substr(start, index - start);
    try
    {
        if (!has_fraction && !has_exponent)
        {
            return json_value(std::stoll(token));
        }
        return json_value(std::stod(token));
    }
    catch (...)
    {
        throw std::runtime_error("Invalid JSON number");
    }
}

json_value parse_array(const std::string& text, size_t& index)
{
    if (text[index] != '[')
    {
        throw std::runtime_error("Expected array opening bracket");
    }
    ++index;
    skip_whitespace(text, index);

    json_array array;
    if (index < text.size() && text[index] == ']')
    {
        ++index;
        return json_value(std::move(array));
    }

    while (true)
    {
        array.push_back(parse_value(text, index));
        skip_whitespace(text, index);
        if (index >= text.size())
        {
            throw std::runtime_error("Unterminated JSON array");
        }
        if (text[index] == ']')
        {
            ++index;
            break;
        }
        if (text[index] != ',')
        {
            throw std::runtime_error("Expected comma in JSON array");
        }
        ++index;
        skip_whitespace(text, index);
    }
    return json_value(std::move(array));
}

json_value parse_object(const std::string& text, size_t& index)
{
    if (text[index] != '{')
    {
        throw std::runtime_error("Expected object opening brace");
    }
    ++index;
    skip_whitespace(text, index);

    json_object object;
    if (index < text.size() && text[index] == '}')
    {
        ++index;
        return json_value(std::move(object));
    }

    while (true)
    {
        skip_whitespace(text, index);
        std::string key = parse_string(text, index);
        skip_whitespace(text, index);
        if (index >= text.size() || text[index] != ':')
        {
            throw std::runtime_error("Expected ':' after object key");
        }
        ++index;
        skip_whitespace(text, index);
        object.emplace(std::move(key), parse_value(text, index));
        skip_whitespace(text, index);
        if (index >= text.size())
        {
            throw std::runtime_error("Unterminated JSON object");
        }
        if (text[index] == '}')
        {
            ++index;
            break;
        }
        if (text[index] != ',')
        {
            throw std::runtime_error("Expected comma in JSON object");
        }
        ++index;
        skip_whitespace(text, index);
    }
    return json_value(std::move(object));
}

json_value parse_value(const std::string& text, size_t& index)
{
    skip_whitespace(text, index);
    if (index >= text.size())
    {
        throw std::runtime_error("Unexpected end of JSON text");
    }

    char ch = text[index];
    if (ch == 'n')
    {
        if (text.compare(index, 4, "null") == 0)
        {
            index += 4;
            return json_value(nullptr);
        }
    }
    else if (ch == 't')
    {
        if (text.compare(index, 4, "true") == 0)
        {
            index += 4;
            return json_value(true);
        }
    }
    else if (ch == 'f')
    {
        if (text.compare(index, 5, "false") == 0)
        {
            index += 5;
            return json_value(false);
        }
    }
    else if (ch == '"')
    {
        return json_value(parse_string(text, index));
    }
    else if (ch == '[')
    {
        return parse_array(text, index);
    }
    else if (ch == '{')
    {
        return parse_object(text, index);
    }
    else if (ch == '-' || std::isdigit(static_cast<unsigned char>(ch)))
    {
        return parse_number(text, index);
    }

    throw std::runtime_error("Invalid JSON value");
}

} // namespace

json_value deserialize(const std::string& json_string)
{
    size_t index = 0;
    auto result = parse_value(json_string, index);
    skip_whitespace(json_string, index);
    if (index != json_string.size())
    {
        throw std::runtime_error("Extra characters in JSON text");
    }
    return result;
}

} // namespace util::json
