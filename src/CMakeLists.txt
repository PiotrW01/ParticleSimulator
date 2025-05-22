add_library(glad STATIC ${CMAKE_SOURCE_DIR}/external/glad/glad.c)
target_include_directories(glad PRIVATE ${CMAKE_SOURCE_DIR}/external/glad)


add_executable(ParticleSimulator)

target_include_directories(ParticleSimulator PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(core)
add_subdirectory(input)
add_subdirectory(graphics)
add_subdirectory(gui)
add_subdirectory(particles)
add_subdirectory(utils)


add_custom_command(
    TARGET ParticleSimulator POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:ParticleSimulator>/assets
)

target_link_libraries(ParticleSimulator glfw glad freetype)

target_include_directories(ParticleSimulator PRIVATE ${CMAKE_SOURCE_DIR}/external/glm)
target_include_directories(ParticleSimulator PRIVATE ${CMAKE_SOURCE_DIR}/external/glad)
