#ifndef UTIL_JSON_JSON_EXCEPTION_HPP
#define UTIL_JSON_JSON_EXCEPTION_HPP

#include <stdexcept>
#include <string>

namespace util::json
{

class json_exception : public std::runtime_error
{
public:
    explicit json_exception(const std::string& message)
        : std::runtime_error(message)
    {
    }
};

} // namespace util::json

#endif // UTIL_JSON_JSON_EXCEPTION_HPP
