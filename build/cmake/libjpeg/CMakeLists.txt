cmake_minimum_required(VERSION 2.8)

SET(LIB_NAME "libjpeg")

project(${LIB_NAME} C)

SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR}/$(TERRALIB_DIR)/src/libjpeg)

file(GLOB SRCS ${ROOT}/*.c)


# these files are present but are not used
list(REMOVE_ITEM SRCS
  ${ROOT}/ansi2knr.c
  ${ROOT}/ckconfig.c
  ${ROOT}/jmemansi.c
  ${ROOT}/jmemname.c
  ${ROOT}/jpegtran.c
  ${ROOT}/rdjpgcom.c
  ${ROOT}/wrjpgcom.c
)

include_directories(${ROOT})

add_library(${LIB_NAME} SHARED ${SRCS} )
