function(enable_doxygen)
  option(INCH_BUILD_DOCS "Build documentation with Doxygen" OFF)
  if(INCH_BUILD_DOCS)
    find_package(Doxygen)

    if(NOT DOXYGEN_FOUND)
      message(ERROR " Doxygen is needed to build the documentation.")
      return()
    endif()

    # Don't document external libraries
    set(DOXYGEN_EXCLUDE_PATTERNS "*/external/*")

    message(STATUS "[Doxygen ${DOXYGEN_VERSION}] Building documentation")

    # We could exclude this target from 'all',
    # but if the user has turned this option on lets assume they want it.
    doxygen_add_docs(doxygen_docs
      WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
      COMMENT "Generating documentation with doxygen"
      )
  endif()
endfunction(enable_doxygen)
