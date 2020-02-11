Ubpa_GetDirName(DIRNAME)
set(targetName "${TARGET_PREFIX}${DIRNAME}")

Ubpa_AddCurPathSrcs(sources)
list(APPEND sources "${PROJECT_SOURCE_DIR}/include/Template/Typelist.h")
list(APPEND sources "${PROJECT_SOURCE_DIR}/include/Template/List.h")
list(APPEND sources "${PROJECT_SOURCE_DIR}/include/Template/Name.h")

set(libs "")
Ubpa_AddTarget(MODE "EXE" NAME ${targetName} SOURCES ${sources} LIBS ${libs})
