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

    while (window.isOpen()) {
        window.effectPass(&rb::effects::colorDemo);

        window.triangle(glm::vec2(0, 200),
                        glm::vec2(200, 0),
                        glm::vec2(200, 200),
                        rb::Color(255));

        window.step();
    }

    return 0;
}
