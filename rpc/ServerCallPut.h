/**
 * @file Async GRPC server handler for PUT
 */

#pragma once
#include "ServerCallBase.h"

namespace kv_storage
{

/**
 * 'PUT' end-point for KeyValue storage
 */
class ServerCallPut final : public ServerCallBase
{
public:
    static void spawn(KeyValueStore::AsyncService& service, grpc::ServerCompletionQueue& queue);

private:
    ServerCallPut(KeyValueStore::AsyncService& service, grpc::ServerCompletionQueue& queue);
    void Proceed(bool ok) override;

private:
    bool callSuccess_;
    bool responderCreated_;
    PutRequest request_;
    PutResponse response_;
    grpc::ServerAsyncReader<PutResponse, PutRequest> responder_;
};

}
