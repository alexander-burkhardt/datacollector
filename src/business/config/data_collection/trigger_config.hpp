#ifndef BUSINESS_CONFIG_DATA_COLLECTION_TRIGGER_CONFIG_HPP
#define BUSINESS_CONFIG_DATA_COLLECTION_TRIGGER_CONFIG_HPP

#include <string>
#include <vector>

#include "utils/json/json_object.hpp"
#include "utils/json/json_writer.hpp"

namespace business::config::data_collection
{

class trigger_config
{
private:
    std::string _triggerName;
    std::string _triggerType;

protected:
    void write_json_properties(util::json::json_writer& writer) const;

public:
    trigger_config(const std::string& triggerName, const std::string& triggerType);
    explicit trigger_config(const util::json::json_object& obj);

    trigger_config(const trigger_config&) = default;
    virtual ~trigger_config() = default;
    trigger_config& operator=(const trigger_config&) = default;
    trigger_config(trigger_config&&) = default;
    trigger_config& operator=(trigger_config&&) = default;

    const std::string& get_trigger_name() const;
    const std::string& get_trigger_type() const;

    virtual std::vector<std::string> validate() const;

    std::string to_json() const;

    virtual void write_json(util::json::json_writer& writer) const = 0;
};

} // namespace business::config::data_collection

#endif // BUSINESS_CONFIG_DATA_COLLECTION_TRIGGER_CONFIG_HPP