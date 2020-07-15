macro(add_example NAME)
  add_executable("${NAME}" ${ARGN})
  target_link_libraries("${NAME}" PRIVATE escript)
endmacro()

add_example(Script "script/main.cpp")
