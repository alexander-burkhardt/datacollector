#ifndef BUSINESS_ADAPTERS_QUALITY_CODE_HPP
#define BUSINESS_ADAPTERS_QUALITY_CODE_HPP

#include <cstdint>
#include <optional>
#include <string>

namespace business::adapters
{

enum class quality_code : std::uint32_t
{
    good = 0x00000000U,
    uncertain = 0x40000000U,
    bad = 0x80000000U,
    bad_timeout = 0x800A0000U,
    bad_no_communication = 0x80310000U,
    bad_waiting_for_initial_data = 0x80320000U,
    uncertain_last_usable_value = 0x40900000U,
    uncertain_substitute_value = 0x40910000U
};

bool is_known_quality_code(quality_code code);
std::string to_string(quality_code code);
std::optional<quality_code> quality_code_from_string(const std::string& value);

} // namespace business::adapters

#endif // BUSINESS_ADAPTERS_QUALITY_CODE_HPP
