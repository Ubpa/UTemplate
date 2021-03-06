set(c_options "")
if(MSVC)
  list(APPEND c_options "/permissive-")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  list(APPEND c_options "-fconstexpr-steps=16777216") # for long type_name
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  #
endif()

Ubpa_AddTarget(
  MODE INTERFACE
  INC
    "${PROJECT_SOURCE_DIR}/include"
  C_OPTION
    ${c_options}
)
