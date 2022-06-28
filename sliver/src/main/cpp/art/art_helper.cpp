#include "art_helper.h"
#include "art.h"
#include "../sliver_log.h"
#include <vector>
#include <string>
#include <stdlib.h>
#include <sys/system_properties.h>
#include <jni.h>


int ARTHelper::init(JNIEnv *env) {
    JavaVM *javaVm;
    env->GetJavaVM(&javaVm);
    auto *javaVMExt = (JavaVMExt *) javaVm;
    void *runtime = javaVMExt->runtime;
    LOGV("runtime ptr: %p, vmExtPtr: %p", runtime, javaVMExt);

    const int MAX = 2000;
    int offsetOfVmExt = findOffset(runtime, 0, MAX, (size_t) javaVMExt);
    LOGV("offsetOfVmExt: %d", offsetOfVmExt);

    if (offsetOfVmExt < 0) {
        return -1;
    }



    return 0;


}

void ARTHelper::Resume(void *thread_list, void *thread, int mode) {

}
