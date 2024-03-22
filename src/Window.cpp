#include "Window.hpp"

#include "Color.hpp"
#include "math.h"

#include <glm/glm.hpp>


#define BACKGROUND_COLOR sf::Color(36, 37, 38, 255)

rb::Window::Window(unsigned int width,
                   unsigned int height,
                   const char* window_title)
    : window(sf::RenderWindow(sf::VideoMode(width, height), window_title)),
      width(width),
      height(height),
      // each pixel takes 4 Uint8's (RGBA)
      pixels(new sf::Uint8[width * height * 4]) {}

void rb::Window::draw() {
    // PERF: possibly attach `texture` and `sprite` as
    // private fields to only allocate once, maybe use a different data flow
    // than sf::Uint8* -> sf::Texture -> sf::Sprite -> window.draw()
    sf::Texture texture;
    texture.create(this->width, this->height);
    texture.update(this->pixels);
    sf::Sprite sprite(texture);
    this->window.draw(sprite);
    this->window.display();
}

void rb::Window::step() {
    this->handleInput();
    this->draw();
}

void rb::Window::handleInput() {
    sf::Event event;
    while (this->window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) { this->window.close(); }
    }
}

void rb::Window::setPixel(unsigned int x, unsigned int y, rb::Color color) {
    unsigned int index = y * this->width * 4 + x * 4;

    this->pixels[index]     = color.r;
    this->pixels[index + 1] = color.g;
    this->pixels[index + 2] = color.b;
    this->pixels[index + 3] = color.a;
}

void rb::Window::setPixel(glm::vec2 p, rb::Color color) {
    unsigned int index = p.y * this->width * 4 + p.x * 4;

    this->pixels[index]     = color.r;
    this->pixels[index + 1] = color.g;
    this->pixels[index + 2] = color.b;
    this->pixels[index + 3] = color.a;
}

void rb::Window::setPixel(unsigned int n, rb::Color color) {
    unsigned int index = n * 4;

    this->pixels[index]     = color.r;
    this->pixels[index + 1] = color.g;
    this->pixels[index + 2] = color.b;
    this->pixels[index + 3] = color.a;
}

bool rb::Window::isOpen() { return this->window.isOpen(); }

void rb::Window::fill(rb::Color color) {
    for (int n = 0; n < this->width * this->height; n++) {
        this->setPixel(n, color);
    }
}

void rb::Window::effectPass(void (*effect)(rb::Window& window)) {
    effect(*this);
}

int absolute(int x) {
    if (x < 0) { return -x; }
    return x;
}

// void rb::Window::line(unsigned int x0, unsigned int y0, unsigned int x1,
// unsigned int y1, rb::Color color) {
void rb::Window::line(glm::vec2 p0, glm::vec2 p1, rb::Color color) {
    int x0 = p0.x;
    int y0 = p0.y;
    int x1 = p1.x;
    int y1 = p1.y;

    bool steep = false;
    if (absolute(x0 - x1) < absolute(y0 - y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int dx = x1 - x0;
    int dy = y1 - y0;

    int derror2 = std::abs(dy) * 2;
    int error2  = 0;

    int y = y0;
    for (int x = x0; x <= x1; x++) {
        if (steep) {
            this->setPixel(y, x, color);
        } else {
            this->setPixel(x, y, color);
        }
        error2 += derror2;
        if (error2 > dx) {
            y += (y1 > y0 ? 1 : -1);
            error2 -= dx * 2;
        }
    }
}

void rb::Window::triangle(glm::vec2 p0,
                          glm::vec2 p1,
                          glm::vec2 p2,
                          rb::Color color) {
    this->line(p0, p1, color);
    this->line(p1, p2, color);
    this->line(p2, p0, color);
}

rb::Window::~Window() {}
