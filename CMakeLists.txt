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

# The option CMAKE_CXX_STANDARD was introduced in 3.1
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

#Project name, along with language type
project(inch LANGUAGES CXX)

# Allow the user to specify where the data files are located
set(LOCAL_PATH "${CMAKE_SOURCE_DIR}" CACHE STRING "Full path to location of the data_files directory")
add_definitions(-DLOCAL_PATH=\"${LOCAL_PATH}\")

# Put all binaries in a common location
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Compile against c++11
set(CMAKE_CXX_STANDARD 11)
# We need c++11 so can't fallback
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Use -std=c++11 rather than -std=gnu++11
set(CMAKE_CXX_EXTENSIONS OFF)

# Use Debug as the default build type
if ( NOT CMAKE_BUILD_TYPE )
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Set the build type to use: Debug (default), Release, RelWithDebInfo, MinSizeRel" FORCE)
endif ()

# Locate all of the header files
# Keep list alphabetical
set(HEADER_DIR ${PROJECT_SOURCE_DIR}/include)
set(HEADERS
  ${HEADER_DIR}/createChart.h
  ${HEADER_DIR}/extractValue.h
  ${HEADER_DIR}/functions.h
  ${HEADER_DIR}/inputs.h
  ${HEADER_DIR}/nuclide.h
  ${HEADER_DIR}/partition.h
  )

# Locate all of the source files
# Keep list alphabetical
set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(SOURCES
  ${SOURCE_DIR}/checkFileExists.cpp
  ${SOURCE_DIR}/constructChart.cpp
  ${SOURCE_DIR}/convertFloatToExponent.cpp
  ${SOURCE_DIR}/convertIsomerEnergyToHuman.cpp
  ${SOURCE_DIR}/convertSecondsToHuman.cpp
  ${SOURCE_DIR}/convertSymbolToZ.cpp
  ${SOURCE_DIR}/convertZToSymbol.cpp
  ${SOURCE_DIR}/createChart.cpp
  ${SOURCE_DIR}/createDriplineFile.cpp
  ${SOURCE_DIR}/createEPSProlog.cpp
  ${SOURCE_DIR}/createSVGProlog.cpp
  ${SOURCE_DIR}/createTIKZProlog.cpp
  ${SOURCE_DIR}/displaySection.cpp
  ${SOURCE_DIR}/drawEPSDoubleDriplines.cpp
  ${SOURCE_DIR}/drawEPSDoubleNeutronDripline.cpp
  ${SOURCE_DIR}/drawEPSDoubleProtonDripline.cpp
  ${SOURCE_DIR}/drawEPSDripline.cpp
  ${SOURCE_DIR}/drawEPSGrid.cpp
  ${SOURCE_DIR}/drawEPSKey.cpp
  ${SOURCE_DIR}/drawEPSMagicNumbers.cpp
  ${SOURCE_DIR}/drawEPSRprocess.cpp
  ${SOURCE_DIR}/drawEPSSingleDriplines.cpp
  ${SOURCE_DIR}/drawEPSSingleNeutronDripline.cpp
  ${SOURCE_DIR}/drawEPSSingleProtonDripline.cpp
  ${SOURCE_DIR}/drawNuclei.cpp
  ${SOURCE_DIR}/inputs.cpp
  ${SOURCE_DIR}/main.cpp
  ${SOURCE_DIR}/nuclide.cpp
  ${SOURCE_DIR}/partition.cpp
  ${SOURCE_DIR}/populateInternalMassTable.cpp
  ${SOURCE_DIR}/readAME.cpp
  ${SOURCE_DIR}/readNUBASE.cpp
  ${SOURCE_DIR}/readOWN.cpp
  ${SOURCE_DIR}/readRProcessData.cpp
  ${SOURCE_DIR}/setColours.cpp
  ${SOURCE_DIR}/setEPSKeyText.cpp
  ${SOURCE_DIR}/setIsotopeAttributes.cpp
  ${SOURCE_DIR}/setKeyScale.cpp
  ${SOURCE_DIR}/setNeutronLimits.cpp
  ${SOURCE_DIR}/validateInputFile.cpp
  ${SOURCE_DIR}/writeChart.cpp
  ${SOURCE_DIR}/writeEPS.cpp
  ${SOURCE_DIR}/writeSVG.cpp
  ${SOURCE_DIR}/writeTIKZ.cpp
  )


# Set the executable to depend on the sources and headers
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Where are the header files
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)

# Additional flags per build type
# Lets be as strict as possible
set(MY_DEBUG_OPTIONS -Wall -Wextra -pedantic-errors)
# Do we need additional release flags? Set up like we do
set(MY_RELEASE_OPTIONS )

target_compile_options(${PROJECT_NAME}
  PUBLIC "$<$<CONFIG:DEBUG>:${MY_DEBUG_OPTIONS}>"
  PUBLIC "$<$<CONFIG:DEBUG>:${MY_RELEASE_OPTIONS}>")

# Setup an install target
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
