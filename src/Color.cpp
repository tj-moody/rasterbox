#include "Color.hpp"

using rb::Color;

Color::Color(std::uint8_t r, std::uint8_t g, std::uint8_t b): r(r), g(g), b(b), a(255) {}
Color::Color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a)
    : r(r), g(g), b(b), a(a) {}
Color::Color(std::uint8_t v, std::uint8_t a): r(v), g(v), b(v), a(a) {}
Color::Color(std::uint8_t v): r(v), g(v), b(v), a(255) {}
Color::Color(int v): r(v), g(v), b(v), a(255) {}
Color::Color(): r(0), g(0), b(0), a(255) {}
Color::Color(float v): r(v), g(v), b(v), a(255) {
    if (v < 0) {
        this->r = 255;
        this->b = 255;
    }
}

auto Color::value() const -> int { return (this->r + this->g + this->b) / 3; }
void Color::scale(const float v) {
    this->r *= v / 255;
    this->g *= v / 255;
    this->b *= v / 255;
}
