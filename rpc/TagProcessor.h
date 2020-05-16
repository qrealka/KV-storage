/**
 * @file std::function wrapper to customize events handlers
 *       todo: make CRTP instead of std::function
 */

#pragma once
#include <list>
#include <functional>

namespace kv_storage {

// We add a 'TagProcessor' to the completion queue for each event.
// This way, each tag knows how to process itself.
using TagProcessor = std::function<void(bool)>;
struct TagInfo
{
    TagProcessor* tagProcessor; // The function to be called to process incoming event
    bool ok; // The result of tag processing as indicated by gRPC library.
             // Calling it 'ok' to be in sync with other gRPC examples.
};

using TagList = std::list<TagInfo>;

}
