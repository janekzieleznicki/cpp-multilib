cmake_minimum_required(VERSION 3.16)
# Setting up dlib as an external package
# set(HUNTER_PACKAGES Boost GTest)

### Hunter snapshot that will be used ###
include("cmake/HunterGate.cmake")
HunterGate(
    # https://github.com/cpp-pm/hunter/releases
    URL "https://github.com/cpp-pm/hunter/archive/v0.23.251.tar.gz"
    SHA1 "5659b15dc0884d4b03dbd95710e6a1fa0fc3258d"
)

project(CMakeMultitarget)

hunter_add_package(GTest)
hunter_add_package(Boost)
hunter_add_package(benchmark)
find_package(Boost CONFIG REQUIRED)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
if(COMPILER_SUPPORTS_MARCH_NATIVE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
endif()

add_subdirectory(header-lib)
add_subdirectory(linked-lib)
add_subdirectory(src)