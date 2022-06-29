#include "./art/stackvisitor.h"
#include "sliver_log.h"
#include "art/art.h"
#include "art/art_method.h"

class FetchStackTraceVisitorR : public StackVisitor {

    bool VisitFrame() override {
        void *method = GetMethod();

        if (method == nullptr) {
            //should never happen
            return false;
        }
        auto *artMethod = static_cast<ArtMethod *>(method);
        if (artMethod->IsRuntimeMethod()){
            LOGV("this method is runtimeMethod,so we should ignore it");
        }
        //TODO 进行异步 pretty操作
        const std::string &methodSignature = ArtHelper::PrettyMethod(method, true);
        LOGV("method descriptor is -> %s",methodSignature.c_str());
        if (StackVisitorCallback!= nullptr){
            return StackVisitorCallback(method,StackVisitorData);
        }
        return true;
    }

public:
    FetchStackTraceVisitorR(void *thread, void *data, bool (*callback)(void *, void *)){
        this->thread_ = thread;
        this->StackVisitorCallback = callback;
        this->frameData =data;
    }

private:
    bool (*StackVisitorCallback)(void *method, void *data) = nullptr;
    void *StackVisitorData = nullptr;
    void* frameData;
};




