cmake_minimum_required(VERSION 3.10)
project(ShiftingVariables VERSION 1.0.0)

message(STATUS "")
message(STATUS "    == ${PROJECT_NAME} Project configuration ==")
message(STATUS "")
message(STATUS "Generator: ${CMAKE_GENERATOR}")

#------------------------------------------------------------------------------
# General settings

#
# Disallow in-tree building
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "Prevented in-tree built. Please avoid insource building")
endif(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})

#
# Force All binaries to output into the same directory
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output CACHE STRING "Library directory" FORCE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output CACHE STRING "Binary directory" FORCE)

#
# Be nice to visual studio
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#
# Be nice and export compile commands by default, this is handy for clang-tidy
# and for other tools.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#
# We can use include() and find_package() for our scripts in there
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

#------------------------------------------------------------------------------
# ShiftingVariablesLib

set(ShiftingVariablesLib_HEADERS include/implementation/shifting_variables.hpp include/shifting_variables.h include/shifting_config.h include/overloads/assignment/operator_assignment.h)

add_library(ShiftingVariablesLib INTERFACE)

target_include_directories(ShiftingVariablesLib INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

#------------------------------------------------------------------------------
# ShiftingVariablesTest

enable_testing()

add_executable(ShiftingVariablesTest test/main.cpp)

set_target_properties(ShiftingVariablesTest PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS YES
)

target_include_directories(ShiftingVariablesTest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_options(ShiftingVariablesTest PUBLIC -Wall -Wextra -Wpedantic -Werror)

target_link_libraries(ShiftingVariablesTest PUBLIC ShiftingVariablesLib pthread)

add_test(ShiftingVariablesTest ShiftingVariablesTest)

#-------------------------------------------------------------------------------
# Wrap up of settings printed on build
message(STATUS "")
message(STATUS "    == Final overview for ${PROJECT_NAME} ==")
message(STATUS "Version:              ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH} ${VERSION_TYPE} @ ${VERSION_HOST}")
message(STATUS "Install prefix:       ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Compiler:             ${CMAKE_CXX_COMPILER}")
message(STATUS "CMAKE_BUILD_TYPE:     ${CMAKE_BUILD_TYPE}")
message(STATUS "  possible options: Debug Release RelWithDebInfo MinSizeRel")
message(STATUS "  set with ` cmake -DCMAKE_BUILD_TYPE=Debug .. `")
message(STATUS "")
