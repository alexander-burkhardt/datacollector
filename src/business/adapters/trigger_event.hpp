#ifndef BUSINESS_ADAPTERS_TRIGGER_EVENT_HPP
#define BUSINESS_ADAPTERS_TRIGGER_EVENT_HPP

#include <string>
#include <vector>

#include "utils/json/json_object.hpp"
#include "utils/json/json_writer.hpp"

namespace business::adapters
{

class trigger_event
{
private:
    const std::string _triggerName;
    const std::string _triggerType;
    const std::string _timestamp;
    const std::string _triggeringAddress;
    const std::string _triggeringValueJson;

public:
    trigger_event(
        const std::string& triggerName,
        const std::string& triggerType,
        const std::string& timestamp,
        const std::string& triggeringAddress,
        const std::string& triggeringValueJson);
    explicit trigger_event(const util::json::json_object& obj);

    trigger_event(const trigger_event&) = default;
    ~trigger_event() = default;
    trigger_event& operator=(const trigger_event&) = delete;
    trigger_event(trigger_event&&) = default;
    trigger_event& operator=(trigger_event&&) = delete;

    const std::string& get_trigger_name() const;
    const std::string& get_trigger_type() const;
    const std::string& get_timestamp() const;
    const std::string& get_triggering_address() const;
    const std::string& get_triggering_value_json() const;

    std::vector<std::string> validate() const;

    std::string to_json() const;
    void write_json(util::json::json_writer& writer) const;
};

} // namespace business::adapters

#endif // BUSINESS_ADAPTERS_TRIGGER_EVENT_HPP
