cmake_minimum_required(VERSION 3.22)
project(SkeletalAnimation)

set(CMAKE_CXX_STANDARD 20)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(SkeletalAnimation main.cpp src/Window.cpp src/Window.h)

target_include_directories(SkeletalAnimation PRIVATE src/)
target_link_libraries(SkeletalAnimation PRIVATE ${CONAN_LIBS})