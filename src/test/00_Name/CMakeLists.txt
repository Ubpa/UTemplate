Ubpa_GetDirName(DIRNAME)
set(targetName "${DIRNAME}")

Ubpa_AddCurPathSrcs(sources)

set(libs "")

Ubpa_AddTarget(MODE "EXE" NAME ${targetName} SOURCES ${sources} LIBS ${libs})
