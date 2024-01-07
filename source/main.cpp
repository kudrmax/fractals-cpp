#include <SFML/Graphics.hpp>
#include <iostream>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <stack>
#include <execution>

const int W = 960;
const int H = 540;
const int it_max = 2000;
float min_re = -2.5;
float max_re = 1;
float min_im = -1;
float max_im = 1;

sf::Vector2f compute_next_z(const sf::Vector2f& z, const sf::Vector2f& c) {
    float zr = z.x * z.x - z.y * z.y + c.x;
    float zi = 2.0f * z.x * z.y + c.y;
    return { zr, zi };
}

int compute_count_of_iterations(const sf::Vector2f& c, int it_max) {
    auto is_in_radius = [](const sf::Vector2f& z) { return z.x * z.x + z.y * z.y < 4.0; };
    sf::Vector2f z = { 0, 0 };
    int it = 0;
    while (it < it_max) {
        z = compute_next_z(z, c);
        if (!is_in_radius(z))
            break;
        ++it;
    }
    return it;
}

sf::Color compute_color_of_iterations(int it, int it_max) {
    int color = 1.0 * (it_max - it) / it_max * 0xff;
    return sf::Color(color, color, color);
}

void fillStack(std::stack<std::pair<int, int>>& coords) {
    for (int y = 0; y < H; ++y)
        for (int x = 0; x < W; ++x)
            coords.emplace(x, y);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(W, H), "Fractal");

    sf::Image image;
    image.create(W, H);
    sf::Texture texture;
    sf::Sprite sprite;

    std::stack<std::pair<int, int>> coords;
    fillStack(coords);

    size_t step_count = 0;
    while (window.isOpen()) {
//        std::cout << "step: " << ++step_count << std::endl;
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                std::cout << "PRESSED" << std::endl;
                float delta_x = (max_re - min_re) * 0.1;
                float delta_y = (max_im - min_im) * 0.1;
                if (event.key.code == sf::Keyboard::Left) {
                    fillStack(coords);
                    min_re -= delta_x;
                    max_re -= delta_x;
                }
                if (event.key.code == sf::Keyboard::Right) {
                    fillStack(coords);
                    min_re += delta_x;
                    max_re += delta_x;
                }
                if (event.key.code == sf::Keyboard::Up) {
                    fillStack(coords);
                    min_im -= delta_y;
                    max_im -= delta_y;
                }
                if (event.key.code == sf::Keyboard::Down) {
                    fillStack(coords);
                    min_im += delta_y;
                    max_im += delta_y;
                }
            }
            if (event.type == sf::Event::MouseButtonPressed) {

                auto zoom = [&](double scale) {

                    fillStack(coords);

                    auto new_center_r = min_re + (max_re - min_re) * event.mouseButton.x / W;
                    auto new_center_im = min_im + (max_im - min_im) * event.mouseButton.y / H;

                    auto delta_re = (max_re - min_re) / 2 / scale;
                    min_re = new_center_r - delta_re;
                    max_re = new_center_r + delta_re;

                    auto delta_im = (max_im - min_im) / 2 / scale;
                    min_im = new_center_im - delta_im;
                    max_im = new_center_im + delta_im;
                };

                if (event.mouseButton.button == sf::Mouse::Right)
                    zoom(5);
                if (event.mouseButton.button == sf::Mouse::Left)
                    zoom(1.0 / 5);
            }
        }

        window.clear();

        if (!coords.empty()) {
            for (int i = 0; i < W; i++) {
                auto [x, y] = coords.top();
                coords.pop();
                auto cr = min_re + (max_re - min_re) * x / W;
                auto ci = min_im + (max_im - min_im) * y / H;
                sf::Vector2f c = { cr, ci };
                image.setPixel(x, y, compute_color_of_iterations(compute_count_of_iterations(c, it_max), it_max));
            }
        } else {

        }

        texture.loadFromImage(image);
        sprite.setTexture(texture);
        window.draw(sprite);
        window.display();
    }

    return 0;
}