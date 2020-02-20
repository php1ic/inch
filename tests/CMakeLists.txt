set(INCH_TEST_NAME UnitTests)

set(TEST_SOURCE_DIR ${PROJECT_SOURCE_DIR}/tests)
set(TEST_SOURCES
  ${TEST_SOURCE_DIR}/main.cpp
  ${TEST_SOURCE_DIR}/converter_test.cpp
  ${TEST_SOURCE_DIR}/dripline_test.cpp
  ${TEST_SOURCE_DIR}/grid_test.cpp
  ${TEST_SOURCE_DIR}/io_test.cpp
  ${TEST_SOURCE_DIR}/nuclide_test.cpp
  )

set(IMPLEMENTATION_SOURCES ${SOURCES})
list(REMOVE_ITEM IMPLEMENTATION_SOURCES ${PROJECT_SOURCE_DIR}/src/main.cpp)

add_executable(${INCH_TEST_NAME} ${TEST_SOURCES} ${IMPLEMENTATION_SOURCES})

# Where are the header files
target_include_directories(${INCH_TEST_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include/)

target_link_libraries(${INCH_TEST_NAME} Catch2::Catch2)

if ( NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" )
  target_link_libraries(${INCH_TEST_NAME} stdc++fs)
endif()

add_test(
  NAME allTests
  COMMAND ${INCH_TEST_NAME}
  )
