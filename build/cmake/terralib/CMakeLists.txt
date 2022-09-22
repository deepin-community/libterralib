cmake_minimum_required(VERSION 2.8)

SET(LIB_NAME "terralib")

project(${LIB_NAME})

SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR}/$(TERRALIB_DIR)/src/terralib/kernel)

file(GLOB SRCS ${ROOT}/*.cpp)

list(APPEND SRCS
  ${ROOT}/../drivers/libjpeg/TeLibJpegWrapper.cpp
  ${ROOT}/../drivers/libjpeg/jmemdst.c
  ${ROOT}/../drivers/libjpeg/jmemsrc.c
)

# these files are present but are not used
list(REMOVE_ITEM SRCS
  ${ROOT}/TeBlockLoader.cpp
  ${ROOT}/TeConsoleErrorMessage.cpp
  ${ROOT}/TeCoverageImportUtils.cpp
  ${ROOT}/TeCoverageUtils.cpp
  ${ROOT}/TeViewTreeIterator.cpp
  ${ROOT}/TeViewTreeUtils.cpp
)

# TODO
# ${ROOT}/TeGraph.cpp not listed in .pro but still required? Is this the place to compile it?

include_directories(
  $(TERRALIB_DIR)/src/terralib/kernel

  # TODO: this must be separate projects ?
  $(TERRALIB_DIR)/src/tiff
  ${DRIVER_INCLUDES}
  $(TERRALIB_DIR)/src/terralib/drivers/libjpeg
)

add_library(${LIB_NAME} SHARED ${SRCS} )
