project(StaticFoo)


add_library(${PROJECT_NAME} OBJECT
    include/Foo.hpp
    src/Foo.cpp
)
target_include_directories(${PROJECT_NAME} PUBLIC include)

add_library(linkedfoo SHARED $<TARGET_OBJECTS:${PROJECT_NAME}>)
# add_library(foostatic STATIC $<TARGET_OBJECTS:${PROJECT_NAME}>)

source_group(TREE "${PROJECT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_LIST})