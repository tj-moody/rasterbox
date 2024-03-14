#pragma once

#include <SFML/Graphics.hpp>

namespace Rasterbox {
    class Color {
        public:
            const sf::Uint8 r, g, b, a;

            Color(sf::Uint8 r, sf::Uint8 g, sf::Uint8 b);
            Color(sf::Uint8 r, sf::Uint8 g, sf::Uint8 b, sf::Uint8 a);
    };
} // namespace Rasterbox
