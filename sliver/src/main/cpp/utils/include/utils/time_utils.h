//
// Created by knight-zxw on 2023/1/2.
// Email: nimdanoob@163.com
//
#pragma once
#include <cstdint>
#include <time.h>

namespace kb{

enum TimeUnit {
  kTimeUnitNanosecond,
  kTimeUnitMicrosecond,
  kTimeUnitMillisecond,
  kTimeUnitSecond,
};

// Constants for common time periods.
constexpr unsigned int kOneMinuteInSeconds = 60;
constexpr unsigned int kOneHourInSeconds = 60 * kOneMinuteInSeconds;

// Returns the monotonic time since some unspecified starting point in milliseconds.
uint64_t MilliTime();

// Returns the monotonic time since some unspecified starting point in microseconds.
uint64_t MicroTime();
}
