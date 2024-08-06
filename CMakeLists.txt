cmake_minimum_required(VERSION 3.19 FATAL_ERROR)
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)

project(AyonCppDevTools VERSION 1.0.0 LANGUAGES CXX)

add_library(AyonCppDevToolsLib INTERFACE)

target_include_directories(AyonCppDevToolsLib INTERFACE
    "${CMAKE_CURRENT_LIST_DIR}/src/NameSpaceDef"
    "${CMAKE_CURRENT_LIST_DIR}/src/Macros"
    "${CMAKE_CURRENT_LIST_DIR}/src/lib"
)

if(AYON_CPPTOOLS_GTEST)
  target_include_directories(AyonCppDevToolsLib INTERFACE "${CMAKE_CURRENT_LIST_DIR}/ext/googletest")
endif()
if(AYON_CPPTOOLS_GBNECH)
  target_include_directories(AyonCppDevToolsLib INTERFACE "${CMAKE_CURRENT_LIST_DIR}/ext/benchmark")
endif()


if(AYON_CPPTOOLS_ENABLE_TESTING)
  target_include_directories(AyonCppDevToolsLib INTERFACE "${CMAKE_CURRENT_LIST_DIR}/tests")
endif()
