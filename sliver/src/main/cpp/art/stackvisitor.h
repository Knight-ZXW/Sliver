#ifndef SLIVER_FAKE_STACK_VISITOR_H_
#define SLIVER_FAKE_STACK_VISITOR_H_
#include "shadow_frame.h"

class StackVisitor {
public:
    // This enum defines a flag to control whether inlined frames are included
    // when walking the stack.
    enum class StackWalkKind {
        kIncludeInlinedFrames,
        kSkipInlinedFrames,
    };
public:
    virtual ~StackVisitor() {}

    virtual bool VisitFrame() = 0;

    void *thread_ = nullptr;
    const StackWalkKind walkKind = StackWalkKind::kIncludeInlinedFrames;

    ShadowFrame *cur_shadow_frame;
    void **cur_quick_frame_ = nullptr;

    //开辟一个足够大的内存空间 用于存在其他变量
    char otherPack[80];

};


#endif