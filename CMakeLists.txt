cmake_minimum_required(VERSION 3.1)

project("Digital Signal Processing Simulation C++ library (libdsps)"
  VERSION 1.0
  LANGUAGES CXX
)

option(DSPS_DEBUG     "Activate debug build" OFF)
option(DSPS_TESTS     "Activate the unitary tests" OFF)
option(DSPS_EXAMPLES  "Activate the compilation of some examples" OFF)

if(NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
  message(STATUS "Setting build type to 'RelWithDebInfo' as none was specified.")
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# CXXFLAGS
# assume it's a GNU-like compiler (GCC, clang)
set(CMAKE_CXX_FLAGS                 "-Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG           "-O0 -g")
set(CMAKE_CXX_FLAGS_MINSIZEREL      "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE         "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "-O2 -g")

# Manage the debug
if (DSPS_DEBUG)
  message(STATUS "Debug enable")
  set(CMAKE_BUILD_TYPE Debug)
endif()

# Manage the units tests
if (DSPS_TESTS)
  message(STATUS "Tests enable")
  add_subdirectory(test)
endif()

# Manage the examples
if (DSPS_EXAMPLES)
  message(STATUS "Compile examples")
  add_subdirectory(example)
endif()

# Common depends
find_package(dsac REQUIRED)

add_subdirectory("src")

# Include GNUInstallDirs (if Unix system)
if(UNIX)
  include(GNUInstallDirs)
endif()

# Rules for install
install(
  TARGETS dsps
  EXPORT dspsConfig
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FILES_MATCHING
  PATTERN "*.h"
  PATTERN ".swp" EXCLUDE
)

install(
  EXPORT dspsConfig
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/dsps"
)
