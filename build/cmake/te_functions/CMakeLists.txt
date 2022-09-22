cmake_minimum_required(VERSION 2.8)

SET(LIB_NAME "te_function")

project(${LIB_NAME})

SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR}/$(TERRALIB_DIR)/src/te_function)

file(GLOB SRCS ${ROOT}/*.cpp)

include_directories(${ROOT})

add_library(${LIB_NAME} SHARED ${SRCS} )
