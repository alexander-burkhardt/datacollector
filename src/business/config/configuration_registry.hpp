#ifndef BUSINESS_CONFIG_CONFIGURATION_REGISTRY_HPP
#define BUSINESS_CONFIG_CONFIGURATION_REGISTRY_HPP

#include <memory>
#include <mutex>
#include <vector>

#include "business/config/data_collection/data_collection_config.hpp"

namespace business::config
{

class configuration_registry
{
public:
    static configuration_registry& instance();

    configuration_registry(const configuration_registry&) = delete;
    configuration_registry& operator=(const configuration_registry&) = delete;
    configuration_registry(configuration_registry&&) = delete;
    configuration_registry& operator=(configuration_registry&&) = delete;

    std::shared_ptr<const data_collection::data_collection_config> get_data_collection_config() const;
    std::vector<std::string> set_data_collection_config(
        std::shared_ptr<const data_collection::data_collection_config> configuration);

private:
    configuration_registry() = default;
    ~configuration_registry() = default;

    mutable std::mutex _mutex;
    std::shared_ptr<const data_collection::data_collection_config> _dataCollectionConfiguration;
};

} // namespace business::config

#endif // BUSINESS_CONFIG_CONFIGURATION_REGISTRY_HPP
