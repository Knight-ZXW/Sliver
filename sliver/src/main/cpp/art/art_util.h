//
// Created by knight-zxw on 2022/12/31.
//

#ifndef KB_ART_UTIL_H_
#define KB_ART_UTIL_H_
#include "stackvisitor.h"
#include "art.h"

namespace kbArt {
#ifndef __LP64__
#define ART_UTIL_LIBART   "/system/lib/libart.so"
#define ART_UTIL_LIBART_Q "/apex/com.android.runtime/lib/libart.so"
#define XCC_UTIL_LIBART_R "/apex/com.android.art/lib/libart.so"
#else
#define ART_UTIL_LIBART   "/system/lib64/libart.so"
#define ART_UTIL_LIBART_Q "/apex/com.android.runtime/lib64/libart.so"
#define ART_UTIL_LIBART_R "/apex/com.android.art/lib64/libart.so"
#endif
typedef void (*WalkStack_t)(StackVisitor *stack_visitor, bool include_transitions);

typedef void *(*SuspendThreadByPeer_t)(void *thread_list, jobject peer, SuspendReason suspendReason,
                                       bool *timed_out);

//_ZN3art10ThreadList23SuspendThreadByThreadIdEjNS_13SuspendReasonEPb
typedef void *(*SuspendThreadByThreadId_t)(void *thread_list,
                                           uint32_t thread_id,
                                           SuspendReason suspendReason,
                                           bool *time_out);

typedef bool (*Resume_t)(void *thread_list, void *thread, SuspendReason suspendReason);

typedef std::string (*PrettyMethod_t)(void *art_method, bool with_signature);

typedef ThreadState (*FetchState_t)(void *thread,/* out */void* monitor_object,/* out */uint32_t* lock_owner_tid);

}

#endif //KB_ART_UTIL_H_
