set(TARGET_NAME pemo_engine)

add_library(${TARGET_NAME} thread_pool.cpp)

target_link_libraries(${TARGET_NAME} pemo_common)

target_include_directories(${TARGET_NAME} PUBLIC .)