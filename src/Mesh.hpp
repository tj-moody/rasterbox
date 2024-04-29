#pragma once

#include "Texture.hpp"

#include <glm/glm.hpp>
#include <vector>

namespace rb {
    class Mesh {
        public:
            Mesh(std::vector<glm::vec3>&& vertices,
                 std::vector<glm::vec3>&& normals,
                 std::vector<glm::vec2>&& uv_coordinates,
                 std::vector<unsigned int>&& vertex_indices,
                 std::vector<unsigned int>&& normal_indices,
                 std::vector<unsigned int>&& uv_indices,
                 const char* uv_texture_file_name);

            Mesh(Mesh&& mesh);

            static auto from_obj(const char* filename) -> std::optional<Mesh>;

            void rotate(const float& amount, const glm::vec3& direction);
            void translate(const glm::vec3& translation);
            void scale(const float& scale);

            void set_rotation(const float& amount, const glm::vec3& direction);
            void set_translation(const glm::vec3& translation);
            void set_scale(const float& scale);

            std::vector<glm::vec3> vertices;
            std::vector<glm::vec3> normals;
            std::vector<glm::vec2> uv_coordinates;
            std::vector<unsigned int> vertex_indices;
            std::vector<unsigned int> normal_indices;
            std::vector<unsigned int> uv_indices;
            rb::Texture uv_texture;

            glm::mat4 model_translation;
            glm::mat4 model_rotation;
            float model_scale;
    };
} // namespace rb
