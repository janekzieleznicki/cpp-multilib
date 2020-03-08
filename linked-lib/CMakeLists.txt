project(StaticFoo)

file(GLOB SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/Foo.cpp)
file(GLOB HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)

include_guard(GLOBAL)

add_library(${PROJECT_NAME} OBJECT ${SOURCE_FILES})


find_package(benchmark CONFIG REQUIRED)
add_executable(${PROJECT_NAME}_demo src/WaitingDemo.cpp)
# target_compile_definitions(${PROJECT_NAME}_demo PRIVATE LOGGING_TRACE)



add_executable(variant_compile_size src/CompileSize.cpp)
target_compile_definitions(variant_compile_size PRIVATE COMPILE_VARIANT)
target_link_libraries(variant_compile_size PRIVATE StaticFoo)

add_executable(virtual_compile_size src/CompileSize.cpp)
target_compile_definitions(virtual_compile_size PRIVATE COMPILE_VIRTUAL)
target_link_libraries(virtual_compile_size PRIVATE StaticFoo)


target_link_libraries(${PROJECT_NAME}_demo PRIVATE StaticFoo benchmark::benchmark)

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

add_library(linkedfoo SHARED $<TARGET_OBJECTS:${PROJECT_NAME}>)
add_library(foostatic STATIC $<TARGET_OBJECTS:${PROJECT_NAME}>)

source_group(TREE "${PROJECT_SOURCE_DIR}/include" PREFIX "Header Files" FILES ${HEADER_FILES})


install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(FILES ${HEADER_FILES} DESTINATION include)

enable_testing()
add_subdirectory(test)