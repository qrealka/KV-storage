/**
 * @file Grpc Async Server for KV storage
 *
 */

#pragma once
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "kv.grpc.pb.h"

namespace kv_storage {

class AsyncServerImpl final
{
public:
    explicit AsyncServerImpl(std::string adr);
    ~AsyncServerImpl() noexcept;
    void Run();
private:
    std::string serverAddress_;
    std::unique_ptr<grpc::ServerCompletionQueue> queue_;
    std::unique_ptr<grpc::Server> server_;
    KeyValueStore::AsyncService service_;
};

}





