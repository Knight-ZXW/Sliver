#include <vector>
#include <string>
#include <cstdlib>
#include <sys/system_properties.h>
#include <jni.h>
#include <dlfcn.h>
#include <asm-generic/fcntl.h>
#include <fstream>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>

#include "android/log.h"
#include "../sliver_log.h"
#include "../fake_dlfcn.h"
#include "art.h"
#include "art_method.h"
#include "art_thread.h"
#include "stackvisitor.h"

#define ANDROID_R_API 30
#define MAX_SEARCH_LEN 2000

class FetchStackTraceVisitorR;

void *ArtHelper::runtime_instance_ = nullptr;
int ArtHelper::api = 0;

template<typename T>
inline int find_offset(void *hay, int size, T needle) {
    for (int i = 0; i < size; i += sizeof(T)) {
        auto current = reinterpret_cast<T *>(reinterpret_cast<char *>(hay) + i);
        if (*current == needle) {
            return i;
        }
    }
    return -1;
}


 void (*WalkStack_)(StackVisitor *stack, bool include_transitions) = nullptr;


uint32_t (*GetDexPc_)(void *stackVisitor, bool abortOnFailure);

void *(*SuspendThreadByPeer_)(void *thread_list, jobject peer, SuspendReason suspendReason,
                              bool *timed_out) = nullptr;

//_ZN3art10ThreadList23SuspendThreadByThreadIdEjNS_13SuspendReasonEPb
void *(*SuspendThreadByThreadId_)(void *thread_list, uint32_t thread_id, SuspendReason suspendReason, bool *time_out);

bool (*Resume_)(void *thread_list, void *thread, SuspendReason suspendReason);

static std::string (*PrettyMethod_)(void *art_method, bool with_signature) = nullptr;

//_ZN3art7Monitor10FetchStateEPKNS_6ThreadEPNS_6ObjPtrINS_6mirror6ObjectEEEPj
//ref: https://cs.android.com/android/platform/superproject/+/master:art/runtime/monitor.h;l=112?q=FetchState&sq=&ss=android%2Fplatform%2Fsuperproject
void (*FetchState_)(void *thread, void *monitor_object, uint32_t *lock_owner_tid);


void ArtHelper::init(JNIEnv *env, int api) {
    ArtHelper::api = api;
    JavaVM *javaVM;
    env->GetJavaVM(&javaVM);
    auto *javaVMExt = (JavaVMExt *) javaVM;
    void *runtime = javaVMExt->runtime;
    LOGV("runtime ptr: %p, vmExtPtr: %p", runtime, javaVMExt);

    if (api < ANDROID_R_API) {
        runtime_instance_ = runtime;
    } else {
        int vm_offset = find_offset(runtime, MAX_SEARCH_LEN, javaVM);
        runtime_instance_ = reinterpret_cast<void *>(reinterpret_cast<char *>(runtime)
                                                     + vm_offset - offsetof(PartialRuntimeR, java_vm_));
    }
    initMethods();
}


void ArtHelper::initMethods() {
    void *handle = dlopen_ex("libart.so", RTLD_NOW);

    WalkStack_ = reinterpret_cast<void (*)(StackVisitor *,bool )>(dlsym_ex(handle,
                                                    "_ZN3art12StackVisitor9WalkStackILNS0_16CountTransitionsE0EEEvb"));
    GetDexPc_ = reinterpret_cast<uint32_t (*)(void *, bool)>(dlsym_ex(handle, "_ZNK3art12StackVisitor8GetDexPcEb"));
    SuspendThreadByPeer_ = reinterpret_cast<void *(*)(void *, jobject, SuspendReason, bool *)>(dlsym_ex(handle, "_ZN3art10ThreadList19SuspendThreadByPeerEP8_jobjectbNS_13SuspendReasonEPb"));
    SuspendThreadByThreadId_ = reinterpret_cast<void *(*)(void *, uint32_t, SuspendReason, bool *)>(dlsym_ex(handle, "_ZN3art10ThreadList23SuspendThreadByThreadIdEjNS_13SuspendReasonEPb"));

    Resume_ = reinterpret_cast<bool (*)(void *, void *, SuspendReason)>(dlsym_ex(handle, "_ZN3art10ThreadList6ResumeEPNS_6ThreadENS_13SuspendReasonE"));
    PrettyMethod_ = reinterpret_cast<std::string (*)(void *, bool)>(dlsym_ex(handle, "_ZN3art9ArtMethod12PrettyMethodEb"));
}

void ArtHelper::StackVisitorWalkStack(StackVisitor *visitor, bool include_transitions) {
    WalkStack_(visitor, include_transitions);
}

std::string ArtHelper::PrettyMethod(void *art_method, bool with_signature) {
    return PrettyMethod_(art_method, with_signature);
}


void *ArtHelper::SuspendThreadByPeer(jobject peer, SuspendReason suspendReason, bool *timed_out) {
    return SuspendThreadByPeer_(ArtHelper::getThreadList(), peer, suspendReason, timed_out);
}

void *ArtHelper::SuspendThreadByThreadId(uint32_t threadId, SuspendReason suspendReason, bool *timed_out) {
    return SuspendThreadByThreadId_(ArtHelper::getThreadList(),threadId, suspendReason, timed_out);
}

bool ArtHelper::Resume(void *thread, SuspendReason suspendReason) {
    return Resume_(ArtHelper::getThreadList(), thread, suspendReason);
}

void *ArtHelper::getThreadList() {
    if (runtime_instance_ == nullptr || api < ANDROID_R_API) {
        return nullptr;
    }
    auto *runtimeR = (PartialRuntimeR *) runtime_instance_;
    return runtimeR->thread_list_;
}






