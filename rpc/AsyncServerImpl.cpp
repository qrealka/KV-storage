/**
 * @file GRPC Asynchronous server implementation
 *
 */

#include "AsyncServerImpl.h"
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

}

}


