  find_package(GTest REQUIRED)
  # Add tests
  add_executable(${PROJECT_NAME}_tests
          test_SpecialId.cpp)

  target_link_libraries(${PROJECT_NAME}_tests
          PUBLIC
          can_reader_lib
          PRIVATE
          GTest::gtest_main
          GTest::gtest
          spdlog::spdlog
  )
  target_include_directories(${PROJECT_NAME}_tests PRIVATE src)
  add_test(NAME ${PROJECT_NAME}_tests COMMAND ${PROJECT_NAME}_tests)