cmake_minimum_required(VERSION 2.8)

SET(LIB_NAME "qwt")

project(${LIB_NAME})

SET(ROOT ${CMAKE_CURRENT_SOURCE_DIR}/$(TERRALIB_DIR)/src/qwt)

find_package(Qt3 REQUIRED)

file(GLOB SRCS ${ROOT}/*.cpp)
file(GLOB HDRS ${ROOT}/include/*.h)

# note: according to the .pro file the following files can be exculded if
# you're interested in the plot widget only:
# qwt_abstract_slider.cpp
# qwt_abstract_scale.cpp
# qwt_arrow_button.cpp
# qwt_analog_clock.cpp
# qwt_compass.cpp
# qwt_compass_rose.cpp
# qwt_counter.cpp
# qwt_dial.cpp
# qwt_dial_needle.cpp
# qwt_double_range.cpp
# qwt_knob.cpp
# qwt_slider.cpp
# qwt_thermo.cpp
# qwt_wheel.cpp

qt_wrap_cpp(${LIB_NAME} SRCS ${HDRS})

include_directories(${ROOT}/include)

add_library(${LIB_NAME} SHARED ${SRCS} )
