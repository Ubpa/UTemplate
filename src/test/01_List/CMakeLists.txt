Ubpa_GetDirName(DIRNAME)
set(targetName "${DIRNAME}")

Ubpa_GlobGroupSrcs(RST sources PATHS ${CMAKE_CURRENT_SOURCE_DIR})

set(libs "")

Ubpa_AddTarget(MODE "EXE" NAME ${targetName} SOURCES ${sources} LIBS ${libs})
