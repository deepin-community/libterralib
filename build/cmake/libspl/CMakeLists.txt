cmake_minimum_required(VERSION 2.8)

SET(LIB_NAME "libspl")

project(${LIB_NAME})

SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR}/$(TERRALIB_DIR)/src/libspl)

file(GLOB SRCS ${ROOT}/source/*.cpp)

include_directories(${ROOT}/include)

add_library(${LIB_NAME} SHARED ${SRCS} )
