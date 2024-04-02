#pragma once

#include "Texture.hpp"
#include "Vertex.hpp"

#include <glm/glm.hpp>
#include <vector>

namespace rb {
    class Mesh {
        public:
            Mesh(std::vector<Vertex>&& vertices,
                 std::vector<unsigned int>&& vertex_indices,
                 std::vector<unsigned int>&& uv_indices,
                 std::vector<glm::vec2>&& uv_coordinates,
                 const char* uv_texture_file_name);

            Mesh(std::vector<Vertex> vertices,
                 std::vector<unsigned int> vertex_indices,
                 std::vector<unsigned int> uv_indices,
                 std::vector<glm::vec2> uv_coordinates,
                 const char* uv_texture_file_name);
            Mesh(Mesh&& mesh);

            static auto from_obj(const char* filename) -> std::optional<Mesh>;

            void rotate(const float& amount, const glm::vec3& direction);
            void translate(const glm::vec3& translation);

            std::vector<Vertex> vertices;
            std::vector<unsigned int> vertex_indices;
            std::vector<unsigned int> uv_indices;
            std::vector<glm::vec2> uv_coordinates;
            rb::Texture uv_texture;
    };
} // namespace rb
