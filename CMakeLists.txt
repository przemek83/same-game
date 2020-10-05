cmake_minimum_required(VERSION 3.8)

project(same-game
    LANGUAGES CXX 
    VERSION 1.0.0 
    DESCRIPTION "Simple board game based on rules of Same Game."
    HOMEPAGE_URL "https://github.com/przemek83/same-game")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB TXT_FILES
  "${CMAKE_SOURCE_DIR}/*.txt"
)
file(COPY ${TXT_FILES} DESTINATION ${CMAKE_BINARY_DIR})

set(${PROJECT_NAME}_SOURCES
    main.cpp
    )

add_executable(${CMAKE_PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})
