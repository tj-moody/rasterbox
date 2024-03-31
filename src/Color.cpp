#include "Color.hpp"

using rb::Color;

Color::Color(sf::Uint8 r, sf::Uint8 g, sf::Uint8 b): r(r), g(g), b(b), a(255) {}
Color::Color(sf::Uint8 r, sf::Uint8 g, sf::Uint8 b, sf::Uint8 a)
    : r(r), g(g), b(b), a(a) {}
Color::Color(sf::Uint8 v, sf::Uint8 a): r(v), g(v), b(v), a(a) {}
Color::Color(sf::Uint8 v): r(v), g(v), b(v), a(255) {}
Color::Color(int v): r(v), g(v), b(v), a(255) {}
Color::Color(): r(0), g(0), b(0), a(255) {}
Color::Color(float v): r(v), g(v), b(v), a(255) {
    if (v < 0) {
        this->r = 255;
        this->b = 255;
    }
}

auto Color::value() const -> int { return (this->r + this->g + this->b) / 3; }
void Color::scale(const rb::Color c) {
    this->r *= (float) c.r / 255;
    this->g *= (float) c.g / 255;
    this->b *= (float) c.b / 255;
}
