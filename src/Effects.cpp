#include "Effects.hpp"

void rb::effects::color_demo(rb::Window& window) {
    for (int x = 0; x < window.width; x++) {
        for (int y = 0; y < window.height; y++) {
            window.set_pixel(x, y, rb::Color(x * 200 / window.width, x % 200, y % 200));
        }
    }
}


void rb::effects::stars(rb::Window& window) {
    for (int n = 0; n < window.width * window.height; n++) {
        if (std::rand() % 100 < 1) { window.set_pixel(n, rb::Color(255)); }
    }
}
