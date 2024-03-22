#include "Color.hpp"

using rb::Color;

Color::Color(sf::Uint8 r, sf::Uint8 g, sf::Uint8 b): r(r), g(g), b(b), a(255) {}
Color::Color(sf::Uint8 r, sf::Uint8 g, sf::Uint8 b, sf::Uint8 a)
    : r(r), g(g), b(b), a(a) {}
Color::Color(sf::Uint8 v, sf::Uint8 a): r(v), g(v), b(v), a(a) {}
Color::Color(sf::Uint8 v): r(v), g(v), b(v), a(255) {}
Color::Color(): r(0), g(0), b(0), a(255) {}
