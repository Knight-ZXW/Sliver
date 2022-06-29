#ifndef SLIVER_ART_THREAD
#define SLIVER_ART_THREAD
#include "art_macros.h"


//TODO 这里的偏移值 高低版本不一样， 目前只看了Android 12的,后面需要做下区分，版本兼容性
// Offset of field Thread::tls32_.thin_lock_thread_id
#define THREAD_ID_OFFSET 8
#define TID_OFFSET 12

#endif