add_executable(ch01
  src/main.cpp
  src/Lexer.cpp
)

target_include_directories(ch01 PRIVATE src)

target_compile_definitions(ch01 PRIVATE DEBUG_OUTPUT=1)

file(GLOB TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/test*.txt")
foreach(TEST_FILE ${TEST_FILES})
        get_filename_component(TEST_FILENAME ${TEST_FILE} NAME)
        configure_file(${TEST_FILE}
                       ${CMAKE_CURRENT_BINARY_DIR}/${TEST_FILENAME} COPYONLY)
endforeach()
