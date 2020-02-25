# Require (i.e. demand) an out-of-source build
if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
  message("")
  message(STATUS "Please use an out-of-source build, it's neater.")
  message(STATUS "Don't forget to clean up ${CMAKE_BINARY_DIR} by removing:")
  message(STATUS "\tCMakeCache.txt")
  message(STATUS "\tCMakeFiles")
  message(STATUS "Then you can create a separate directory and re-run cmake from there.\n")
  message(FATAL_ERROR "In-source build attempt detected")
endif()

# CheclIPOSupported was introduced in 3.9
cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

# Project name, along with language type
project(
  inch
  #HOMEPAGE_URL "https://github.com/php1ic/inch"
  LANGUAGES CXX
  VERSION 0.9.8
  )

# Output compilation database that can be used by analysis tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Put all binaries in a common location
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Compile against c++17
set(CMAKE_CXX_STANDARD 17)
# We need c++17 so can't fallback
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Use -std=c++?? rather than -std=gnu++??
set(CMAKE_CXX_EXTENSIONS OFF)

# Use Debug as the default build type
if ( NOT CMAKE_BUILD_TYPE )
  set(
    CMAKE_BUILD_TYPE "RelWithDebInfo"
    CACHE STRING "Set the build type to use: Debug, Release, RelWithDebInfo (default), MinSizeRel" FORCE
    )
endif ()

find_program(CCACHE ccache)
if(CCACHE)
  message(STATUS "Using ccache")
  set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
else()
  message(STATUS "ccache not found cannot use")
endif()

# Locate all of the header files
# Keep list alphabetical
set(HEADER_DIR ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME})
set(HEADERS
  ${HEADER_DIR}/chart.hpp
  ${HEADER_DIR}/chartColour.hpp
  ${HEADER_DIR}/chartSelection.hpp
  ${HEADER_DIR}/chartType.hpp
  ${HEADER_DIR}/converter.hpp
  ${HEADER_DIR}/dripline.hpp
  ${HEADER_DIR}/fileType.hpp
  ${HEADER_DIR}/grid.hpp
  ${HEADER_DIR}/io.hpp
  ${HEADER_DIR}/key.hpp
  ${HEADER_DIR}/limits.hpp
  ${HEADER_DIR}/magicNumbers.hpp
  ${HEADER_DIR}/massTable.hpp
  ${HEADER_DIR}/nuclide.hpp
  ${HEADER_DIR}/options.hpp
  ${HEADER_DIR}/partition.hpp
  ${HEADER_DIR}/prolog.hpp
  ${HEADER_DIR}/rProcess.hpp
  ${HEADER_DIR}/ui.hpp
  ${HEADER_DIR}/version.hpp
  )

configure_file (
  "${HEADER_DIR}/version.hpp.in"
  "${HEADER_DIR}/version.hpp"
  @ONLY
  )

# Locate all of the source files
# Keep list alphabetical
set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(SOURCES
  ${SOURCE_DIR}/chart.cpp
  ${SOURCE_DIR}/converter.cpp
  ${SOURCE_DIR}/dripline.cpp
  ${SOURCE_DIR}/grid.cpp
  ${SOURCE_DIR}/io.cpp
  ${SOURCE_DIR}/key.cpp
  ${SOURCE_DIR}/main.cpp
  ${SOURCE_DIR}/massTable.cpp
  ${SOURCE_DIR}/magicNumbers.cpp
  ${SOURCE_DIR}/nuclide.cpp
  ${SOURCE_DIR}/options.cpp
  ${SOURCE_DIR}/partition.cpp
  ${SOURCE_DIR}/prolog.cpp
  ${SOURCE_DIR}/rProcess.cpp
  ${SOURCE_DIR}/ui.cpp
  )


# Set the executable to depend on the sources and headers
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Where are the header files
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include/)

# Need to link the filesystem library in older compiler versions
# But not for MSVC
if ( NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" )
  target_link_libraries(${PROJECT_NAME} PUBLIC stdc++fs)
endif()

include(CheckIPOSupported)
check_ipo_supported(RESULT IPOsupported OUTPUT error)

if ( IPOsupported )
  message(STATUS "Use IPO/LTO")
  set_property(TARGET ${PROJECT_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
  message(STATUS "IPO/LTO not supported: <${error}>")
endif()


# Empty list of flags we can append to
set(INCH_DEBUG_FLAGS )

# I don't currently have access to a windows machine to test these,
# so rather than trial and error pushing to trigger appveyor and check
# for errors, leave it at this minimal list.
if ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  list(APPEND INCH_DEBUG_FLAGS
    /permissive
    /W4
    )
else()
  list(APPEND INCH_DEBUG_FLAGS
    -Werror
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wnon-virtual-dtor
    -Woverloaded-virtual
    -Wnull-dereference
    -Wold-style-cast
    -Wcast-align
    -Wfloat-equal
    -Wunused
    #-Wconversion # STL containers uses size_t so this gives lots of warnings
    #-Wsign-conversion # STL containers uses size_t so this gives lots of warnings
    )

  # Clang doesn't have the exact same flags as gcc
  if ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )
    list(APPEND INCH_DEBUG_FLAGS
      -Wmisleading-indentation
      -Wduplicated-branches
      -Wduplicated-cond
      -Wlogical-op
      -Wuseless-cast
      )
  endif()
endif()

# Do we need separate Release flags, or just the same as Debug?
set(INCH_RELEASE_FLAGS )

target_compile_options(
  ${PROJECT_NAME}
  PUBLIC "$<$<CONFIG:DEBUG>:${INCH_DEBUG_FLAGS}>"
  PUBLIC "$<$<CONFIG:RELEASE>:${INCH_RELEASE_FLAGS}>"
  )


# Use the {fmt} library. It will be part of C++20
add_subdirectory(external/fmt)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt-header-only)

# Code Coverage Configuration
add_library(coverage_config INTERFACE)

option(INCH_CODE_COVERAGE "Enable coverage reporting" OFF)
if(INCH_CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  message(STATUS "Checking code coverage")
  # Add required flags (GCC & LLVM/Clang)
  target_compile_options(coverage_config INTERFACE
    -O0        # no optimization
    -g         # generate debug info
    --coverage # sets all required flags
  )
  if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.13)
    target_link_options(coverage_config INTERFACE --coverage)
  else()
    target_link_libraries(coverage_config INTERFACE --coverage)
  endif()
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC coverage_config)

# Unit testing wit Catch2
option(INCH_UNIT_TESTS "Build unit tests" ON)
if(INCH_UNIT_TESTS)
  enable_testing()
  add_subdirectory(external/Catch2)
  add_subdirectory(tests)
endif()

# Documentation with Doxygen, but only in release mode
option(INCH_BUILD_DOCS "Build documentation with Doxygen" ON)
if(INCH_BUILD_DOCS)
  if(${CMAKE_BUILD_TYPE} MATCHES "Release")
    add_subdirectory(docs)
  else()
    message(STATUS "Documentation is only done in Release builds")
  endif()
endif()

# Setup an install target
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
