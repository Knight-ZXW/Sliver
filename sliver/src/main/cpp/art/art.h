//
// Created by Administrator on 2022/12/28.
// Email: nimdanoob@163.com
//

#ifndef KB_ART_H_
#define KB_ART_H_
#include <stdint.h>
#include <jni.h>
#include <string>
#include "stackvisitor.h"
#include "shadow_frame.h"
namespace kbArt {

//TODO 这里的偏移值 高低版本不一样， 目前只看了Android 12的,后面需要做下区分，版本兼容性
// Offset of field Thread::tls32_.thin_lock_thread_id
#define THREAD_ID_OFFSET 12


enum class SuspendReason : char {
  // Suspending for internal reasons (e.g. GC, stack trace, etc.).
  kInternal,
  // Suspending due to non-runtime, user controlled, code. (For example Thread#Suspend()).
  kForUserCode,
};

enum class ThreadState : uint8_t {
  // `kRunnable` was previously 67 but it is now set to 0 so that we do not need to extract
  // flags from the thread's `state_and_flags` to check for any flag being set while Runnable.
  // Note: All atomic accesses for a location should use the same data size,
  // so the incorrect old approach of reading just 16 bits has been rewritten.

  kTerminated =
  66,                 // TERMINATED     TS_ZOMBIE    Thread.run has returned, but Thread* still around
  kRunnable = 0,                    // RUNNABLE       TS_RUNNING   runnable
  kObsoleteRunnable = 67,           // ---            ---          obsolete value
  kTimedWaiting = 68,               // TIMED_WAITING  TS_WAIT      in Object.wait() with a timeout
  kSleeping,                        // TIMED_WAITING  TS_SLEEPING  in Thread.sleep()
  kBlocked,                         // BLOCKED        TS_MONITOR   blocked on a monitor
  kWaiting,                         // WAITING        TS_WAIT      in Object.wait()
  kWaitingForLockInflation,         // WAITING        TS_WAIT      blocked inflating a thin-lock
  kWaitingForTaskProcessor,         // WAITING        TS_WAIT      blocked waiting for taskProcessor
  kWaitingForGcToComplete,          // WAITING        TS_WAIT      blocked waiting for GC
  kWaitingForCheckPointsToRun,      // WAITING        TS_WAIT      GC waiting for checkpoints to run
  kWaitingPerformingGc,             // WAITING        TS_WAIT      performing GC
  kWaitingForDebuggerSend,          // WAITING        TS_WAIT      blocked waiting for events to be sent
  kWaitingForDebuggerToAttach,      // WAITING        TS_WAIT      blocked waiting for debugger to attach
  kWaitingInMainDebuggerLoop,       // WAITING        TS_WAIT      blocking/reading/processing debugger events
  kWaitingForDebuggerSuspension,    // WAITING        TS_WAIT      waiting for debugger suspend all
  kWaitingForJniOnLoad,             // WAITING        TS_WAIT      waiting for execution of dlopen and JNI on load code
  kWaitingForSignalCatcherOutput,   // WAITING        TS_WAIT      waiting for signal catcher IO to complete
  kWaitingInMainSignalCatcherLoop,  // WAITING        TS_WAIT      blocking/reading/processing signals
  kWaitingForDeoptimization,        // WAITING        TS_WAIT      waiting for deoptimization suspend all
  kWaitingForMethodTracingStart,    // WAITING        TS_WAIT      waiting for method tracing to start
  kWaitingForVisitObjects,          // WAITING        TS_WAIT      waiting for visiting objects
  kWaitingForGetObjectsAllocated,   // WAITING        TS_WAIT      waiting for getting the number of allocated objects
  kWaitingWeakGcRootRead,           // WAITING        TS_WAIT      waiting on the GC to read a weak root
  kWaitingForGcThreadFlip,          // WAITING        TS_WAIT      waiting on the GC thread flip (CC collector) to finish
  kNativeForAbort,                  // WAITING        TS_WAIT      checking other threads are not run on abort.
  kStarting,                        // NEW            TS_WAIT      native thread started, not yet ready to run managed code
  kNative,                          // RUNNABLE       TS_RUNNING   running in a JNI native method
  kSuspended,                       // RUNNABLE       TS_RUNNING   suspended by GC or debugger
};

struct PartialRuntimeP {
  void *thread_list_;

  void *intern_table_;

  void *class_linker_;

  void *signal_catcher_;

  bool use_tombstoned_traces_;

  // Location to which traces must be written on SIGQUIT. Only used if
  // tombstoned_traces_ == false.
  std::string stack_trace_file_;

  void *java_vm_;

  void *jit_;
  void *jit_code_cache_;
};

struct PartialRuntimeR {
  void *thread_list_;

  void *intern_table_;

  void *class_linker_;

  void *signal_catcher_;

  void *jni_id_manager_;

  void *java_vm_;

  void *jit_;
  void *jit_code_cache_;
};

struct PartialRuntimeTiramisu {
    void *thread_list_;

    void *intern_table_;

    void *class_linker_;

    void *signal_catcher_;

    void *small_lrt_allocator_;

    void *jni_id_manager_;

    void *java_vm_;

    void *jit_;
    void *jit_code_cache_;
};


struct JavaVMExt {
  void *functions;
  void *runtime;
};

class ArtHelper {
 public:
  static int init(JNIEnv *env);

  static void *getThreadList();

  static void StackVisitorWalkStack(StackVisitor *visitor, bool include_transitions);

  //Suspend a thread using a peer.
  static void *suspendThreadByPeer(jobject peer, SuspendReason reason, bool *timed_out);

  // Suspend a thread using its thread id, typically used by lock/monitor inflation. Returns the
  // thread on success else null.
  static void *SuspendThreadByThreadId(uint32_t threadId,
                                       SuspendReason suspendReason,
                                       bool *timed_out);

  static bool Resume(void *thread, SuspendReason suspendReason);

  static std::string PrettyMethod(void *art_method, bool with_signature);

  static ThreadState FetchState(void *thread, void *monitor_object, uint32_t *lock_owner_tid);

  static uint64_t GetCpuMicroTime(void *thread);

 private:
  static void *runtime_instance_;
  static int api;

};
}

#endif //KB_ART_H_
