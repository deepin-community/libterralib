cmake_minimum_required(VERSION 2.8)

SET(LIB_NAME "terralibdtm")

project(${LIB_NAME})

SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR}/$(TERRALIB_DIR)/src/terralibdtm)

file(GLOB SRCS ${ROOT}/*.cpp)

include_directories(${ROOT})

add_library(${LIB_NAME} SHARED ${SRCS} )
