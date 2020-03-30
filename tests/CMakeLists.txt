set(INCH_TEST_NAME UnitTests)

set(TEST_SOURCES
  main.cpp
  converter_test.cpp
  dripline_test.cpp
  grid_test.cpp
  io_test.cpp
  nuclide_test.cpp
  )

set(IMPLEMENTATION_SOURCES ${SOURCES})
list(REMOVE_ITEM IMPLEMENTATION_SOURCES ${PROJECT_SOURCE_DIR}/src/main.cpp)

add_executable(${INCH_TEST_NAME} ${TEST_SOURCES} ${IMPLEMENTATION_SOURCES})

# Where are the header files
target_include_directories(${INCH_TEST_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include/)

target_link_libraries(
  ${INCH_TEST_NAME}
  Catch2::Catch2
  fmt::fmt-header-only
  coverage_config
  )

# Need to link the filesystem library in older compiler versions
# But not for MSVC
if ( NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" )
  target_link_libraries(${INCH_TEST_NAME} stdc++fs)
endif()

add_test(
  NAME allTests
  COMMAND ${INCH_TEST_NAME}
  )
