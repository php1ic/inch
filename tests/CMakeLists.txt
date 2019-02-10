set(INCH_TEST_NAME UnitTests)

add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${PROJECT_SOURCE_DIR}/external/)

set(TEST_SOURCE_DIR ${PROJECT_SOURCE_DIR}/tests)
set(TEST_SOURCES
  ${TEST_SOURCE_DIR}/main.cpp
  ${TEST_SOURCE_DIR}/converter_test.cpp
  )

set(IMPLEMENTATION_SOURCES ${SOURCES})
list(REMOVE_ITEM IMPLEMENTATION_SOURCES ${PROJECT_SOURCE_DIR}/src/main.cpp)

add_executable(${INCH_TEST_NAME} ${TEST_SOURCES} ${IMPLEMENTATION_SOURCES})

# Where are the header files
target_include_directories(${INCH_TEST_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(${INCH_TEST_NAME} Catch)

add_test(
  NAME allTests
  COMMAND ${INCH_TEST_NAME}
  )
