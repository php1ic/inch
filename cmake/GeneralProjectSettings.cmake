# Output compilation database that can be used by analysis tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

## Compile against c++17
#set(CMAKE_CXX_STANDARD 17)
## We need c++17 so can't fallback
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
## Use -std=c++?? rather than -std=gnu++??
#set(CMAKE_CXX_EXTENSIONS OFF)

# Use Debug as the default build type
if(NOT CMAKE_BUILD_TYPE)
  set(
    CMAKE_BUILD_TYPE "RelWithDebInfo"
    CACHE STRING "Set the build type to use: Debug, Release, RelWithDebInfo (default), MinSizeRel" FORCE
    )
endif()

# Use ccache if it exists on the system
find_program(CCACHE ccache)
if(CCACHE)
  message(STATUS "ccache found")
  set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
else()
  message(STATUS "ccache not found cannot use")
endif()

# Use IPO if possible
include(CheckIPOSupported)
check_ipo_supported(RESULT IPOsupported OUTPUT error)
if(IPOsupported)
  message(STATUS "IPO/LTO supported")
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
  message(STATUS "IPO/LTO not supported: <${error}>")
endif()
