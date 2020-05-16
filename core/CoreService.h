/**
 * @file Interface to core functionality
 *
 */

#pragma once
#include <string>
#include <string_view>
#include <optional>

namespace kv_storage
{

enum class DeleteResult : char
{
    Succeeded = 0,
    NotFound = 1,
    Locked = 2,
};

class CoreService
{
public:
    [[nodiscard]]
    virtual std::optional<std::string> getValueByKey(const std::string& key) = 0;

    virtual DeleteResult deleteValueByKey(const std::string& key) = 0;

    virtual bool putValue(std::string_view key, std::string value) = 0;

    CoreService(const CoreService&) = delete;
    CoreService& operator=(const CoreService&) = delete;

    virtual ~CoreService() noexcept = default;
protected:
    explicit CoreService(size_t cacheLimit) : cacheLimit_(cacheLimit) {}
    size_t cacheLimit_;
};

}