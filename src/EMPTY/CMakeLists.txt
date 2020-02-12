Ubpa_GetDirName(DIRNAME)
set(targetName "${TARGET_PREFIX}${DIRNAME}")

Ubpa_GlobGroupSrcs(RST sources PATHS
	${CMAKE_CURRENT_SOURCE_DIR}
	"${PROJECT_SOURCE_DIR}/include/UTemplate"
)

set(libs "")
Ubpa_AddTarget(MODE "EXE" NAME ${targetName} SOURCES ${sources} LIBS ${libs})
