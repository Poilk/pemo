set(TARGET_NAME pemo)

add_library(${TARGET_NAME} INTERFACE)

target_link_libraries(${TARGET_NAME} INTERFACE pemo_common pemo_engine)

add_subdirectory(engine)