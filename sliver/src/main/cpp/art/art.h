#ifndef SLIVER_ART_HELPER
#define SLIVER_ART_HELPER

#include <jni.h>
#include "../sliver_log.h"
#include "stackvisitor.h"
#include <list>
#include <stdint.h>
#include <string>
#include <vector>

// The various reasons that we might be suspending a thread.
enum class SuspendReason : char {
    // Suspending for internal reasons (e.g. GC, stack trace, etc.).
    // TODO Split this into more descriptive sections.
    kInternal,
    // Suspending due to non-runtime, user controlled, code. (For example Thread#Suspend()).
    kForUserCode,
};

template<typename T>
int findOffset(void *start, int regionStart, int regionEnd, T value) {
    if (nullptr == start || regionEnd <= 0 || regionStart < 0) {
        return -1;
    }
    char *c_start = (char *) start;
    for (int i = regionStart; i < regionEnd; i += 4) {
        //将 c_start+i 对应的地址用 一个指针保存
        T *current_value = (T *) (c_start + i);
        //比较这两个指针指向的地址是否相同
        //比较这两个指针指向的地址是否相同
        if (value == *current_value) {
            LOGV("found offset:%d", i);
            //todo 通过 打印地址 理解下 这段函数
            return i;
        }
    }
    return -2;

}

// Android 6.0: http://androidxref.com/6.0.0_r5/xref/art/runtime/runtime.h
// Android 7.0: http://androidxref.com/7.0.0_r1/xref/art/runtime/runtime.h
// Android 7.1.1: http://androidxref.com/7.1.1_r6/xref/art/runtime/runtime.h
// Android 8.0: http://androidxref.com/8.0.0_r4/xref/art/runtime/runtime.h

struct Runtime_7X {

    uint64_t callee_save_methods_[3];
    void *pre_allocated_OutOfMemoryError_;
    void *pre_allocated_NoClassDefFoundError_;
    void *resolution_method_;
    void *imt_conflict_method_;
    // Unresolved method has the same behavior as the conflict method, it is used by the class linker
    // for differentiating between unfilled imt slots vs conflict slots in superclasses.
    void *imt_unimplemented_method_;
    void *sentinel_;

    int instruction_set_;
    uint32_t callee_save_method_frame_infos_[9]; // QuickMethodFrameInfo = uint32_t * 3

    void *compiler_callbacks_;
    bool is_zygote_;
    bool must_relocate_;
    bool is_concurrent_gc_enabled_;
    bool is_explicit_gc_disabled_;
    bool dex2oat_enabled_;
    bool image_dex2oat_enabled_;

    std::string compiler_executable_;
    std::string patchoat_executable_;
    std::vector<std::string> compiler_options_;
    std::vector<std::string> image_compiler_options_;
    std::string image_location_;

    std::string boot_class_path_string_;
    std::string class_path_string_;
    std::vector<std::string> properties_;

    // The default stack size for managed threads created by the runtime.
    size_t default_stack_size_;

    void *heap_;
};

struct Runtime_8X {

    uint64_t callee_save_methods_[3];
    void *pre_allocated_OutOfMemoryError_;
    void *pre_allocated_NoClassDefFoundError_;
    void *resolution_method_;
    void *imt_conflict_method_;
    // Unresolved method has the same behavior as the conflict method, it is used by the class linker
    // for differentiating between unfilled imt slots vs conflict slots in superclasses.
    void *imt_unimplemented_method_;
    void *sentinel_;

    int instruction_set_;
    uint32_t callee_save_method_frame_infos_[9]; // QuickMethodFrameInfo = uint32_t * 3

    void *compiler_callbacks_;
    bool is_zygote_;
    bool must_relocate_;
    bool is_concurrent_gc_enabled_;
    bool is_explicit_gc_disabled_;
    bool dex2oat_enabled_;
    bool image_dex2oat_enabled_;

    std::string compiler_executable_;
    std::string patchoat_executable_;
    std::vector<std::string> compiler_options_;
    std::vector<std::string> image_compiler_options_;
    std::string image_location_;

    std::string boot_class_path_string_;
    std::string class_path_string_;
    std::vector<std::string> properties_;

    std::list<void *> agents_;
    std::vector<void *> plugins_;

    // The default stack size for managed threads created by the runtime.
    size_t default_stack_size_;

    void *heap_;
};

struct PartialRuntimeR {
    void *heap_;

    void *jit_arena_pool_;
    void *arena_pool_;
    // Special low 4gb pool for compiler linear alloc. We need ArtFields to be in low 4gb if we are
    // compiling using a 32 bit image on a 64 bit compiler in case we resolve things in the image
    // since the field arrays are int arrays in this case.
    void *low_4gb_arena_pool_;

    // Shared linear alloc for now.
    void *linear_alloc_;

    // The number of spins that are done before thread suspension is used to forcibly inflate.
    size_t max_spins_before_thin_lock_inflation_;
    void *monitor_list_;
    void *monitor_pool_;

    void *thread_list_;

    void *intern_table_;

    void *class_linker_;

    void *signal_catcher_;

    void *jni_id_manager_;

    void *java_vm_;

    void *jit_;
    void *jit_code_cache_;
};

struct JavaVMExt {
    void *functions;
    void *runtime;
};

class FetchStackTraceVisitorR;

class ArtHelper {

public:
    static void init(JNIEnv *env, int);

    static void StackVisitorWalkStack(StackVisitor *visitor, bool include_transitions);

    static void *SuspendThreadByPeer(jobject peer, SuspendReason suspendReason,
                                     bool *timed_out);

    static void *SuspendThreadByThreadId(uint32_t threadId, SuspendReason suspendReason, bool *timed_out);


    static bool Resume(void *thread, SuspendReason suspendReason);

    static void *getThreadList();

    static void initMethods();

    static std::string PrettyMethod(void *art_method, bool with_signature);


private:
    static void *runtime_instance_;
    static int api;

    ArtHelper() {}

};


#endif