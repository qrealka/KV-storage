/**
 * @file
 */

#include "ServerCallDelete.h"
#include "core/CoreService.h"

namespace kv_storage
{

void ServerCallDelete::spawn(kv_storage::KeyValueStore::AsyncService& service,
                             grpc::ServerCompletionQueue& queue)
{
    new ServerCallDelete(service, queue);
}

ServerCallDelete::ServerCallDelete(KeyValueStore::AsyncService& service, grpc::ServerCompletionQueue& queue)
    : ServerCallBase(service, queue), callSuccess_(false)
    , responderCreated_(false), responder_(&ctx_)
{
    Proceed(true);
}

void ServerCallDelete::Proceed(bool ok)
{
    switch (status_) {
        case CallStatus::CREATE:
        {
            status_ = CallStatus::PROCESS;
            service_->RequestDelete(&ctx_, &responder_, queue_, queue_, this);
        } break;
        case CallStatus::PROCESS:
        {
            if (!responderCreated_) {
                GPR_ASSERT(service_);
                GPR_ASSERT(queue_);
                new ServerCallDelete(*service_, *queue_);
                responderCreated_ = true;
            }
            if (!ok) {
                // send reply
                const auto result = coreService_->deleteValueByKey(request_.key());
                switch (result) {
                    case DeleteResult::Succeeded:
                        response_.set_status(DeleteResponse_DeleteStatus_Succeeded);
                        break;
                    case DeleteResult::NotFound:
                        response_.set_status(DeleteResponse_DeleteStatus_NotFound);
                        break;
                    case DeleteResult::Locked:
                        response_.set_status(DeleteResponse_DeleteStatus_Locked);
                        break;
                }
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

