set(c_options "")
if(MSVC)
  set(c_options "/permissive-")
endif()

Ubpa_AddTarget(
  MODE INTERFACE
  INC
    "${PROJECT_SOURCE_DIR}/include"
  C_OPTION
    ${c_options}
)
