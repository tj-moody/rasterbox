#include "Assets.hpp"
#include "Color.hpp"
#include "Effects.hpp"
#include "Window.hpp"

#include <glm/glm.hpp>
#include <SFML/Graphics.hpp>

#define GLM_FORCE_RADIANS

constexpr unsigned int HEIGHT = 600;
constexpr unsigned int WIDTH  = 800;

int main() {
    rb::Window window(WIDTH, HEIGHT, "Rasterbox");

    auto optional_mesh = rb::Assets::load_obj("obj/head.obj");
    if (!optional_mesh.has_value()) { return 1; }
    rb::Mesh mesh(std::move(optional_mesh.value()));

    const glm::vec3 rotation_axis(0.3, 2, -0.1);

    while (window.isOpen()) {
        window.fill(0);

        window.renderMesh(mesh);
        mesh.rotate(0.01f, rotation_axis);

        window.step();
    }

    return 0;
}
