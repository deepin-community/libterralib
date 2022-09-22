cmake_minimum_required(VERSION 2.8)

SET(LIB_NAME "stat")

project(${LIB_NAME})

# dependencies
add_subdirectory(../libjpeg ./build-of-libjpeg)

SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR}/$(TERRALIB_DIR)/src/terralib/stat)

file(GLOB SRCS ${ROOT}/*.cpp)

include_directories(
  ../../src/terralib/kernel
)

# these files are present but are not used
list(REMOVE_ITEM SRCS
  ${ROOT}/TeMSVFactory.cpp
  ${ROOT}/TeSemivarModelFactory.cpp
  ${ROOT}/TeTreeMinimum.cpp
  ${ROOT}/hash.cpp
  ${ROOT}/mtrand.cpp
)

include_directories(${ROOT})

add_library(${LIB_NAME} SHARED ${SRCS} )
target_link_libraries(${LIB_NAME} libjpeg)
