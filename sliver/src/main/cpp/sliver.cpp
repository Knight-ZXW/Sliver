#include <jni.h>
#include <string>
#include "android/log.h"
#include "sliver_log.h"
#include "fetch_stack_visitor.cpp"
#include "./art/art.h"
#include "art/art_thread.h"

static int api_level;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;

    LOGV("JNI_OnLoad");

    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }
    char api_level_str[5];
    __system_property_get("ro.build.version.sdk", api_level_str);
    api_level = atoi(api_level_str);
    LOGV("api level: %d", api_level);
    ArtHelper::init(env, api_level);
    ArtHelper::initMethods();
    return JNI_VERSION_1_6;
}


extern "C"
JNIEXPORT void JNICALL
Java_com_knightboost_sliver_NativeLib_traceStack(JNIEnv *env, jclass clazz,
                                                 jobject threadPeer,jlong native_peer) {

    bool timeOut;
//    LOGV("try suspend thread");
    auto *tid_p  = reinterpret_cast<uint32_t *>(native_peer + THREAD_ID_OFFSET);

    void *thread = ArtHelper::SuspendThreadByThreadId(*tid_p, SuspendReason::kForUserCode, &timeOut);
    //使用 SuspendThreadByPeer 的方式会出现崩溃，
    // 而如果在调用前 打印一下任意日志 就不会崩溃，不清楚是为什么
//    void *thread = ArtHelper::SuspendThreadByPeer(threadPeer, SuspendReason::kForUserCode, &timeOut);

//    LOGV("suspend Thread Success, begin to visitor stack");
    FetchStackTraceVisitorR visitor = FetchStackTraceVisitorR(thread, nullptr, nullptr);
    ArtHelper::StackVisitorWalkStack(&visitor, false);
    ArtHelper::Resume(thread, SuspendReason::kInternal);
//    LOGV("resume Thread %p, begin to visitor stack", &resumeResult);

}

