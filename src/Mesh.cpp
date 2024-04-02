#include "Mesh.hpp"

#include <fstream>
#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>


rb::Mesh::Mesh(Mesh&& mesh)
    : vertices(std::move(mesh.vertices)),
      vertex_indices(std::move(mesh.vertex_indices)),
      uv_indices(std::move(mesh.uv_indices)),
      uv_coordinates(std::move(mesh.uv_coordinates)),
      uv_texture(std::move(mesh.uv_texture)) {}

rb::Mesh::Mesh(std::vector<Vertex>&& vertices,
               std::vector<unsigned int>&& indices,
               std::vector<unsigned int>&& uv_indices,
               std::vector<glm::vec2>&& uv_coordinates,
               const char* uv_texture_file_name)
    : vertices(std::move(vertices)),
      vertex_indices(std::move(indices)),
      uv_indices(std::move(uv_indices)),
      uv_coordinates(std::move(uv_coordinates)),
      uv_texture(rb::Texture::from_tga(uv_texture_file_name)) {}

rb::Mesh::Mesh(std::vector<Vertex> vertices,
               std::vector<unsigned int> indices,
               std::vector<unsigned int> uv_indices,
               std::vector<glm::vec2> uv_coordinates,
               const char* uv_texture_file_name)
    : vertices(std::move(vertices)),
      vertex_indices(std::move(indices)),
      uv_indices(std::move(uv_indices)),
      uv_coordinates(std::move(uv_coordinates)),
      uv_texture(rb::Texture::from_tga(uv_texture_file_name)) {}

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

auto rb::Mesh::from_obj(const char* filename) -> std::optional<rb::Mesh> {
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail()) {
        std::cerr << "Invalid file path: " << filename << '\n';
        in.close();
        return std::nullopt;
    };
    std::string line;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> vertex_indices;
    std::vector<unsigned int> uv_indices;
    std::vector<glm::vec2> uv_coordinates;

    int _i;
    float _f;

    Vertex v;
    int i0, i1, i2; // Vertex indices

    unsigned int uv_i0, uv_i1, uv_i2; // UV indices

    glm::vec2 uv_coordinate;

    while (!in.eof()) {
        std::getline(in, line);

        if (!line.compare(0, 2, "v ")) {
            sscanf(line.c_str(),
                   "v %f %f %f",
                   &v.position.x,
                   &v.position.y,
                   &v.position.z);

            vertices.emplace_back(v);
        } else if (!line.compare(0, 2, "f ")) {
            // TODO: Detect different `.obj` versions (?) and read vertex data
            // differently

            // clang-format off
            sscanf(line.c_str(),
                   // "f %i %i %i", &x, &y, &z);
                   "f %i/%i/%i %i/%i/%i %i/%i/%i",
                   &i0, &uv_i0, &_i,
                   &i1, &uv_i1, &_i,
                   &i2, &uv_i2, &_i);
            // clang-format on

            vertex_indices.push_back(std::move(i0));
            vertex_indices.push_back(std::move(i1));
            vertex_indices.push_back(std::move(i2));

            uv_indices.push_back(std::move(uv_i0));
            uv_indices.push_back(std::move(uv_i1));
            uv_indices.push_back(std::move(uv_i2));
        } else if (!line.compare(0, 3, "vt ")) {
            sscanf(line.c_str(),
                   "vt %f %f %f",
                   &uv_coordinate.x,
                   &uv_coordinate.y,
                   &_f);
            uv_coordinates.push_back(uv_coordinate);
        }
    }

    for (auto& index : vertex_indices) { index -= 1; }
    for (auto& index : uv_indices) { index -= 1; }

    in.close();

    return std::make_optional(rb::Mesh(vertices,
                                       vertex_indices,
                                       uv_indices,
                                       uv_coordinates,
                                       "obj/african_head_diffuse.tga"));
}
