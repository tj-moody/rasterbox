#include "Color.hpp"

using Rasterbox::Color;

Color::Color(sf::Uint8 r, sf::Uint8 g, sf::Uint8 b): r(r), g(g), b(b), a(255) {}
Color::Color(sf::Uint8 r, sf::Uint8 g, sf::Uint8 b, sf::Uint8 a): r(r), g(g), b(b), a(a) {}
