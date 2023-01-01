#pragma once

#include <stdint.h>

#include <string>

namespace bytedance {
namespace utils {

uint32_t GetThreadId();
bool GetCurrentThreadName(std::string* name);
void SetThreadName(const char* name);

}  // namespace utils
}  // namespace bytedance