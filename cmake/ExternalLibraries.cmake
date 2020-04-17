function(add_external_libraries project_name)
  # Use the {fmt} library. It will be part of C++20
  add_subdirectory(external/fmt)
  target_link_libraries(project_options INTERFACE fmt::fmt-header-only)

  # Need to link the filesystem library in older compiler versions
  # But not for MSVC
  target_link_libraries(project_options INTERFACE $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:stdc++fs>)
endfunction(add_external_libraries)
