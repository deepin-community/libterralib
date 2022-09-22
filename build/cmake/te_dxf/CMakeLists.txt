cmake_minimum_required(VERSION 2.8)

SET(LIB_NAME "te_dxfDXF")

project(${LIB_NAME})

SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR}/$(TERRALIB_DIR)/src/terralib/drivers/DXF)

file(GLOB SRCS ${ROOT}/*.cpp)

include_directories(
  ${ROOT}
  ../../src/terralib/kernel
  ../../src/dxflib
)

add_library(${LIB_NAME} SHARED ${SRCS} )
