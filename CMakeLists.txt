cmake_minimum_required(VERSION 3.22)
project(SkeletalAnimation)

set(CMAKE_CXX_STANDARD 20)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(SkeletalAnimation main.cpp src/Window.cpp src/Window.h src/Renderer.cpp src/Renderer.h src/Buffer.h src/VertexArrayObject.cpp src/VertexArrayObject.h src/VertexArrayObjectOpenGL.cpp src/VertexArrayObjectOpenGL.h src/VertexBufferObject.cpp src/VertexBufferObject.h src/VertexBufferObjectOpenGL.cpp src/VertexBufferObjectOpenGL.h src/Shader.h src/ShaderOpenGL.cpp src/ShaderOpenGL.h)

target_include_directories(SkeletalAnimation PRIVATE src/ resources/)
target_link_libraries(SkeletalAnimation PRIVATE ${CONAN_LIBS})