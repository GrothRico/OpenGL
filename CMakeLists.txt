cmake_minimum_required(VERSION 3.22)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(Game)
add_executable(${CMAKE_PROJECT_NAME}
    src/main.cpp
    src/shader/shader.cpp
    src/debug/opengl_debug.cpp
    src/util/file_operations.cpp
    src/libport/stb_image.cpp
)
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/configuration/assets.hpp.in
    ${CMAKE_CURRENT_BINARY_DIR}/configuration/assets.hpp
)
target_include_directories(${CMAKE_PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/configuration
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/configuration
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/src
)
set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 20)
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE GLEW::GLEW)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE glfw)
