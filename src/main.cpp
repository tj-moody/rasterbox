#include "Window.hpp"

#include <glm/glm.hpp>
#include <SFML/Graphics.hpp>

#define GLM_FORCE_RADIANS

constexpr unsigned int HEIGHT = 600;
constexpr unsigned int WIDTH  = 800;

    constexpr unsigned int NUM_PIXELS
    = HEIGHT * WIDTH;

Rasterbox::Color pixel_callback(unsigned int x, unsigned int y) {
    return Rasterbox::Color(x * 256 / WIDTH, x % 256, y % 256);
}

int main() {
    Rasterbox::Window window(WIDTH, HEIGHT, "Rasterbox");

    while (window.isOpen()) {
        window.clear();

        window.writePixels(&pixel_callback);

        window.step();
    }

    return 0;
}
