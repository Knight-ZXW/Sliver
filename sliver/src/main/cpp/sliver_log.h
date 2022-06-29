#ifndef SLIVER_LOG
#define SLIVER_LOG
#include "android/log.h"

#define LOGV(...)  ((void)__android_log_print(ANDROID_LOG_ERROR, "Sliver", __VA_ARGS__))


#endif