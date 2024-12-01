cmake_minimum_required(VERSION 3.14)

project(same-game
    LANGUAGES CXX 
    VERSION 1.0.0 
    DESCRIPTION "Simple board game based on rules of Same Game."
    HOMEPAGE_URL "https://github.com/przemek83/same-game")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(CODE_COVERAGE "Collect coverage" OFF)

if(CODE_COVERAGE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 --coverage")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 --coverage")
endif()

file(COPY testFile.txt DESTINATION ${CMAKE_BINARY_DIR})

set(${PROJECT_NAME}_SOURCES
    src/SameGame.cpp
    src/SameGame.h
    src/Board.cpp
    src/Board.h
    src/Point.h
    src/Generator.h
    src/NumbersGenerator.cpp 
    src/NumbersGenerator.h
)

add_library (${PROJECT_NAME}-lib STATIC ${${PROJECT_NAME}_SOURCES})
target_link_libraries(${PROJECT_NAME}-lib)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}-lib)

enable_testing()
include(Tests.cmake)
add_subdirectory(tests)
