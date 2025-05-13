
file(GLOB SOURCES "*.cpp")

add_library(glad STATIC ${CMAKE_SOURCE_DIR}/external/glad/glad.c)


add_executable(ParticleSimulator ${SOURCES})

target_link_libraries(ParticleSimulator glfw glad)


target_include_directories(ParticleSimulator PRIVATE ${CMAKE_SOURCE_DIR}/external/glm)
target_include_directories(ParticleSimulator PRIVATE ${CMAKE_SOURCE_DIR}/external/glad)