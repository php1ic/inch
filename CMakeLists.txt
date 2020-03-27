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

# CheckIPOSupported was introduced in 3.9
cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

# Project name, along with language type
project(
  inch
  #HOMEPAGE_URL "https://github.com/php1ic/inch"
  LANGUAGES CXX
  VERSION 0.9.8
  )

# Put all binaries in a common location
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add the 'cmake' folder to the list of paths to check for modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include(GeneralProjectSettings)

add_subdirectory(include/${PROJECT_NAME})

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

# Set the executable to depend on the sources
add_executable(${PROJECT_NAME} ${SOURCES})

# Where are the header files
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include/)

# Need to link the filesystem library in older compiler versions
# But not for MSVC
if ( NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" )
  target_link_libraries(${PROJECT_NAME} PUBLIC stdc++fs)
endif()

# Use the {fmt} library. It will be part of C++20
add_subdirectory(external/fmt)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt-header-only)

include(CompilerWarnings)
set_project_warnings(PROJECT_WARNINGS)

target_compile_options(${PROJECT_NAME} PUBLIC ${PROJECT_WARNINGS})

include(Doxygen)
enable_doxygen()

# I know this doesn't work, but it will!
# Code Coverage Configuration
#include(CodeCoverage)
#enable_codecoverage()

# Unit testing wit Catch2
option(INCH_UNIT_TESTS "Build unit tests" ON)
if(INCH_UNIT_TESTS)
  enable_testing()
  add_subdirectory(external/Catch2)
  add_subdirectory(tests)
endif()

# Setup an install target
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
