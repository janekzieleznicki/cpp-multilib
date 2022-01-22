project(foo-tests)


find_package(GTest REQUIRED)
include(GoogleTest)
add_executable(${PROJECT_NAME} 
FooTest.cpp
WaitingSMTest.cpp
)


target_link_libraries(${PROJECT_NAME} PRIVATE StaticFoo GTest::gtest_main GTest::gmock)
gtest_discover_tests(${PROJECT_NAME} )