#ifndef BUSINESS_CONFIG_DATA_COLLECTION_TRIGGER_CONFIG_FACTORY_HPP
#define BUSINESS_CONFIG_DATA_COLLECTION_TRIGGER_CONFIG_FACTORY_HPP

#include <memory>

#include "trigger_config.hpp"

namespace business::config::data_collection
{

class trigger_config_factory
{
public:
    static std::shared_ptr<trigger_config> create(const util::json::json_object& obj);
};

} // namespace business::config::data_collection

#endif // BUSINESS_CONFIG_DATA_COLLECTION_TRIGGER_CONFIG_FACTORY_HPP
