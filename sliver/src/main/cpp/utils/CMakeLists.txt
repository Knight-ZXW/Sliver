CMAKE_MINIMUM_REQUIRED(VERSION 3.6.0)

PROJECT(utils)

SET(TARGET utils)

SET(
        ${TARGET}_SOURCE_FILES

        ${CMAKE_CURRENT_SOURCE_DIR}/threads.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/time_utils.cpp

)

ADD_LIBRARY(
        ${TARGET}
        STATIC
        ${${TARGET}_SOURCE_FILES}
)

TARGET_INCLUDE_DIRECTORIES(
        ${TARGET}
        PUBLIC include
)

FIND_LIBRARY(log-lib log)

TARGET_LINK_LIBRARIES(
        ${TARGET}
        PUBLIC ${log-lib}
)
