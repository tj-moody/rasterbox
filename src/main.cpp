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

    auto optional_mesh = rb::Assets::load_obj("obj/teapot.obj");
    if (!optional_mesh.has_value()) { return 1; }
    rb::Mesh mesh(std::move(optional_mesh.value()));

    while (window.isOpen()) {
        // window.effectPass(&rb::effects::colorDemo);

        window.fill(0);

        window.renderMesh(mesh);
        // window.drawDepthBuffer();

        window.step();

        if (window.frame_num % 3) { mesh.rotate(0.001f, glm::vec3(0, 1, 1)); }
    }


    return 0;
}
