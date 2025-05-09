#include "Texture.hpp"

#include <cstring>
#include <fstream>


rb::Texture::Texture(unsigned int width,
                     unsigned int height,
                     unsigned int bits_per_pixel,
                     std::vector<std::uint8_t> data)
    : width(width),
      height(height),
      bits_per_pixel(bits_per_pixel),
      data(std::move(data)) {}

rb::Texture::Texture(const Texture& texture) = default;

auto rb::Texture::get(const glm::vec2& uv_coordinate) const -> rb::Color {
    const unsigned int x               = uv_coordinate.x * this->width;
    const unsigned int y               = uv_coordinate.y * this->height;
    const unsigned int bytes_per_pixel = this->bits_per_pixel / 8;
    const unsigned int i = bytes_per_pixel * (y * this->width + x);
    if (this->has_alpha()) {
        return rb::Color(this->data[i + 0],
                         this->data[i + 1],
                         this->data[i + 2],
                         this->data[i + 3]);
    } else {
        // clang-format off
        return rb::Color(
            this->data[i + 0],
            this->data[i + 1],
            this->data[i + 2]
        );
        // clang-format on
    }
}

auto rb::Texture::get(unsigned int x, unsigned int y) const -> rb::Color {
    const unsigned int bytes_per_pixel = this->bits_per_pixel / 8;
    const unsigned int i = bytes_per_pixel * (y * this->width + x);
    if (this->bits_per_pixel == 24) {
        // clang-format off
        return rb::Color(
            this->data[i + 0],
            this->data[i + 1],
            this->data[i + 2]
        );
        // clang-format on
    } else if (this->bits_per_pixel == 32) {
        return rb::Color(this->data[i + 0],
                         this->data[i + 1],
                         this->data[i + 2],
                         this->data[i + 3]);
    } else {
        throw "Invalid texture format";
    }
}

auto rb::Texture::has_alpha() const -> bool {
    return this->bits_per_pixel == 32;
}

auto rb::Texture::get_width() const -> unsigned int { return this->width; }
auto rb::Texture::get_height() const -> unsigned int { return this->height; }

void rb::Texture::flip_vertically() {
    std::vector<std::uint8_t> copy;
    copy.reserve(this->width * this->height * sizeof(std::uint32_t));
    const unsigned int bytes_per_pixel = this->bits_per_pixel / 8;
    for (unsigned int y = 0; y < this->height; y++) {
        for (unsigned int x = 0; x < this->width; x++) {
            for (unsigned int i = 0; i < bytes_per_pixel; i++) {
                // clang-format off
                copy[i + bytes_per_pixel * (x + this->width * y)] = this->data[
                     i + bytes_per_pixel * (x + this->width * (this->height - y - 1))];
                // clang-format on
            }
        }
    }
    this->data = std::move(copy);
}

void rb::Texture::flip_horizontally() {
    std::vector<std::uint8_t> copy;
    copy.reserve(this->width * this->height * sizeof(std::uint32_t));
    const unsigned int bytes_per_pixel = this->bits_per_pixel / 8;
    for (unsigned int y = 0; y < this->height; y++) {
        for (unsigned int x = 0; x < this->width; x++) {
            for (unsigned int i = 0; i < bytes_per_pixel; i++) {
                // clang-format off
                copy[i + bytes_per_pixel * (this->width * y + x)] = this->data[
                     i + bytes_per_pixel * (this->width * y + (this->width - x - 1))];
                // clang-format on
            }
        }
    }
    this->data = std::move(copy);
}

union PixelInfo {
        std::uint32_t Bits;
        struct {
                std::uint8_t R, G, B, A;
        } RGBA;
};

// From
// https://stackoverflow.com/questions/20595340/loading-a-tga-bmp-file-in-c-opengl
auto rb::Texture::from_tga(const char* file_path) -> rb::Texture {
    std::fstream hFile(file_path, std::ios::in | std::ios::binary);
    if (!hFile.is_open()) { throw std::invalid_argument("File Not Found."); }

    std::uint8_t Header[18] = { 0 };
    std::vector<std::uint8_t> image_data;
    static std::uint8_t DeCompressed[12]
        = { 0x0, 0x0, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
    static std::uint8_t IsCompressed[12]
        = { 0x0, 0x0, 0xA, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };

    hFile.read(reinterpret_cast<char*>(&Header), sizeof(Header));

    unsigned int bits_per_pixel, width, height, size;
    if (!std::memcmp(DeCompressed, &Header, sizeof(DeCompressed))) {
        bits_per_pixel = Header[16];
        width          = Header[13] * 256 + Header[12];
        height         = Header[15] * 256 + Header[14];
        size           = ((width * bits_per_pixel + 31) / 32) * 4 * height;

        if ((bits_per_pixel != 24) && (bits_per_pixel != 32)) {
            hFile.close();
            throw std::invalid_argument(
                "Invalid File Format. Required: 24 or 32 Bit Image.");
        }

        image_data.resize(size);
        hFile.read(reinterpret_cast<char*>(image_data.data()), size);
    } else if (!std::memcmp(IsCompressed, &Header, sizeof(IsCompressed))) {
        bits_per_pixel = Header[16];
        width          = Header[13] * 256 + Header[12];
        height         = Header[15] * 256 + Header[14];

        if ((bits_per_pixel != 24) && (bits_per_pixel != 32)) {
            hFile.close();
            throw std::invalid_argument(
                "Invalid File Format. Required: 24 or 32 Bit Image.");
        }

        PixelInfo pixel           = { 0 };
        unsigned int current_byte = 0;
        std::size_t current_pixel = 0;
        std::uint8_t chunk_header = { 0 };
        int bytes_per_pixel       = bits_per_pixel / 8;
        image_data.resize(width * height * sizeof(PixelInfo));

        do {
            hFile.read(reinterpret_cast<char*>(&chunk_header),
                       sizeof(chunk_header));

            if (chunk_header < 128) {
                ++chunk_header;
                for (int I = 0; I < chunk_header; ++I, ++current_pixel) {
                    hFile.read(reinterpret_cast<char*>(&pixel),
                               bytes_per_pixel);


                    image_data[current_byte++] = pixel.RGBA.B;
                    image_data[current_byte++] = pixel.RGBA.G;
                    image_data[current_byte++] = pixel.RGBA.R;
                    if (bits_per_pixel == 32) {
                        image_data[current_byte++] = pixel.RGBA.A;
                    }
                }
            } else {
                chunk_header -= 127;
                hFile.read(reinterpret_cast<char*>(&pixel), bytes_per_pixel);

                for (int I = 0; I < chunk_header; ++I, ++current_pixel) {
                    image_data[current_byte++] = pixel.RGBA.B;
                    image_data[current_byte++] = pixel.RGBA.G;
                    image_data[current_byte++] = pixel.RGBA.R;
                    if (bits_per_pixel == 32) {
                        image_data[current_byte++] = pixel.RGBA.A;
                    }
                }
            }
        } while (current_pixel < (width * height));
    } else {
        hFile.close();
        throw std::invalid_argument(
            "Invalid File Format. Required: 24 or 32 Bit TGA File.");
    }

    hFile.close();
    return rb::Texture(width, height, bits_per_pixel, image_data);
}
