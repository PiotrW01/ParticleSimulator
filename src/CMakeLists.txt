
file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.h")

add_library(glad STATIC ${CMAKE_SOURCE_DIR}/external/glad/glad.c)
target_include_directories(glad PRIVATE ${CMAKE_SOURCE_DIR}/external/glad)

add_executable(ParticleSimulator ${SOURCES} ${HEADERS})

target_link_libraries(ParticleSimulator glfw glad)

target_include_directories(ParticleSimulator PRIVATE ${CMAKE_SOURCE_DIR}/external/glm)
target_include_directories(ParticleSimulator PRIVATE ${CMAKE_SOURCE_DIR}/external/glad)
