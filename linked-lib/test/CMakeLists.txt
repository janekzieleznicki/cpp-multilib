project(foo-tests)


find_package(GTest CONFIG REQUIRED)
include(GoogleTest)
add_executable(${PROJECT_NAME} 
FooTest.cpp
)


target_link_libraries(${PROJECT_NAME} PRIVATE StaticFoo GTest::gtest_main GTest::gmock)
gtest_discover_tests(${PROJECT_NAME} )