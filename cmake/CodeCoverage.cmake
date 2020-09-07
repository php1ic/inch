function(enable_codecoverage project_name)
  option(INCH_CODE_COVERAGE "Enable coverage reporting" OFF)

  if(INCH_CODE_COVERAGE)
    add_library(coverage_config INTERFACE)

    if(INCH_CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
      message(STATUS "[code-coverage] Measure coverage metrics")

      # Add required flags (GCC & LLVM/Clang)
      target_compile_options(coverage_config
        INTERFACE
        -O0        # no optimization
        -g         # generate debug info
        --coverage # sets all required flags
        )

      # Current required version is 3.12 so need this check
      # Remove once required version >=3.13 or the main distros that version or higher in their repos
      if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.13)
        target_link_options(coverage_config INTERFACE --coverage)
      else()
        target_link_libraries(coverage_config INTERFACE --coverage)
      endif()
    endif()

    target_link_libraries(${project_name} INTERFACE coverage_config)
  else()
    message(STATUS "[code-coverage] Not measuring metrics")
  endif()

endfunction(enable_codecoverage)
