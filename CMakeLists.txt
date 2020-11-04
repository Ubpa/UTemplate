cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(UTemplate VERSION 0.4.9)
message(STATUS "[Project] ${PROJECT_NAME}")

include(cmake/InitUCMake.cmake)
Ubpa_InitUCMake()

Ubpa_InitProject()

CPMAddPackage(
  NAME nameof
  GITHUB_REPOSITORY Neargye/nameof
  GIT_TAG 0e8bec0
)
Ubpa_AddSubDirsRec(src)

Ubpa_Export(
  TARGET
  CPM
  DIRECTORIES
    "include"
)
