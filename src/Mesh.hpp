#pragma once

#include "Vertex.hpp"

#include <glm/glm.hpp>
#include <vector>

namespace rb {
    class Mesh {
        public:
            Mesh(std::vector<Vertex>&& vertices,
                 std::vector<unsigned int>&& indices);
            Mesh(std::vector<Vertex> vertices,
                 std::vector<unsigned int> indices);
            Mesh(Mesh&& mesh);
            Mesh();

            void rotate(const float& amount, const glm::vec3& direction);
            void translate(const glm::vec3& translation);

            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;
    };
}
