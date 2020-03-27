function(set_project_warnings PROJECT_WARNINGS)
  # I don't currently have access to a windows machine to test these,
  # so rather than trial and error pushing to trigger appveyor and check
  # for errors, leave it at this minimal list.
  set(MSVC_WARNINGS
    /wpermissive
    /W4
    )

  set(CLANG_WARNINGS
    -Werror
    -Wall
    -Wextra
    -Wpedantic
    #-Wshadow # {fmt} uses -Wno-shadow so breaks CI. I want CI to be green
    -Wnon-virtual-dtor
    -Woverloaded-virtual
    -Wnull-dereference
    -Wold-style-cast
    -Wcast-align
    -Wfloat-equal
    -Wunused
    #-Wconversion # STL containers uses size_t so this gives lots of warnings
    #-Wsign-conversion # STL containers uses size_t so this gives lots of warnings
    )

  set(GCC_WARNINGS
    ${CLANG_WARNINGS}
    -Wmisleading-indentation
    -Wduplicated-branches
    -Wduplicated-cond
    -Wlogical-op
    -Wuseless-cast
    )

  if(MSVC)
    set(${PROJECT_WARNINGS} ${MSVC_WARNINGS} PARENT_SCOPE)
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(${PROJECT_WARNINGS} ${CLANG_WARNINGS} PARENT_SCOPE)
  else()
    set(${PROJECT_WARNINGS} ${GCC_WARNINGS} PARENT_SCOPE)
  endif()

endfunction()
