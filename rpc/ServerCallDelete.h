/**
 * @file Async GRPC server handler for DELETE
 */

#pragma once
#include "ServerCallBase.h"

namespace kv_storage
{

/**
 * 'DELETE' end-point for KeyValue storage
 */
class ServerCallDelete final : public ServerCallBase
{
public:
    static void spawn(KeyValueStore::AsyncService& service, grpc::ServerCompletionQueue& queue);

private:
    ServerCallDelete(KeyValueStore::AsyncService& service, grpc::ServerCompletionQueue& queue);
    void Proceed(bool ok) override;

private:
    bool callSuccess_;
    bool responderCreated_;
    DeleteRequest request_;
    DeleteResponse response_;
    grpc::ServerAsyncReader<DeleteResponse, DeleteRequest> responder_;
};

}
