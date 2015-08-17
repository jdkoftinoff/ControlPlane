cmake_minimum_required (VERSION 2.8)
set(PROJECT ControlPlane)
project(${PROJECT} C CXX)

set(CXX11 "ON")
set(PROJECT_INCLUDES_GLOBS "include/*.hpp" "include/ControlPlane/*.hpp" "include/ControlPlane/Descriptor/*.hpp" )

if( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
    set(LIBS ${LIBS} m rt)
endif()

INCLUDE (common.cmake)

