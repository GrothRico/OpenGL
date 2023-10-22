#include <GL/glew.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include "assets.hpp"
#include "debug/opengl_debug.hpp"
#include "shader/shader.hpp"

constexpr int WINDOW_HEIGHT{800};
constexpr int WINDOW_WIDTH{800};

void teardown()
{
    glfwTerminate();
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    std::cout << "resize window to " << width << ", " << height << ".\n";
    glViewport(0, 0, width, height);
}

uint32_t create_triangle()
{
    float vertex_data[]{
        -0.5f,
        -0.5f,
        0.0f,
        0.5f,
        0.5f,
        -0.5f,
    };
    uint32_t vertex_buffer{};
    uint32_t vertex_array{};
    glCreateVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);
    glCreateBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    return vertex_array;
}

uint32_t create_square()
{
    float vertex_data[]{
        -0.5f, 0.5f,
        0.5f, 0.5f,
        0.5f, -0.5f,
        -0.5f, -0.5f};
    int n_triangles{2};
    int n_vertices_per_triangle{3};
    int n_coordinates_per_vertex{2};

    uint32_t index_data[]{
        0, 1, 2,
        2, 3, 0};

    uint32_t vertex_array{};
    uint32_t vertex_buffer{};
    uint32_t index_buffer{};
    glCreateVertexArrays(1, &vertex_array);
    glCreateBuffers(1, &vertex_buffer);
    glCreateBuffers(1, &index_buffer);

    glBindVertexArray(vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(float) * n_triangles * n_vertices_per_triangle * n_coordinates_per_vertex,
        vertex_data,
        GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(uint32_t) * n_triangles * n_vertices_per_triangle,
        index_data,
        GL_STATIC_DRAW);

    glVertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        false,
        sizeof(float) * n_coordinates_per_vertex,
        nullptr);

    glEnableVertexAttribArray(0);

    return vertex_array;
}

void initGLFW()
{
    if (glfwInit() == GLFW_FALSE)
    {
        std::cerr << "Failed to initialize GLFW\n";
        teardown();
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void initGLEW()
{
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW\n";
        teardown();
        exit(EXIT_FAILURE);
    }
}

GLFWwindow *create_window()
{
    GLFWwindow *window{glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL", nullptr, nullptr)};
    if (window == nullptr)
    {
        std::cerr << "Failed to create GLFWwindow\n";
        teardown();
        exit(EXIT_FAILURE);
    }
    return window;
}

GLFWwindow *init(bool wireframe_mode)
{
    initGLFW();
    GLFWwindow *window{create_window()};
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    initGLEW();
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(open_gl::debug_callback, 0);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    if (wireframe_mode)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    return window;
}

void event_loop_square(GLFWwindow *window)
{
    shader s{SHADER_DIR "vertex.glsl", SHADER_DIR "fragment.glsl"};
    uint32_t square_vertex_array{create_square()};
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        s.bind();
        glBindVertexArray(square_vertex_array);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void event_loop_triangle(GLFWwindow *window)
{
    shader s{SHADER_DIR "vertex.glsl", SHADER_DIR "fragment.glsl"};
    uint32_t triangle_vertex_array{create_triangle()};
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        s.bind();
        glBindVertexArray(triangle_vertex_array);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

int main()
{
    GLFWwindow *window{init(false)};
    shader s{SHADER_DIR "vertex.glsl", SHADER_DIR "fragment.glsl"};
    // event_loop_square(window);
    event_loop_triangle(window);

    teardown();
    return EXIT_SUCCESS;
}
