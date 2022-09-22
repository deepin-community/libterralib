cmake_minimum_required(VERSION 2.8)

SET(LIB_NAME "shapelib")

project(${LIB_NAME} C)

SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR}/$(TERRALIB_DIR)/src/shapelib)

file(GLOB SRCS ${ROOT}/*.c)

include_directories(${ROOT})

add_library(${LIB_NAME} SHARED ${SRCS} )
