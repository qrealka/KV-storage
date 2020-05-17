/**
 * @file GRPC Asynchronous server implementation
 *
 */

#include "AsyncServerImpl.h"
#include "ServerCallGet.h"
#include "ServerCallDelete.h"
#include <grpc/support/log.h>

namespace kv_storage
{

AsyncServerImpl::AsyncServerImpl(std::string adr) : serverAddress_(std::move(adr))
{
}

AsyncServerImpl::~AsyncServerImpl() noexcept
{
    server_->Shutdown();
    queue_->Shutdown();
}

void AsyncServerImpl::Run()
{
    ServerCallGet::spawn(service_, *queue_);
    ServerCallDelete::spawn(service_, *queue_);
}

}


