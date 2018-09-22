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

# The option CMAKE_CEXPORT_COMPILE_COMMANDS was introduced in 3.5.2
cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

# Project name, along with language type
project(
  inch
  LANGUAGES CXX
  VERSION 0.9.7
  )

# Output compilation database that can be used by analysis tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Put all binaries in a common location
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Compile against c++14
set(CMAKE_CXX_STANDARD 14)
# We need c++14 so can't fallback
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Use -std=c++?? rather than -std=gnu++??
set(CMAKE_CXX_EXTENSIONS OFF)

# Use Debug as the default build type
if ( NOT CMAKE_BUILD_TYPE )
  set(
    CMAKE_BUILD_TYPE "Debug"
    CACHE STRING "Set the build type to use: Debug (default), Release, RelWithDebInfo, MinSizeRel" FORCE
    )
endif ()

# Locate all of the header files
# Keep list alphabetical
set(HEADER_DIR ${PROJECT_SOURCE_DIR}/include)
set(HEADERS
  ${HEADER_DIR}/chart.hpp
  ${HEADER_DIR}/chartColour.hpp
  ${HEADER_DIR}/chartSelection.hpp
  ${HEADER_DIR}/chartType.hpp
  ${HEADER_DIR}/converter.hpp
  ${HEADER_DIR}/dripline.hpp
  ${HEADER_DIR}/fileType.hpp
  ${HEADER_DIR}/grid.hpp
  ${HEADER_DIR}/inputs.hpp
  ${HEADER_DIR}/key.hpp
  ${HEADER_DIR}/magicNumbers.hpp
  ${HEADER_DIR}/massTable.hpp
  ${HEADER_DIR}/nuclide.hpp
  ${HEADER_DIR}/partition.hpp
  ${HEADER_DIR}/prolog.hpp
  ${HEADER_DIR}/rProcess.hpp
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
  ${SOURCE_DIR}/inputs.cpp
  ${SOURCE_DIR}/key.cpp
  ${SOURCE_DIR}/main.cpp
  ${SOURCE_DIR}/massTable.cpp
  ${SOURCE_DIR}/magicNumbers.cpp
  ${SOURCE_DIR}/nuclide.cpp
  ${SOURCE_DIR}/partition.cpp
  ${SOURCE_DIR}/prolog.cpp
  ${SOURCE_DIR}/rProcess.cpp
  )


# Set the executable to depend on the sources and headers
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Where are the header files
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)

# Additional flags per build type
# Lets be as strict as possible
set(MY_DEBUG_OPTIONS
  -Werror
  -Wall
  -Wextra
  -pedantic-errors
  -Wshadow
  -Wnon-virtual-dtor
  -Woverloaded-virtual
  -Wnull-dereference
  -Wold-style-cast
  -Wfloat-equal
  #-Wduplicated-cond
  #-Wduplicated-branches
  #-Wlogical-op
  #-Wuseless-cast
  #-Wpadded
  #-Wconversion
  )
# Do we need additional release flags? Set up like we do
set(MY_RELEASE_OPTIONS )

target_compile_options(
  ${PROJECT_NAME}
  PUBLIC "$<$<CONFIG:DEBUG>:${MY_DEBUG_OPTIONS}>"
  PUBLIC "$<$<CONFIG:RELEASE>:${MY_RELEASE_OPTIONS}>"
  )

# Setup an install target
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
