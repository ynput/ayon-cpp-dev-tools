cmake_minimum_required(VERSION 3.19 FATAL_ERROR)
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)

project(AyonCppDevTools VERSION 1.0.0 LANGUAGES CXX)

include_directories("${CMAKE_CURRENT_LIST_DIR}/src/NameSpaceDef")
include_directories("${CMAKE_CURRENT_LIST_DIR}/src/Macros")
include_directories("${CMAKE_CURRENT_LIST_DIR}/src/lib")

if(AYON_CPPTOOLS_GTEST)
  add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/ext/googletest")
endif()
if(AYON_CPPTOOLS_GBNECH)
  add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/ext/benchmark")
endif()


if(AYON_CPPTOOLS_ENABLE_TESTING)
  add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/tests")
endif()
