cmake_minimum_required(VERSION 3.10)


project(ParticleSimulator)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_subdirectory(external/freetype)
add_subdirectory(external/glfw)
add_subdirectory(src)
