cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

project(AyonCppDevTools VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_LIST_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(AyonCppDevToolsLib INTERFACE)

target_include_directories(AyonCppDevToolsLib INTERFACE
    "${CMAKE_CURRENT_LIST_DIR}/src/NameSpaceDef"
    "${CMAKE_CURRENT_LIST_DIR}/src/Macros"
    "${CMAKE_CURRENT_LIST_DIR}/src/lib"
)

if(AYON_CPPTOOLS_BUILD_LOGGER)
  add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/ext/spdlog")
  include_directories("${CMAKE_CURRENT_LIST_DIR}/ext/spdlog/include")
endif()

if(AYON_CPPTOOLS_ENABLE_TESTING)
  message(STATUS "Testing Builds are Enabled")
  add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/ext/googletest")
  add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/tests")
endif()

if(AYON_CPPTOOLS_ENABLE_BENCH)

  add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/ext/googletest")
  add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/ext/benchmark")
  add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/tests")
endif()

