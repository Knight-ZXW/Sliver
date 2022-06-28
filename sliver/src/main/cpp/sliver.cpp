#include <jni.h>
#include <string>
#include "android/log.h"
#include "sliver_log.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_knightboost_sliver_NativeLib_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}