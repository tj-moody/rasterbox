#include "Assets.hpp"
#include "Color.hpp"
#include "Effects.hpp"
#include "Window.hpp"

#include <glm/glm.hpp>
#include <iostream>
#include <SFML/Graphics.hpp>

#define GLM_FORCE_RADIANS

constexpr unsigned int HEIGHT = 600;
constexpr unsigned int WIDTH  = 800;

int main() {
    rb::Window window(WIDTH, HEIGHT, "Rasterbox");

    auto optional_mesh = rb::Assets::load_obj("obj/teapot.obj");
    if (!optional_mesh.has_value()) { return 1; }
    rb::Mesh mesh(std::move(optional_mesh.value()));

    // for (int i = 0; i < mesh.indices.size(); i += 3) {
    //     std::cout << i << " : "
    //               << mesh.indices[i] << ' '
    //               << mesh.indices[i + 1] << ' '
    //               << mesh.indices[i + 2] << ' '
    //               << '\n';
    //     if (mesh.indices[i] >= mesh.vertices.size()) {
    //         // std::cout << mesh.vertices.size() << '\n';
    //     }
    //     const auto vertex = mesh.vertices[mesh.indices[i]].position;
    //     // std::cout << vertex.x << '\n';
    // }
    //
    // std::cout << mesh.vertices[mesh.vertices.size() - 1].position.x;
    // return 0;

    // mesh.rotate(5 * 3.1415 / 4., glm::vec3(0, 1, 0));
    while (window.isOpen()) {
        // window.effectPass(&rb::effects::colorDemo);

        window.fill(0);

        // window.triangle(
        //     glm::vec3(200, 200, 0),
        //     glm::vec3(200, 400, 0),
        //     glm::vec3(500, 200, 0),
        //     rb::Color(255)
        // );

        window.renderMesh(mesh);
        // window.drawDepthBuffer();

        window.step();

        if (window.frame_num % 3) { mesh.rotate(0.001f, glm::vec3(0, 1, 1)); }
        // mesh.translate(glm::vec3(0, 0.0001, 0));
    }


    return 0;
}
