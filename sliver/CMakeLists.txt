
# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Declares and names the project.

project(sliver)

set(fastlog_DIR src/main/cpp/third_party/profilo/fastlog)
set(utils_DIR src/main/cpp/utils)

include_directories(${fastlog_DIR})
include_directories(${utils_DIR})

message("binary dir is ${CMAKE_CURRENT_BINARY_DIR}")
add_subdirectory(${fastlog_DIR} ${CMAKE_CURRENT_BINARY_DIR}/fastlog)

message("CMAKE_CURRENT_BINARY_DIR is ", ${CMAKE_CURRENT_BINARY_DIR})

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

include_directories(src/main/cpp)

add_subdirectory(${utils_DIR} ${CMAKE_CURRENT_BINARY_DIR}/utils)

aux_source_directory(src/main/cpp/art SRC_DIR_ART)
include_directories(src/main/cpp/art)

aux_source_directory(src/main/cpp/sliver SRC_DIR_Sliver)
include_directories(src/main/cpp/sliver)



add_library( # Sets the name of the library.
        sliver

        # Sets the library as a shared library.
        SHARED
        ${SRC_DIR_ART}
        ${SRC_DIR_Sliver}
        # Provides a relative path to your source file(s).
        src/main/cpp/sliver.cpp
        )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib
        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

find_package(bytehook REQUIRED CONFIG)
find_package(xdl REQUIRED CONFIG)
# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        sliver
        ${log-lib}
        utils
        fastlog
        xdl::xdl
        bytehook::bytehook)
