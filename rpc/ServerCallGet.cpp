/**
 * @file
 *
 */

#include "ServerCallGet.h"
#include "core/CoreService.h"

namespace kv_storage
{

void ServerCallGet::spawn(KeyValueStore::AsyncService& service, grpc::ServerCompletionQueue& queue)
{
    new ServerCallGet(service, queue);
}

ServerCallGet::ServerCallGet(KeyValueStore::AsyncService& service,
                             grpc::ServerCompletionQueue& queue)
    : ServerCallBase(service, queue), callSuccess_(false), callInputReceived_(false)
    , responderCreated_(false), responder_(&ctx_)
{
    Proceed(true);
}

void ServerCallGet::Proceed(bool ok)
{
    switch (status_) {
        case CallStatus::CREATE:
            status_ = CallStatus::PROCESS;
            service_->RequestGet(&ctx_, &responder_, queue_, queue_, this);
            break;
        case CallStatus::PROCESS:
        {
            if (!responderCreated_) {
                GPR_ASSERT(service_);
                GPR_ASSERT(queue_);
                new ServerCallGet(*service_, *queue_);
                responderCreated_ = true;
            }

            // receive a whole request
            if (!callInputReceived_) {
                // reading mode
                if (!ok) {
                    callInputReceived_ = true;
                    ok = true;
                } else  {
                    responder_.Read(&request_, (void*)this);
                }
            }

            if (callInputReceived_) {
                // analyze that we have a GET result from CORE
                if (!ok || ctx_.IsCancelled()) {
                    status_ = CallStatus::FINISH;
                    // todo: add cancel message
                    responder_.Finish(grpc::Status(), (void*)this);
                } else {
                    // todo: make async call
                    if (auto result = coreService_->getValueByKey(request_.key())) {
                        response_.set_value(*result);
                        response_.set_success(true);
                    } else {
                        response_.set_success(false);
                        response_.set_value(std::string());
                    }
                    // todo: WriteAndFinish ???
                    responder_.Write(response_, (void*)this);
                }
            }

        }  break;
        case CallStatus::FINISH:
            delete this;
            break;
        default:
            GPR_ASSERT(false);
            break;
    }
}


} // namespace kv_storage

