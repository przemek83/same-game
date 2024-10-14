SET(PROJECT_TEST ${PROJECT_NAME}-test )

include_directories(${CMAKE_SOURCE_DIR})

set(${PROJECT_TEST}_SOURCES
    SameGameTest.cpp
    BoardTest.cpp
    TestTools.h
    TestTools.cpp
    MockedGenerator.h
    MockedGenerator.cpp
)

file(GLOB TXT_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/*.txt"
)
file(COPY ${TXT_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(${PROJECT_TEST} ${${PROJECT_TEST}_SOURCES})
target_link_libraries(${PROJECT_TEST} PRIVATE GTest::gtest_main ${PROJECT_NAME}-lib)

include(CTest)
include(GoogleTest)
gtest_discover_tests(${PROJECT_TEST})
