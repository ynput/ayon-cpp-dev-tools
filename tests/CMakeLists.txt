if(AYON_CPPTOOLS_ENABLE_TESTING)
  add_executable(AyonCppToolsTest ${CMAKE_CURRENT_LIST_DIR}/src/GTestMain.cpp) 
  target_link_libraries(AyonCppToolsTest PRIVATE GTest::gtest_main)
  target_link_libraries(AyonCppToolsTest PRIVATE AyonCppDevToolsLib)

  install (
    TARGETS AyonCppToolsTest
    DESTINATION ${CMAKE_INSTALL_PREFIX}
  )
endif()

if(AYON_CPPTOOLS_ENABLE_BENCH)
  add_executable(AyonCppToolsBench ${CMAKE_CURRENT_LIST_DIR}/src/GBenchMain.cpp) 
  target_link_libraries(AyonCppToolsBench PRIVATE benchmark::benchmark)
  target_link_libraries(AyonCppToolsBench PRIVATE AyonCppDevToolsLib)

  install (
    TARGETS AyonCppToolsBench
    DESTINATION ${CMAKE_INSTALL_PREFIX}
  )
endif()

