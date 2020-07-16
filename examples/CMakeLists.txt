macro(add_example NAME)
  add_executable("${NAME}" ${ARGN})
  target_link_libraries("${NAME}" PRIVATE escript)

  add_custom_target("${NAME}_resource_copy" ALL
    "${CMAKE_COMMAND}" -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/resources"
    "${CMAKE_CURRENT_BINARY_DIR}/resources")
endmacro()

add_subdirectory(script)
