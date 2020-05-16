/**
 * @file common parts rpc methods
 *
 */

#pragma once
#include <grpcpp/grpcpp.h>
#include "kv.grpc.pb.h"

namespace kv_storage
{

// all real methods implementation
class CoreService;

/**
 * Abstract class to describe and initialize Async Server Call
 */
class ServerCallBase {
public:
    enum class CallStatus {
        CREATE, PROCESS, FINISH
    };
    virtual ~ServerCallBase() noexcept = default;
    virtual void Proceed(bool ok) = 0;

protected:
    ServerCallBase(KeyValueStore::AsyncService& service, grpc::ServerCompletionQueue& queue)
        : service_(&service), queue_(&queue), status_(CallStatus::CREATE), coreService_(nullptr)
    {
    }

    KeyValueStore::AsyncService* service_;
    grpc::ServerCompletionQueue* queue_;
    grpc::ServerContext          ctx_;
    CallStatus                   status_;
    CoreService*                 coreService_;
};
}