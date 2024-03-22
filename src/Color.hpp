#pragma once

#include <SFML/Graphics.hpp>

namespace rb {
    class Color {
        public:
            sf::Uint8 r, g, b, a;

            Color(sf::Uint8 r, sf::Uint8 g, sf::Uint8 b);
            Color(sf::Uint8 r, sf::Uint8 g, sf::Uint8 b, sf::Uint8 a);
            Color(sf::Uint8 v, sf::Uint8 a);
            Color(sf::Uint8 v);
            Color();
    };
} // namespace rb
