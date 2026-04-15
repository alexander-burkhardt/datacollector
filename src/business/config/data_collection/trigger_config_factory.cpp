#include "trigger_config_factory.hpp"

#include "cyclic_trigger.hpp"
#include "onchange_trigger_config.hpp"
#include "utils/json/json_exception.hpp"

namespace business::config::data_collection
{

std::shared_ptr<const trigger_config> trigger_config_factory::create(const util::json::json_object& obj)
{
    const std::string triggerType = obj.get_string("triggerType");

    if (triggerType == cyclic_trigger_config::type_name())
    {
        return std::make_shared<cyclic_trigger_config>(obj);
    }
    if (triggerType == onchange_trigger_config::type_name())
    {
        return std::make_shared<onchange_trigger_config>(obj);
    }

    throw util::json::json_exception("Unsupported trigger type '" + triggerType + "'");
}

} // namespace business::config::data_collection
