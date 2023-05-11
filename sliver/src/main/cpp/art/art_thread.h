//
// Created by knight-zxw on 2023/1/3.
// Email: nimdanoob@163.com
//

#ifndef KB_ART_THREAD_H_
#define KB_ART_THREAD_H_
#include "macro.h"
#include "tls.h"
#include "common.h"



namespace kbArt {
class Thread {
  struct PACKED(4) tls_32bit_sized_values {
    // We have no control over the size of 'bool', but want our boolean fields
    // to be 4-byte quantities.
    using bool32_t = uint32_t;

    // The state and flags field must be changed atomically so that flag values aren't lost.
    // See `StateAndFlags` for bit assignments of `ThreadFlag` and `ThreadState` values.
    // Keeping the state and flags together allows an atomic CAS to change from being
    // Suspended to Runnable without a suspend request occurring.
    std::atomic<uint32_t> state_and_flags;
    // A non-zero value is used to tell the current thread to enter a safe point

    // at the next poll.
    int suspend_count;

    // Thin lock thread id. This is a small integer used by the thin lock implementation.
    // This is not to be confused with the native thread's tid, nor is it the value returned
    // by java.lang.Thread.getId --- this is a distinct value, used only for locking. One
    // important difference between this id and the ids visible to managed code is that these
    // ones get reused (to ensure that they fit in the number of bits available).
    uint32_t thin_lock_thread_id;

    // System thread id.
    uint32_t tid;

  } tls32_;

  struct PACKED(4) tls_32bit_sized_values_android11 {
    // The state and flags field must be changed atomically so that flag values aren't lost.
    // See `StateAndFlags` for bit assignments of `ThreadFlag` and `ThreadState` values.
    // Keeping the state and flags together allows an atomic CAS to change from being
    // Suspended to Runnable without a suspend request occurring.
    std::atomic<uint32_t> state_and_flags;
    // A non-zero value is used to tell the current thread to enter a safe point

    // A non-zero value is used to tell the current thread to enter a safe point
    // at the next poll.
    int suspend_count;

    // How much of 'suspend_count_' is by request of the debugger, used to set things right
    // when the debugger detaches. Must be <= suspend_count_.
    int debug_suspend_count;

    // Thin lock thread id. This is a small integer used by the thin lock implementation.
    // This is not to be confused with the native thread's tid, nor is it the value returned
    // by java.lang.Thread.getId --- this is a distinct value, used only for locking. One
    // important difference between this id and the ids visible to managed code is that these
    // ones get reused (to ensure that they fit in the number of bits available).
    uint32_t thin_lock_thread_id;

    // System thread id.
    uint32_t tid;

  } tls32_android11;

  struct PACKED(4) tls_32bit_sized_values_android10 {

    // The state and flags field must be changed atomically so that flag values aren't lost.
    // See `StateAndFlags` for bit assignments of `ThreadFlag` and `ThreadState` values.
    // Keeping the state and flags together allows an atomic CAS to change from being
    // Suspended to Runnable without a suspend request occurring.
    std::atomic<uint32_t> state_and_flags;
    // A non-zero value is used to tell the current thread to enter a safe point

    // at the next poll.
    int suspend_count;

    int debug_suspend_count;

    // Thin lock thread id. This is a small integer used by the thin lock implementation.
    // This is not to be confused with the native thread's tid, nor is it the value returned
    // by java.lang.Thread.getId --- this is a distinct value, used only for locking. One
    // important difference between this id and the ids visible to managed code is that these
    // ones get reused (to ensure that they fit in the number of bits available).
    uint32_t thin_lock_thread_id;

    // System thread id.
    uint32_t tid;

  } tls32_android10;


 public:
// Guaranteed to be non-zero.
  inline uint32_t GetThreadId() const {
    static uint32_t offset =0;
    if (offset==0){
      int api_level = android_get_device_api_level();
      if (api_level>= __ANDROID_API_S__){ // >=Android 12
        offset = offsetof(tls_32bit_sized_values,thin_lock_thread_id);
      }else if (api_level >=__ANDROID_API_Q__){ // <=Android 10
        offset = offsetof(tls_32bit_sized_values_android10,thin_lock_thread_id);
      } else if ( api_level<=__ANDROID_API_P__){
        offset = offsetof(tls_32bit_sized_values_android10,thin_lock_thread_id);
      }
    }
    return *(uint32_t*)((char *)this+offset);
  }

  inline uint32_t GetTid() {
    return tls32_.tid;
  }


  inline static Thread *current() {
    void* thread = __get_tls()[TLS_SLOT_ART_THREAD_SELF];
    return reinterpret_cast<Thread*>(thread);
  }
};

}

#endif //KB_ART_THREAD_H_
