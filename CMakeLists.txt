cmake_minimum_required(VERSION 3.0.0)

project(vsb2)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

fetchcontent_declare(
    cglm
    GIT_REPOSITORY https://github.com/recp/cglm.git
    GIT_TAG v0.8.9
    GIT_PROGRESS TRUE
)

if(NOT cglm_POPULATED)
    message("populating cglm")
    fetchcontent_populate(cglm)
    add_subdirectory(${cglm_SOURCE_DIR} ${cglm_BUILD_DIR})
endif()

fetchcontent_declare(
    sake
    GIT_REPOSITORY https://github.com/wervin/sake.git
    GIT_PROGRESS TRUE
)

if(NOT sake_POPULATED)
    message("populating sake")
    fetchcontent_populate(sake)
    add_subdirectory(${sake_SOURCE_DIR} ${sake_BUILD_DIR})
endif()

set(VSB2_SOURCES
    src/vsb2/graphics/commandbuffer.c
    src/vsb2/graphics/commandpool.c
    src/vsb2/graphics/device.c
    src/vsb2/graphics/framebuffers.c
    src/vsb2/graphics/instance.c
    src/vsb2/graphics/renderpass.c
    src/vsb2/graphics/swapchain.c
    src/vsb2/graphics/sync.c
    src/vsb2/graphics/window.c
    src/vsb2/engine.c
    src/vsb2/error.c
    src/vsb2/log.c
    src/vsb2/main.c)

add_executable(vsb2 ${VSB2_SOURCES})

target_link_libraries(vsb2
    m
    cglm
    sake
    /usr/lib/x86_64-linux-gnu/libvulkan.so
    /usr/lib/x86_64-linux-gnu/libglfw.so)

target_include_directories(vsb2 PRIVATE
    inc)

set_property(TARGET vsb2 PROPERTY C_STANDARD 11)

target_compile_options(vsb2 PRIVATE 
    -Wall -Wextra -Wpedantic)

# add_custom_target(vert.spv
#     COMMAND glslangValidator --quiet -V ${CMAKE_SOURCE_DIR}/shaders/shader.vert -o ${CMAKE_BINARY_DIR}/vert.spv)

# add_custom_target(frag.spv
#     COMMAND glslangValidator --quiet -V ${CMAKE_SOURCE_DIR}/shaders/shader.frag -o ${CMAKE_BINARY_DIR}/frag.spv)

# add_dependencies(vsb2
#     vert.spv
#     frag.spv)

