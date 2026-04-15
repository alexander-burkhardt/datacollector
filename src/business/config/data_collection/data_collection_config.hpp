#ifndef BUSINESS_CONFIG_DATA_COLLECTION_DATA_COLLECTION_CONFIG_HPP
#define BUSINESS_CONFIG_DATA_COLLECTION_DATA_COLLECTION_CONFIG_HPP

#include <memory>
#include <string>
#include <vector>

#include "read_config.hpp"
#include "trigger_config.hpp"
#include "utils/json/json_object.hpp"
#include "utils/json/json_writer.hpp"

namespace business::config::data_collection
{

class data_collection_config
{
private:
    std::string _configurationName;
    std::vector<std::shared_ptr<trigger_config>> _triggers;
    std::vector<std::shared_ptr<read_config>> _reads;

public:
    data_collection_config(
        const std::string& configurationName,
        const std::vector<std::shared_ptr<trigger_config>>& triggers = {},
        const std::vector<std::shared_ptr<read_config>>& reads = {});

    explicit data_collection_config(const util::json::json_object& obj);

    data_collection_config(const data_collection_config&) = default;
    ~data_collection_config() = default;
    data_collection_config& operator=(const data_collection_config&) = default;
    data_collection_config(data_collection_config&&) = default;
    data_collection_config& operator=(data_collection_config&&) = default;

    const std::string& get_configuration_name() const;
    const std::vector<std::shared_ptr<trigger_config>>& get_triggers() const;
    const std::vector<std::shared_ptr<read_config>>& get_reads() const;

    std::vector<std::string> validate() const;

    std::string to_json() const;

    void write_json(util::json::json_writer& writer) const;
};

} // namespace business::config::data_collection

#endif // BUSINESS_CONFIG_DATA_COLLECTION_DATA_COLLECTION_CONFIG_HPP
