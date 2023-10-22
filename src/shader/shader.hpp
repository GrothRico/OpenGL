#pragma once

#include <cstdint>
#include <initializer_list>
#include <string>
#include <optional>

class shader {
    public:
        enum class type {vertex, fragment};
        shader(const std::string& vertex_shader_source_path, const std::string& fragment_shader_source_path);
        void bind() const;
        void unbind() const;
    private:
        uint32_t shader_id;
        void compile_shader(uint32_t shader_id, const char *shader_source);
        std::optional<uint32_t> prepare_single_shader(const type &t, const std::string& shader_source_path);
        std::optional<uint32_t> link_shaders(uint32_t vertex_shader_id, uint32_t fragment_shader_id);
};
