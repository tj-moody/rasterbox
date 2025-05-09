#include "Color.hpp"
#include "Effects.hpp"
#include "Window.hpp"

#include <glm/glm.hpp>
#include <SFML/Graphics.hpp>

#define GLM_FORCE_RADIANS

constexpr unsigned int WIDTH  = 800;
constexpr unsigned int HEIGHT = 600;

auto main() -> int {
    rb::Window window(WIDTH, HEIGHT, "Rasterbox");
    // window.set_limit_framerate(60);

    auto optional_mesh = rb::Mesh::from_obj("obj/head.obj");
    if (!optional_mesh.has_value()) { return 1; }
    rb::Mesh mesh(std::move(optional_mesh.value()));

    const glm::vec3 rotation_axis(0.3, 2, -0.1);

    mesh.rotate(M_PI, glm::vec3(0, 1, 0));
    mesh.translate(glm::vec3(0, 0, 1.5f));

    while (window.is_open()) {
        window.fill(0);

        window.render_mesh(mesh);
        mesh.rotate(0.005f, rotation_axis);
        // mesh.translate(glm::vec3(0, 0.001f, -0.001f));

        // window.draw_depth_buffer();
        window.step();
    }

    return 0;
}
