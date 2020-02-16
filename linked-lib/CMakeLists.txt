project(StaticFoo)

file(GLOB SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)

include_guard(GLOBAL)

add_library(${PROJECT_NAME} OBJECT ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC include)

add_library(linkedfoo SHARED $<TARGET_OBJECTS:${PROJECT_NAME}>)
add_library(foostatic STATIC $<TARGET_OBJECTS:${PROJECT_NAME}>)

source_group(TREE "${PROJECT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_FILES})


install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(FILES ${HEADER_FILES} DESTINATION include)