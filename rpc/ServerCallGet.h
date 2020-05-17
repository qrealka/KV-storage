/**
 * @file Async GRPC server handler for GET
 *
 */

#pragma once
#include "ServerCallBase.h"

namespace kv_storage
{

/**
 * 'GET' end-point for KeyValue storage
 */
class ServerCallGet final : public ServerCallBase
{
public:
    static void spawn(KeyValueStore::AsyncService& service, grpc::ServerCompletionQueue& queue);

private:
    ServerCallGet(KeyValueStore::AsyncService& service, grpc::ServerCompletionQueue& queue);
    void Proceed(bool ok) override;

private:
    bool callSuccess_;
    bool callInputReceived_;
    bool responderCreated_;
    GetRequest request_;
    GetResponse response_;
    grpc::ServerAsyncReaderWriter<GetResponse, GetRequest> responder_;
};

}




