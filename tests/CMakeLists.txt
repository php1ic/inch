set(INCH_TEST_NAME UnitTests)

set(TEST_SOURCES
  main.cpp
  chart_test.cpp
  converter_test.cpp
  dripline_test.cpp
  grid_test.cpp
  io_test.cpp
  key_test.cpp
  magicNumbers_test.cpp
  massTable_test.cpp
  nuclide_test.cpp
  options_test.cpp
  partition_test.cpp
  prolog_test.cpp
  rProcess_test.cpp
  ui_test.cpp
  )

set(IMPLEMENTATION_SOURCES ${SOURCES})
list(REMOVE_ITEM IMPLEMENTATION_SOURCES ${PROJECT_SOURCE_DIR}/src/main.cpp)

add_executable(${INCH_TEST_NAME} ${TEST_SOURCES} ${IMPLEMENTATION_SOURCES})

# Where are the header files
target_include_directories(${INCH_TEST_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include/)

target_link_libraries(
  ${INCH_TEST_NAME}
  PRIVATE
  Catch2::Catch2
  fmt::fmt-header-only
  project_warnings
  project_options
  # Need to link the filesystem library in older compiler versions
  # But not for MSVC
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:stdc++fs>
  )

add_test(
  NAME allTests
  COMMAND ${INCH_TEST_NAME}
  )
