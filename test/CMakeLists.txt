SET(PROJECT_TEST ${PROJECT_NAME}-test )

set(${PROJECT_TEST}_SOURCES
    SameGameTest.cpp
    ../SameGame.cpp
    ../SameGame.h
    ../Board.cpp
    ../Board.h
    ../Point.cpp
    ../Point.h
    )

file(GLOB TXT_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/*.txt"
)
file(COPY ${TXT_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(${PROJECT_TEST} ${${PROJECT_TEST}_SOURCES})

target_link_libraries(${PROJECT_TEST} gtest gtest_main)

add_test(NAME ${PROJECT_TEST} COMMAND ${PROJECT_TEST})
