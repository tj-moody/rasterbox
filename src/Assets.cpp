#include "Assets.hpp"

#include <fstream>
#include <iostream>

std::optional<rb::Mesh> rb::Assets::load_obj(const char* filename) {
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail()) {
        std::cout << "Invalid file path: " << filename << '\n';
        return std::nullopt;
    };
    std::string line;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    int _;

    Vertex v;
    int x, y, z;
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
            sscanf(line.c_str(),
                   "f %i %i %i",
                   &x, &y, &z);
                   // "f %i/%i/%i %i/%i/%i %i/%i/%i",
                   // &x, &_, &_,
                   // &y, &_, &_,
                   // &z, &_, &_);

            indices.push_back(std::move(x));
            indices.push_back(std::move(y));
            indices.push_back(std::move(z));
        }
    }

    for (auto& index : indices) { index -= 1; }

    return std::make_optional(rb::Mesh(vertices, indices));
}
