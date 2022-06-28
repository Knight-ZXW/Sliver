#include "./art/stackvisitor.h"
class FetchStackTraceVisitorR: public StackVisitor{

    void* GetMethod(){
        if (cur_shadow_frame!= nullptr){
            return cur_shadow_frame->method;
        } else if(cur_quick_frame_ != nullptr){
            return *cur_quick_frame_;
        }
        return nullptr;
    }

    bool  VisitFrame() override{
        GetMethod()
    }
};