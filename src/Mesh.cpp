#include "Mesh.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

rb::Mesh::Mesh(Mesh&& mesh)
    : vertices(std::move(mesh.vertices)), indices(std::move(mesh.indices)) {}
rb::Mesh::Mesh(std::vector<Vertex>&& vertices,
               std::vector<unsigned int>&& indices)
    : vertices(std::move(vertices)), indices(std::move(indices)) {}
rb::Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
    : vertices(vertices), indices(indices) {}
rb::Mesh::Mesh(): vertices(), indices() {}

void rb::Mesh::rotate(const float& amount, const glm::vec3& direction) {
    glm::mat4 rotationMat = glm::rotate(glm::mat4(1), amount, direction);
    for (rb::Vertex& vertex : this->vertices) {
        vertex.position
            = glm::vec3(rotationMat * glm::vec4(vertex.position, 0));
    }
}

void rb::Mesh::translate(const glm::vec3& translation) {
    for (rb::Vertex& vertex : this->vertices) {
        vertex.position += translation;
    }
}
