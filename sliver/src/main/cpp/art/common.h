//
// Created by Administrator on 2023/5/10.
//

#pragma once
#include <android/log.h>
#include <android/api-level.h>
#ifdef __LP64__
const char *const kSystemLibDir = "/system/lib64/";
const char *const kApexLibDir = "/apex/com.android.runtime/lib64/";
const char *const kApexArtNsLibDir = "/apex/com.android.art/lib64/";
#else
const char *const kSystemLibDir = "/system/lib/";
const char *const kApexLibDir = "/apex/com.android.runtime/lib/";
const char *const kApexArtNsLibDir = "/apex/com.android.art/lib/";
#endif

static inline int32_t getAndroidApiLevel() {
  static int api_level = -1;
  if (api_level < 0) {
    api_level = android_get_device_api_level();
  }
  return api_level;
}



static const char* getLibArtPath(){
  static const char* artPath = nullptr;
  if (artPath != nullptr){
    return artPath;
  }
  int level = getAndroidApiLevel();
  const char *so_prefix = nullptr;
  if (level >= __ANDROID_API_R__) {
    so_prefix = kApexArtNsLibDir;
  } else if (level == __ANDROID_API_Q__) {
    so_prefix = kApexLibDir;
  } else {
    so_prefix = kSystemLibDir;
  }
  const char *art_name = "libart.so";
  size_t len = strlen(so_prefix) + strlen(art_name) + 1;
  artPath = static_cast<const char*>(malloc(len)); // 在堆上分配内存
  sprintf(const_cast<char*>(artPath), "%s%s", so_prefix, art_name);  // 将so_prefix和art_name的内容拼接到artPath指向的内存空间中
  return artPath;
}