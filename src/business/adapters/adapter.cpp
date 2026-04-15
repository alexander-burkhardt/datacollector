#include "adapter.hpp"

namespace business::adapters
{

void adapter_event_sink::on_trigger(const trigger_event& event)
{
    (void)event;
}

void adapter_event_sink::on_read_result(const read_result& result)
{
    (void)result;
}

void adapter_event_sink::on_adapter_error(const std::string& adapterName, const std::string& errorMessage)
{
    (void)adapterName;
    (void)errorMessage;
}

} // namespace business::adapters
