#pragma once

#include <SFML/Graphics.hpp>
#include <cstdint>

namespace rb {
    class Color {
        public:
            std::uint8_t r, g, b, a;

            Color(std::uint8_t r, std::uint8_t g, std::uint8_t b);
            Color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a);
            Color(std::uint8_t v, std::uint8_t a);
            Color(std::uint8_t v);
            Color(float v);
            Color(int v);
            Color();

            auto value() const -> int;
            void scale(const float);
    };
} // namespace rb
