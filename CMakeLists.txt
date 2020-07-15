cmake_minimum_required(VERSION 3.5)

project(escript)

option(ESCRIPT_EXAMPLES "Build examples" OFF)

include(FindTCL)
if(NOT TCL_FOUND)
  message(FATAL_ERROR "Cannot find the TCL library")
endif()

add_subdirectory("external/elements" EXCLUDE_FROM_ALL)

add_library(escript STATIC
  "src/escript.cpp"
  "src/escript_bindings.cpp"
  "src/escript_tcl.cpp"
  "src/escript.hpp"
  "src/escript_bindings.hpp"
  "src/escript_tcl.hpp"
  "src/escript_elements_fwd.hpp")
target_include_directories(escript PUBLIC "${TCL_INCLUDE_PATH}" "src")
target_link_libraries(escript PUBLIC cycfi::elements "${TCL_LIBRARY}")
add_library(escript::escript ALIAS escript)

if(ESCRIPT_EXAMPLES)
  add_subdirectory("examples")
endif()
