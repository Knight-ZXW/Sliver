#ifndef SLIVER_FAKE_STACK_VISITOR_H_
#define SLIVER_FAKE_STACK_VISITOR_H_

#include "shadow_frame.h"

const size_t STRUCT_COMPAT= sizeof(size_t) * 126;
/**
 *
 */
class StackVisitor {
public:
    // This enum defines a flag to control whether inlined frames are included
    // when walking the stack.
    enum class StackWalkKind {
        kIncludeInlinedFrames,
        kSkipInlinedFrames,
    };

    virtual ~StackVisitor() {}

    virtual bool VisitFrame() = 0;

    void *thread_ = nullptr;
    const StackWalkKind walkKind = StackWalkKind::kIncludeInlinedFrames;
    ShadowFrame *cur_shadow_frame = nullptr;
    void **cur_quick_frame_ = nullptr;
//    uintptr_t cur_quick_frame_pc_ = 0;
//    const void *cur_oat_quick_method_header_ = nullptr;
//    // Lazily computed, number of frames in the stack.
//    size_t num_frames_ = 0;
//    // Depth of the frame we're currently at.
//    size_t cur_depth_ =0;

    //开辟一个足够大的内存空间 用于存在其他变量
    char param[STRUCT_COMPAT] ={};
public:
    void *GetMethod() {
        if (cur_shadow_frame != nullptr) {
            return cur_shadow_frame->method;
        } else if (cur_quick_frame_ != nullptr) {
            return *cur_quick_frame_;
        }
        return nullptr;
    }

};


#endif