#include "configuration_registry.hpp"

#include <utility>

namespace business::config
{

configuration_registry& configuration_registry::instance()
{
    static configuration_registry registry;
    return registry;
}

std::shared_ptr<const data_collection::data_collection_config> configuration_registry::get_data_collection_config() const
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _dataCollectionConfiguration;
}

std::vector<std::string> configuration_registry::set_data_collection_config(
    std::shared_ptr<const data_collection::data_collection_config> configuration)
{
    if (!configuration)
    {
        return {"Data collection configuration must not be null."};
    }

    const auto errors = configuration->validate();
    if (!errors.empty())
    {
        return errors;
    }

    std::shared_ptr<const data_collection::data_collection_config> previousConfiguration;
    {
        std::lock_guard<std::mutex> lock(_mutex);

        // Keep the previous instance alive until after the lock is released so
        // its destructor is never triggered from inside the critical section.
        previousConfiguration = _dataCollectionConfiguration;
        _dataCollectionConfiguration = std::move(configuration);
    }

    return {};
}

} // namespace business::config
