cmake_minimum_required(VERSION 2.8)

project(tiff C)

SET(TIFF_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/$(TERRALIB_DIR)/src/tiff)

file(GLOB SRCS ${TIFF_ROOT}/*.c)

# don't know much about this file, was not used by the qmake build system and
# would cause duplicate symbols
list(REMOVE_ITEM SRCS ${TIFF_ROOT}/fax3sm.c)

# only keep the tif_OS.c version we need for this OS target:
IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  list(REMOVE_ITEM SRCS ${TIFF_ROOT}/tif_win32.c)
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Window")
  list(REMOVE_ITEM SRCS ${TIFF_ROOT}/tif_unix.c)
ELSE()
  MESSAGE(FATAL_ERROR "OS not yet supported? Which tif_* version should be used?")
ENDIF()

include_directories(${TIFF_ROOT})

add_library(tiff SHARED ${SRCS} )
