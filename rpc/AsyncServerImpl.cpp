/**
 * @file GRPC Asynchronous server implementation
 *
 */

#include "AsyncServerImpl.h"
#include "ServerCallGet.h"
#include "ServerCallDelete.h"
#include "ServerCallPut.h"

#include <grpc/support/log.h>

namespace kv_storage
{

AsyncServerImpl::AsyncServerImpl(std::string adr) : serverAddress_(std::move(adr))
{
}

AsyncServerImpl::~AsyncServerImpl() noexcept
{
    server_->Shutdown();
    // Always shutdown the completion queue after the server.
    queue_->Shutdown();
}

void AsyncServerImpl::Run()
{
    grpc::ServerBuilder builder;
    builder.AddListeningPort(serverAddress_, grpc::InsecureServerCredentials());
    builder.RegisterService(&service_);
    queue_ = builder.AddCompletionQueue(&service_);
    server_ = builder.BuildAndStart();

    std::cout << "Server listening on " << serverAddress_ << std::endl;

    ServerCallGet::spawn(service_, *queue_);
    ServerCallDelete::spawn(service_, *queue_);
    ServerCallPut::spawn(service_, *queue_);

    void* tag;  // uniquely identifies a request.
    bool ok;
    while(true)
    {
        const auto queueReady = queue_->Next(&tag, &ok);
        if (auto* calldata = static_cast<ServerCallBase*>(tag) ) {
            calldata->Proceed(ok);
        } else if (!queueReady){
            break;
        }
    }
    }

}


