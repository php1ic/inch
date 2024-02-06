# The project knows Catch2 exits, but include this file to access catch_discover_tests()
include(${PROJECT_SOURCE_DIR}/external/Catch2/extras/Catch.cmake)

# Print the version of catch for info
get_directory_property(
  CATCH_VERSION
  DIRECTORY "${PROJECT_SOURCE_DIR}/external/Catch2"
  DEFINITION PROJECT_VERSION
  )

message(STATUS "Catch2 version: ${CATCH_VERSION}")

# Give the full test executable a name (not the project name)
set(INCH_TEST_NAME UnitTests)

# Alphabetical list of all the test source files
set(TEST_SOURCES
  chart_test.cpp
  converter_test.cpp
  dripline_test.cpp
  eps_chart_test.cpp
  eps_dripline_test.cpp
  eps_grid_test.cpp
  eps_key_test.cpp
  eps_magicNumbers_test.cpp
  eps_rProcess_test.cpp
  io_test.cpp
  key_test.cpp
  limits_test.cpp
  massTable_test.cpp
  nuclide_test.cpp
  options_test.cpp
  partition_test.cpp
  rProcess_test.cpp
  svg_chart_test.cpp
  tikz_chart_test.cpp
  ui_test.cpp
  )

# Create the tests, referencing the already compiled implementations objects
add_executable(${INCH_TEST_NAME} ${TEST_SOURCES} $<TARGET_OBJECTS:${SOURCE_OBJECTS}> )

# Where are the header files
target_include_directories(${INCH_TEST_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include/)

target_link_libraries(
  ${INCH_TEST_NAME}
  PRIVATE
  # ${PROJECT_NAME}
  Catch2::Catch2WithMain
  project_warnings
  project_options
  )

catch_discover_tests(
  ${INCH_TEST_NAME}
  )
