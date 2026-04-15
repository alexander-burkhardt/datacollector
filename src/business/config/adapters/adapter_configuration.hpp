#ifndef BUSINESS_CONFIG_ADAPTERS_ADAPTER_CONFIGURATION_HPP
#define BUSINESS_CONFIG_ADAPTERS_ADAPTER_CONFIGURATION_HPP

#include <string>
#include <vector>

#include "utils/json/json_object.hpp"
#include "utils/json/json_writer.hpp"

namespace business::config::adapters
{

class adapter_configuration
{
private:
    const std::string _adapterName;
    const std::string _adapterType;
    const std::string _adapterConfigurationJson;

public:
    adapter_configuration(
        const std::string& adapterName,
        const std::string& adapterType,
        const std::string& adapterConfigurationJson = "{}");
    explicit adapter_configuration(const util::json::json_object& obj);

    adapter_configuration(const adapter_configuration&) = default;
    ~adapter_configuration() = default;
    adapter_configuration& operator=(const adapter_configuration&) = delete;
    adapter_configuration(adapter_configuration&&) = default;
    adapter_configuration& operator=(adapter_configuration&&) = delete;

    const std::string& get_adapter_name() const;
    const std::string& get_adapter_type() const;
    const std::string& get_adapter_configuration_json() const;

    std::vector<std::string> validate() const;

    std::string to_json() const;
    void write_json(util::json::json_writer& writer) const;
};

} // namespace business::config::adapters

#endif // BUSINESS_CONFIG_ADAPTERS_ADAPTER_CONFIGURATION_HPP
