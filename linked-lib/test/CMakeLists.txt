project(foo-tests)

add_executable(${PROJECT_NAME} 
FooTest.cpp
)


target_link_libraries(${PROJECT_NAME} PRIVATE StaticFoo GTest::gtest_main GTest::gmock)