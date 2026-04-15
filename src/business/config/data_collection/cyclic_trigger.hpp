#ifndef BUSINESS_CONFIG_DATA_COLLECTION_CYCLIC_TRIGGER_HPP
#define BUSINESS_CONFIG_DATA_COLLECTION_CYCLIC_TRIGGER_HPP

#include <cstdint>
#include <string>

#include "trigger_config.hpp"

namespace business::config::data_collection
{

class cyclic_trigger_config : public trigger_config
{
private:
    std::int32_t _frequencyInSeconds;

public:
    cyclic_trigger_config(const std::string& triggerName, std::int32_t frequencyInSeconds);
    explicit cyclic_trigger_config(const util::json::json_object& obj);

    cyclic_trigger_config(const cyclic_trigger_config&) = default;
    ~cyclic_trigger_config() override = default;
    cyclic_trigger_config& operator=(const cyclic_trigger_config&) = default;
    cyclic_trigger_config(cyclic_trigger_config&&) = default;
    cyclic_trigger_config& operator=(cyclic_trigger_config&&) = default;

    static std::string type_name();

    std::int32_t get_frequency_in_seconds() const;

    void write_json(util::json::json_writer& writer) const override;
};

} // namespace business::config::data_collection

#endif // BUSINESS_CONFIG_DATA_COLLECTION_CYCLIC_TRIGGER_HPP
