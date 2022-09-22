cmake_minimum_required(VERSION 2.8)

SET(LIB_NAME "terralibpdi")

project(${LIB_NAME})

SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR}/$(TERRALIB_DIR)/src/terralib/image_processing)

file(GLOB SRCS ${ROOT}/*.cpp)

# these files are present but are not used
list(REMOVE_ITEM SRCS
  ${ROOT}/TePDIRasterRUBuffer.cpp
)

include_directories(
  ${ROOT}
  $(TERRALIB_DIR)/src/terralib/kernel
)

add_library(${LIB_NAME} SHARED ${SRCS} )
target_link_libraries(${LIB_NAME})
