#include <jni.h>
#include "art.h"
#include "fetch_stack_visitor.cpp"
#include "vector"
#include <chrono>
#include "utils/time_utils.h"
using namespace kbArt;
using namespace kb;

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
  JNIEnv *env = nullptr;
  if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
    return -1;
  }
  ArtHelper::init(env);
//  LOGE("sliver","init success");
  return JNI_VERSION_1_6;
}

extern "C"
JNIEXPORT jlongArray JNICALL
Java_com_knightboost_sliver_Sliver_nativeGetMethodStackTrace(JNIEnv *env,
                                                             jclass clazz,
                                                             jobject threadPeer,
                                                             jlong native_peer) {

  auto *tid_p = reinterpret_cast<uint32_t *>(native_peer + THREAD_ID_OFFSET);
  bool timeOut;
  void *thread = ArtHelper::SuspendThreadByThreadId(*tid_p,
                                                    SuspendReason::kForUserCode,
                                                    &timeOut);
  std::vector<std::uintptr_t> stack_methods;
  auto f = [](ArtMethod *method, void *visitorData) -> bool {
    auto *methods = reinterpret_cast<std::vector<std::uintptr_t> *>(visitorData);
    methods->push_back(reinterpret_cast<std::uintptr_t>(&*method));
    return true;
  };

  FetchStackVisitor visitor(thread, &stack_methods, f);
  ArtHelper::StackVisitorWalkStack(&visitor, false);
  ArtHelper::Resume(thread, SuspendReason::kForUserCode);

  std::vector<double> results(4);
  jdoubleArray output = env->NewDoubleArray(4);
  env->SetDoubleArrayRegion(output, 0, 4, &results[0]);

  jlongArray addresses = env->NewLongArray((jsize) stack_methods.size());

  jlong *destElements = env->GetLongArrayElements(addresses, nullptr);
  for (int i = 0; i < stack_methods.size(); i++) {
    destElements[i] =(jlong) stack_methods[i];
  }
  env->ReleaseLongArrayElements(addresses, destElements, 0);
  return addresses;

}

extern "C"
JNIEXPORT jobjectArray JNICALL
Java_com_knightboost_sliver_Sliver_prettyMethods(JNIEnv *env, jclass clazz, jlongArray methods) {
  jlong *methods_ptr = env->GetLongArrayElements(methods, nullptr);
  jsize size = env->GetArrayLength(methods);
  jobjectArray ret = env->NewObjectArray(size, env->FindClass("java/lang/String"),
                                         nullptr);

  for (int i = 0; i < size; i++) {
    const std::string
        &pretty_method = ArtHelper::PrettyMethod(reinterpret_cast<void *>(methods_ptr[i]), true);
    pretty_method.c_str();
    jstring frame = env->NewStringUTF(pretty_method.c_str());
    env->SetObjectArrayElement(ret, i, frame);
    env->DeleteLocalRef(frame);
  }
  return ret;

}