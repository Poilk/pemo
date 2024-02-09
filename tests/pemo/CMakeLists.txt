enable_testing()

find_package(GTest REQUIRED)

file(GLOB_RECURSE unit_test_source "*.cpp" "*.h")

add_executable(pemo_test test_main.cpp ${unit_test_source})

target_link_libraries(pemo_test pemo GTest::gtest)

add_test(all_test pemo_test)