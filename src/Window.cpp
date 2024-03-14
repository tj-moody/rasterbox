#include "Window.hpp"

#include "Color.hpp"


#define BACKGROUND_COLOR sf::Color(36, 37, 38, 255)

using Rasterbox::Window;

Window::Window(unsigned int width,
               unsigned int height,
               const char* window_title)
    : window(sf::RenderWindow(sf::VideoMode(width, height), window_title)),
      width(width),
      height(height),
      pixels(new sf::Uint8[width * height * 4]) {} // each pixel takes 4 Uint8's (RGBA)

void Window::draw() {
    // TODO: Investigate performance: possibly attach `texture` and `sprite` as
    // private fields to only allocate once, maybe use a different data flow
    // than sf::Uint8* -> sf::Texture -> sf::Sprite -> window.draw()
    sf::Texture texture;
    texture.create(this->width, this->height);
    texture.update(this->pixels);
    sf::Sprite sprite(texture);
    this->window.draw(sprite);
}

void Window::step() {
    sf::Event event;
    while (this->window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) { this->window.close(); }
    }

    this->draw();

    this->window.display();
}

void Window::setPix(unsigned int x, unsigned int y, Rasterbox::Color color) {
    unsigned int index = y * this->width * 4 + x * 4;

    this->pixels[index]     = color.r;
    this->pixels[index + 1] = color.g;
    this->pixels[index + 2] = color.b;
    this->pixels[index + 3] = color.a;
}

void Window::setPix(unsigned int n, Rasterbox::Color color) {
    unsigned int index = n * 4;

    this->pixels[index]     = color.r;
    this->pixels[index + 1] = color.g;
    this->pixels[index + 2] = color.b;
    this->pixels[index + 3] = color.a;
}

bool Window::isOpen() { return this->window.isOpen(); }

void Window::clear() { this->window.clear(BACKGROUND_COLOR); }

void Window::writePixels(Rasterbox::Color (*pixelCallback)(unsigned int, unsigned int)) {
    for (int x = 0; x < this->width; x++) {
        for (int y = 0; y < this->height; y++) {
            Rasterbox::Color color(pixelCallback(x, y));
            this->setPix(x, y, color);
        }
    }
}

Window::~Window() {}
