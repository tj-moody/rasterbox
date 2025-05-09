#include "Mesh.hpp"

#include <fstream>
#include <iostream>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>


rb::Mesh::Mesh(Mesh&& mesh) = default;

rb::Mesh::Mesh(std::vector<glm::vec3>&& vertices,
               std::vector<glm::vec3>&& normals,
               std::vector<glm::vec2>&& uv_coordinates,
               std::vector<unsigned int>&& vertex_indices,
               std::vector<unsigned int>&& normal_indices,
               std::vector<unsigned int>&& uv_indices,
               const char* uv_texture_file_name)
    : vertices(std::move(vertices)),
      normals(std::move(normals)),
      uv_coordinates(std::move(uv_coordinates)),
      vertex_indices(std::move(vertex_indices)),
      normal_indices(std::move(normal_indices)),
      uv_indices(std::move(uv_indices)),
      uv_texture(rb::Texture::from_tga(uv_texture_file_name)),
      model_translation(1.0f),
      model_rotation(1.0f),
      model_scale(1.0f) {}


void rb::Mesh::rotate(const float& amount, const glm::vec3& direction) {
    this->model_rotation = glm::rotate(this->model_rotation, amount, direction);
}

void rb::Mesh::translate(const glm::vec3& translation) {
    this->model_translation
        = glm::translate(this->model_translation, translation);
}

void rb::Mesh::scale(const float& scale) {
    this->model_scale *= scale;
}

void rb::Mesh::set_rotation(const float& amount, const glm::vec3& direction) {
    this->model_rotation = glm::rotate(glm::mat4(1.0f), amount, direction);
}
void rb::Mesh::set_translation(const glm::vec3& translation) {
    this->model_translation = glm::translate(glm::mat4(1.0f), translation);
}
void rb::Mesh::set_scale(const float& scale) { this->model_scale = scale; }

auto rb::Mesh::from_obj(const char* filename) -> std::optional<rb::Mesh> {
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail()) {
        std::cerr << "Invalid file path: " << filename << '\n';
        in.close();
        return std::nullopt;
    };
    std::string line;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uv_coordinates;
    std::vector<unsigned int> vertex_indices;
    std::vector<unsigned int> normal_indices;
    std::vector<unsigned int> uv_indices;

    float _f;

    unsigned int v_i0, v_i1, v_i2;    // Vertex indices
    unsigned int uv_i0, uv_i1, uv_i2; // UV indices
    unsigned int n_i0, n_i1, n_i2;    // Normal indices

    glm::vec3 v;
    glm::vec2 uv_coordinate;
    glm::vec3 normal;

    while (!in.eof()) {
        std::getline(in, line);

        if (!line.compare(0, 2, "v ")) {
            sscanf(line.c_str(), "v %f %f %f", &v.x, &v.y, &v.z);

            vertices.emplace_back(v);
        } else if (!line.compare(0, 2, "f ")) {
            // TODO: Detect different `.obj` versions (?) and read vertex data
            // differently

            // clang-format off
            sscanf(line.c_str(),
                   // "f %i %i %i", &x, &y, &z);
                   "f %u/%u/%u %u/%u/%u %u/%u/%u",
                   &v_i0, &uv_i0, &n_i0,
                   &v_i1, &uv_i1, &n_i1,
                   &v_i2, &uv_i2, &n_i2);
            // clang-format on

            vertex_indices.push_back(v_i0);
            vertex_indices.push_back(v_i1);
            vertex_indices.push_back(v_i2);

            uv_indices.push_back(uv_i0);
            uv_indices.push_back(uv_i1);
            uv_indices.push_back(uv_i2);

            normal_indices.push_back(n_i0);
            normal_indices.push_back(n_i1);
            normal_indices.push_back(n_i2);
        } else if (!line.compare(0, 3, "vt ")) {
            sscanf(line.c_str(),
                   "vt %f %f %f",
                   &uv_coordinate.x,
                   &uv_coordinate.y,
                   &_f);
            uv_coordinates.push_back(uv_coordinate);
        } else if (!line.compare(0, 3, "vn ")) {
            sscanf(line.c_str(),
                   "vn %f %f %f",
                   &normal.x,
                   &normal.y,
                   &normal.z);
            normals.push_back(normal);
        }
    }

    for (auto& index : vertex_indices) { index -= 1; }
    for (auto& index : normal_indices) { index -= 1; }
    for (auto& index : uv_indices) { index -= 1; }

    in.close();

    return std::make_optional(rb::Mesh(std::move(vertices),
                                       std::move(normals),
                                       std::move(uv_coordinates),
                                       std::move(vertex_indices),
                                       std::move(normal_indices),
                                       std::move(uv_indices),
                                       "obj/head_texture.tga"));
}
