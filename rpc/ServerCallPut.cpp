/**
 * @file
 */

#include "ServerCallPut.h"
#include "core/CoreService.h"
namespace kv_storage
{

void ServerCallPut::spawn(kv_storage::KeyValueStore::AsyncService& service,
                             grpc::ServerCompletionQueue& queue)
{
    new ServerCallPut(service, queue);
}

ServerCallPut::ServerCallPut(KeyValueStore::AsyncService& service, grpc::ServerCompletionQueue& queue)
    : ServerCallBase(service, queue), callSuccess_(false)
    , responderCreated_(false), responder_(&ctx_)
{
    Proceed(true);
}

void ServerCallPut::Proceed(bool ok)
{
    switch (status_) {
        case CallStatus::CREATE:
        {
            status_ = CallStatus::PROCESS;
            service_->RequestPut(&ctx_, &responder_, queue_, queue_, this);
        } break;
        case CallStatus::PROCESS:
        {
            if (!responderCreated_) {
                GPR_ASSERT(service_);
                GPR_ASSERT(queue_);
                new ServerCallPut(*service_, *queue_);
                responderCreated_ = true;
            }
            if (!ok) {
                // send reply
                const auto result = coreService_->putValue(request_.key(), request_.value());
                response_.set_success(result);
                status_ = CallStatus::FINISH;
                responder_.Finish(response_, grpc::Status(), (void*)this);
            } else {
                responder_.Read(&request_, (void*)this);
            }
        } break;
        case CallStatus::FINISH:
            delete this;
            break;
        default:
            GPR_ASSERT(false);
            break;
    }
}

}
