#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <GL/glew.h>
#include "shader/shader.hpp"
#include "util/file_operations.hpp"

static constexpr std::size_t log_buffer_size{512};

shader::shader(
        const std::string& vertex_shader_source_path,
        const std::string& fragment_shader_source_path
        )
    : shader_id{glCreateProgram()}
{
    std::optional<uint32_t> vertex_shader{
        prepare_single_shader(type::vertex, vertex_shader_source_path)
    };
    std::optional<uint32_t> fragment_shader{
        prepare_single_shader(type::fragment, fragment_shader_source_path)
    };
    if(!vertex_shader.has_value() || !fragment_shader.has_value()) {
        // TODO: raise error or just log the error?
    }
    std::optional<uint32_t> shader{link_shaders(vertex_shader.value(), fragment_shader.value())};
    if(!shader.has_value()) {
        // TODO: raise error or just log the error?
    }
    this->shader_id = shader.value();
}

void shader::bind() const {
    glUseProgram(shader_id);
}

void shader::unbind() const {
    glUseProgram(0);
}

std::optional<uint32_t> shader::prepare_single_shader(const type &t, const std::string& shader_source_path) {
    GLenum gl_shader_type = (t == type::vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
    uint32_t shader{glCreateShader(gl_shader_type)};
    std::optional<std::string> shader_source{
        util::read_file_as_string(shader_source_path)
    };
    if(!shader_source.has_value()) {
        return {};
    }
    const char *shader_source_cstr{shader_source->c_str()};
    glShaderSource(shader, 1, &shader_source_cstr, nullptr);
    glCompileShader(shader);

    /******************************************/
    // TODO: refactor, shouldn't be handled in here but in some logger class or something
    int status{};
    char info_log[log_buffer_size]{};
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE) {
        glGetShaderInfoLog(shader_id, log_buffer_size, nullptr, info_log);
        std::cout << "[shader compilation failed, " << glGetError() << "]: ";
        std::cout << (info_log[0] == '\0' ? "No info log available" : info_log)<< "\n";
        // exit(EXIT_FAILURE);
        return {};
    }
    /******************************************/

    return shader;
}

std::optional<uint32_t> shader::link_shaders(uint32_t vertex_shader_id, uint32_t fragment_shader_id) {
    uint32_t program{glCreateProgram()};
    glAttachShader(program, vertex_shader_id);
    glAttachShader(program, fragment_shader_id);
    glLinkProgram(program);

    /*****************************************/
    // TODO: refactor, shouldnt be handled in here but in some logger class or something
    char info_log[log_buffer_size]{};
    int32_t status{};
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if(status == GL_FALSE) {
        glGetProgramInfoLog(program, log_buffer_size, nullptr, info_log);
        std::cout << "[shader linking failed, " << glGetError() << "]: ";
        std::cout << (info_log[0] == '\0' ? "No info log available" : info_log)<< "\n";
        // exit(EXIT_FAILURE);
        return {};
    }
    /*****************************************/

    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);
    return program;
}
