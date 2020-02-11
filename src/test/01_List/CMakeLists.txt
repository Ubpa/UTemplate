Ubpa_GetDirName(DIRNAME)
set(targetName "${DIRNAME}")

Ubpa_AddCurPathSrcs(sources)

set(libs "")

Ubpa_AddTarget(MODE ${mode} NAME ${targetName} SOURCES ${sources} LIBS ${libs})
