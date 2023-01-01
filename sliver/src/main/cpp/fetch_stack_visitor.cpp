//
// Created by knight-zxw on 2022/12/31.
//
#include "stackvisitor.h"
#include "art.h"
#include "art_method.h"
#include "logger.h"
using namespace kbArt;
class FetchStackVisitor : public StackVisitor {

  bool VisitFrame() override {
    void *method = GetMethod();

    auto *artMethod = static_cast<ArtMethod *>(method);
    if (artMethod == nullptr || artMethod->IsRuntimeMethod()){
      return true;
    }
    if (StackVisitorCallback!= nullptr){
      return StackVisitorCallback(artMethod,visitorData);
    }
    return true;
  }

 public:
  FetchStackVisitor(void *thread,
                    void* visitorData,
                    bool (*callback)(ArtMethod *,void* visitorData)){
    this->thread_ = thread;
    this->StackVisitorCallback = callback;
    this->visitorData = visitorData;
  }

 private:
  bool (*StackVisitorCallback)(ArtMethod *art_method,void* visitorData) = nullptr;
  void* visitorData = nullptr;
};
