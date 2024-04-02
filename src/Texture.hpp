#pragma once

#include "Color.hpp"

#include <glm/glm.hpp>
#include <vector>

namespace rb {
    class Texture {
        private:
            unsigned int width, height;
            unsigned int bits_per_pixel;
            std::vector<std::uint8_t> data;

            Texture(unsigned int width,
                    unsigned int height,
                    unsigned int bits_per_pixel,
                    std::vector<std::uint8_t> data);

        public:
            Texture(const Texture& texture);

            static auto from_tga(const char* file_path) -> Texture;

            auto get(const glm::vec2& uv_coordinate) const -> rb::Color;
            auto get(unsigned int x, unsigned int y) const -> rb::Color;

            void flip_horizontally();
            void flip_vertically();

            auto get_width() const -> unsigned int;
            auto get_height() const -> unsigned int;

            auto has_alpha() const -> bool;
    };
} // namespace rb
