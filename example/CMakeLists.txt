include_directories(${PROJECT_SOURCE_DIR}/include)

aux_source_directory(. TEST)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_executable(test ${TEST})

target_link_libraries(test ${PROJECT_SOURCE_DIR}/lib/libcstring.a)