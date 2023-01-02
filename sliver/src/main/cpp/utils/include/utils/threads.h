#pragma once

#include <stdint.h>

#include <string>

namespace kb {
uint32_t GetThreadId();
bool GetCurrentThreadName(std::string *name);
void SetThreadName(const char *name);
}  // namespace kb