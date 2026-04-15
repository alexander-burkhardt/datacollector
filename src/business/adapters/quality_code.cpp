#include "quality_code.hpp"

namespace business::adapters
{

bool is_known_quality_code(quality_code code)
{
    switch (code)
    {
    case quality_code::good:
    case quality_code::uncertain:
    case quality_code::bad:
    case quality_code::bad_timeout:
    case quality_code::bad_no_communication:
    case quality_code::bad_waiting_for_initial_data:
    case quality_code::uncertain_last_usable_value:
    case quality_code::uncertain_substitute_value:
        return true;
    default:
        return false;
    }
}

std::string to_string(quality_code code)
{
    switch (code)
    {
    case quality_code::good:
        return "good";
    case quality_code::uncertain:
        return "uncertain";
    case quality_code::bad:
        return "bad";
    case quality_code::bad_timeout:
        return "bad_timeout";
    case quality_code::bad_no_communication:
        return "bad_no_communication";
    case quality_code::bad_waiting_for_initial_data:
        return "bad_waiting_for_initial_data";
    case quality_code::uncertain_last_usable_value:
        return "uncertain_last_usable_value";
    case quality_code::uncertain_substitute_value:
        return "uncertain_substitute_value";
    default:
        return "unknown";
    }
}

std::optional<quality_code> quality_code_from_string(const std::string& value)
{
    if (value == "good")
    {
        return quality_code::good;
    }
    if (value == "uncertain")
    {
        return quality_code::uncertain;
    }
    if (value == "bad")
    {
        return quality_code::bad;
    }
    if (value == "bad_timeout")
    {
        return quality_code::bad_timeout;
    }
    if (value == "bad_no_communication")
    {
        return quality_code::bad_no_communication;
    }
    if (value == "bad_waiting_for_initial_data")
    {
        return quality_code::bad_waiting_for_initial_data;
    }
    if (value == "uncertain_last_usable_value")
    {
        return quality_code::uncertain_last_usable_value;
    }
    if (value == "uncertain_substitute_value")
    {
        return quality_code::uncertain_substitute_value;
    }

    return std::nullopt;
}

} // namespace business::adapters
