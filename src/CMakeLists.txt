
file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.h")

add_library(glad STATIC ${CMAKE_SOURCE_DIR}/external/glad/glad.c)
target_include_directories(glad PRIVATE ${CMAKE_SOURCE_DIR}/external/glad)

add_executable(ParticleSimulator ${SOURCES} ${HEADERS})

add_custom_command(
    TARGET ParticleSimulator POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:ParticleSimulator>/assets
)

target_link_libraries(ParticleSimulator glfw glad)

target_include_directories(ParticleSimulator PRIVATE ${CMAKE_SOURCE_DIR}/external/glm)
target_include_directories(ParticleSimulator PRIVATE ${CMAKE_SOURCE_DIR}/external/glad)
