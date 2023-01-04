//
// Created by Administrator on 2022/12/28.
// Email: nimdanoob@163.com
//

#include "art.h"
#include "logger.h"
#include <vector>
#include <string>
#include <cstdlib>
#include <sys/system_properties.h>
#include "xdl.h"
#include "art_util.h"

namespace kbArt {
void *ArtHelper::runtime_instance_ = nullptr;
int ArtHelper::api = 0;

static WalkStack_t walk_stack = nullptr;

static SuspendThreadByPeer_t suspend_thread_by_peer = nullptr;

static SuspendThreadByThreadId_t suspend_thread_by_thread_id = nullptr;

static Resume_t resume = nullptr;

static PrettyMethod_t pretty_method = nullptr;
static FetchState_t fetchState = nullptr;
static GetCpuMicroTime_t getCpuMicroTime = nullptr;

//source from: https://github.com/tiann/FreeReflection/blob/master/library/src/main/cpp/art.cpp
template<typename T>
int findOffset(void *start, int regionStart, int regionEnd, T target) {
  if (nullptr == start || regionStart < 0 || regionEnd <= 0) {
    return -1;
  }
  char *c_start = reinterpret_cast<char *>(start);
  for (int i = regionStart; i < regionEnd; i += 4) {
    T *current_value = reinterpret_cast<T *>(c_start + i);
    if (target == *current_value) {
      LOGE("artHelper", "find target %p", current_value);
      return i;
    }
  }
  return -1;
}

//int dl_iterate_callback(dl_phdr_info *info, size_t size, void *data) {
//  // find libart
//  if (strstr(info->dlpi_name, "libart.so")) {
//    libart = info->dlpi_name;
//  }
//  return 0;
//}

static int load_symbols() {
  LOGV("ArtHelper", "start load_symbols");
  void *handle = xdl_open("/apex/com.android.art/lib64/libart.so",
                          XDL_TRY_FORCE_LOAD);
  LOGV("ArtHelper", "handle is %p", handle);

  walk_stack = reinterpret_cast<WalkStack_t>(xdl_dsym(handle,
                                                      "_ZN3art12StackVisitor9WalkStackILNS0_16CountTransitionsE0EEEvb",
                                                      nullptr));
  if (walk_stack == nullptr) {
    return -1;
  }

  suspend_thread_by_peer =
      reinterpret_cast<SuspendThreadByPeer_t>(xdl_dsym(handle,
                                                       "_ZN3art10ThreadList19SuspendThreadByPeerEP8_jobjectbNS_13SuspendReasonEPb",
                                                       nullptr));
  if (suspend_thread_by_peer == nullptr) {
    return -1;
  }
  suspend_thread_by_thread_id =
      reinterpret_cast<SuspendThreadByThreadId_t>(xdl_dsym(handle,
                                                           "_ZN3art10ThreadList23SuspendThreadByThreadIdEjNS_13SuspendReasonEPb",
                                                           nullptr));
  if (suspend_thread_by_thread_id == nullptr) {
    return -1;
  }

  resume = reinterpret_cast<Resume_t>(xdl_dsym(handle,
                                               "_ZN3art10ThreadList6ResumeEPNS_6ThreadENS_13SuspendReasonE",
                                               nullptr));
  if (resume == nullptr) {
    return -1;
  }

  pretty_method = reinterpret_cast<PrettyMethod_t>(xdl_dsym(handle,
                                                            "_ZN3art9ArtMethod12PrettyMethodEb",
                                                            nullptr));
  if (pretty_method == nullptr) {
    return -1;
  }

  fetchState = reinterpret_cast<FetchState_t>(xdl_dsym(handle,
                                                       "_ZN3art7Monitor10FetchStateEPKNS_6ThreadEPNS_6ObjPtrINS_6mirror6ObjectEEEPj",
                                                       nullptr));
  getCpuMicroTime = reinterpret_cast<GetCpuMicroTime_t>(xdl_dsym(handle,"_ZNK3art6Thread15GetCpuMicroTimeEv",
                                                                 nullptr));
  if (fetchState == nullptr) {
    return -1;
  }
  return 1;
}

int ArtHelper::init(JNIEnv *env) {
  char api_level_str[5];
  __system_property_set("ro.build.version.sdk", api_level_str);
  int api_level = atoi(api_level_str);
  ArtHelper::api = api_level;
  JavaVM *javaVM;
  env->GetJavaVM(&javaVM);

  auto *javaVMExt = reinterpret_cast<JavaVMExt *>(javaVM);
  void *runtime = javaVMExt->runtime;

  LOGV("ArtHelper", "runtime ptr: %p", runtime);
  const int MAX = 2000;
  //找到javaVmExt在 Runtime中的偏移地址
  int offsetOfVmExt = findOffset(runtime, 0, MAX, javaVMExt);
  LOGV("ArtHelper", "offsetOfVmExt: %d", offsetOfVmExt);
  if (offsetOfVmExt < 0) {
    return -1;
  }
  ArtHelper::runtime_instance_ =
      reinterpret_cast<char *>(runtime) + offsetOfVmExt -
          offsetof(PartialRuntime, java_vm_);
  load_symbols();
  return 1;

}

void *ArtHelper::getThreadList() {
  return reinterpret_cast<PartialRuntime *>(runtime_instance_)->thread_list_;
}

void *
ArtHelper::suspendThreadByPeer(jobject peer, SuspendReason suspendReason, bool *timed_out) {
  return suspend_thread_by_peer(ArtHelper::getThreadList(), peer, suspendReason, timed_out);
}

void *ArtHelper::SuspendThreadByThreadId(uint32_t threadId,
                                         SuspendReason suspendReason,
                                         bool *timed_out) {
  void *thread_list = ArtHelper::getThreadList();
  return suspend_thread_by_thread_id(thread_list, threadId, suspendReason, timed_out);
}

bool ArtHelper::Resume(void *thread, SuspendReason suspendReason) {
  return resume(ArtHelper::getThreadList(), thread, suspendReason);
}

std::string ArtHelper::PrettyMethod(void *art_method, bool with_signature) {
  return pretty_method(art_method, with_signature);
}

void ArtHelper::StackVisitorWalkStack(StackVisitor *visitor, bool include_transitions) {
  walk_stack(visitor, include_transitions);
}

ThreadState ArtHelper::FetchState(void *thread, void *monitor_object, uint32_t *lock_owner_tid) {
  return fetchState(thread,monitor_object,lock_owner_tid);
}

// Returns the thread-specific CPU-time clock in microseconds or -1 if unavailable.
uint64_t ArtHelper::GetCpuMicroTime(void *thread) {
  return getCpuMicroTime(thread);
}

}
