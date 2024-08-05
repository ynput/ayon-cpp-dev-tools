file(GLOB AYONCPPTOOLS_TESTSRC ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
add_executable(AyonCppToolsTest ${AYONCPPTOOLS_TESTSRC}) 

target_link_libraries(test_app PRIVATE AyonCppApi)

install (
  TARGETS test_app
  DESTINATION ${CMAKE_INSTALL_PREFIX}
)


