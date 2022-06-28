#ifndef SLIVER_ART_HELPER
#define SLIVER_ART_HELPER

#include <jni.h>
#include "../sliver_log.h"

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

class ARTHelper {

private:
    static  ARTHelper* instance_;
public:

    static int init(JNIEnv *env);
    static ARTHelper* INSTANCE(){
        return instance_;
    };
    void Resume(void *thread_list, void *thread, int mode);

};


#endif