//
// Created by knight-zxw on 2023/1/2.
// Email: nimdanoob@163.com
//

#include <bits/timespec.h>
#include "utils/time_utils.h"
namespace kb{



uint64_t MicroTime(){
  timespec now{};
  clock_gettime(CLOCK_MONOTONIC, &now);
  return static_cast<uint64_t>(now.tv_sec) * UINT64_C(1000000) + now.tv_nsec / UINT64_C(1000);
}
uint64_t MilliTime() {
  timespec now{};
  clock_gettime(CLOCK_MONOTONIC, &now);
  return static_cast<uint64_t>(now.tv_sec) * UINT64_C(1000) + now.tv_nsec / UINT64_C(1000000);
}
}