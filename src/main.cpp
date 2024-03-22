#include "Color.hpp"
#include "Effects.hpp"
#include "Window.hpp"

#include <glm/glm.hpp>
#include <SFML/Graphics.hpp>

#define GLM_FORCE_RADIANS

constexpr unsigned int HEIGHT = 600;
constexpr unsigned int WIDTH  = 800;

constexpr unsigned int NUM_PIXELS = HEIGHT * WIDTH;

int main() {
    rb::Window window(WIDTH, HEIGHT, "Rasterbox");

    float t = 0;

    while (window.isOpen()) {
        window.effectPass(&rb::effects::colorDemo);
        window.triangle(glm::vec2((int) t % WIDTH, 200),
                        glm::vec2(200, (500 + (int) t) % HEIGHT),
                        glm::vec2(200, 200),
                        rb::Color(255));

        window.step();
        t += 0.1; // TODO: Handle framerate properly
    }

    return 0;
}
