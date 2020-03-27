function(enable_doxygen)
  option(INCH_BUILD_DOCS "Build documentation with Doxygen" OFF)
  if(INCH_BUILD_DOCS)
    find_package(Doxygen)

    if(NOT DOXYGEN_FOUND)
      message(ERROR "Doxygen is needed to build the documentation.")
    endif()

    set(DOXYGEN_EXCLUDE_PATTERNS "*/external/*")

    message(STATUS "Building documentation with doxygen version ${DOXYGEN_VERSION}")
    doxygen_add_docs(doxygen_docs
      WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
      COMMENT "Generating documentation with doxygen")
  endif()
endfunction()
