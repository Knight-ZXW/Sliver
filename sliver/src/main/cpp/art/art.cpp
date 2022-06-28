#include "art.h"
#include <android/log.h>
#include "../sliver_log.h"




void *getAndroidRuntime(JNIEnv *env) {
    JavaVM *javaVm;
    env->GetJavaVM(&javaVm);
    auto *javaVmExt = (JavaVMExt *) javaVm;
    void *runtime = javaVmExt->runtime;
    return runtime;
}