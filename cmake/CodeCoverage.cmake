function(enable_codecoverage project_name)
  option(INCH_CODE_COVERAGE "Enable coverage reporting" OFF)

  add_library(coverage_config INTERFACE)

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

  target_link_libraries(project_options INTERFACE coverage_config)

endfunction(enable_codecoverage)
