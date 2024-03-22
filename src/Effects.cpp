#include "Effects.hpp"

#include "Color.hpp"
// #include "Main.cpp"

void rb::effects::colorDemo(rb::Window& window) {
    for (int x = 0; x < window.width; x++) {
        for (int y = 0; y < window.height; y++) {
            window.setPixel(x, y, rb::Color(x * 200 / window.width, x % 200, y % 200));
        }
    }
}


void rb::effects::stars(rb::Window& window) {
    for (int n = 0; n < window.width * window.height; n++) {
        if (std::rand() % 100 < 1) { window.setPixel(n, rb::Color(255)); }
    }
}
