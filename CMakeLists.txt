cmake_minimum_required(VERSION 3.10)
project(Kaleidoscope CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

foreach(CHAPTER RANGE 1 10)
        string(LENGTH "${CHAPTER}" CHAPTER_LEN)
        if(CHAPTER_LEN EQUAL 1)
                set(CHAPTER_DIR "ch0${CHAPTER}")
        else()
                set(CHAPTER_DIR "ch${CHAPTER}")
        endif()

        if(EXISTS "${CMAKE_SOURCE_DIR}/${CHAPTER_DIR}/CMakeLists.txt")
                add_subdirectory(${CHAPTER_DIR})
        endif()
endforeach()
