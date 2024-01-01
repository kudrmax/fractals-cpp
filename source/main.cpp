#include <SFML/Graphics.hpp>
#include <iostream>

const int W = 960;
const int H = 540;
const int it_max = 1000;
float min_re = -2.5;
float max_re = 1;
float min_im = -1;
float max_im = 1;

sf::Vector2f compute_next_z(sf::Vector2f z, sf::Vector2f c) {
    float zr = z.x * z.x - z.y * z.y + c.x;
    float zi = 2.0f * z.x * z.y + c.y;
    return sf::Vector2f{ zr, zi };
}

int compute_count_of_iterations(sf::Vector2f c, int it_max) {
    auto is_in_radius = [](const sf::Vector2f& z) { return z.x * z.x + z.y * z.y < 4.0; };
    sf::Vector2f z = { 0, 0 };
    int it = 0;
    for (; it < it_max && is_in_radius(z); it++)
        z = compute_next_z(z, c);
    return it;
}

sf::Color compute_color_of_iterations(int it, int it_max) {
    int color = 1.0 * (it_max - it) / it_max * 0xff;
    return sf::Color(color, color, color);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(W, H), "Fractal");

    sf::Image image;
    image.create(W, H);
    sf::Texture texture;
    sf::Sprite sprite;

    while (window.isOpen()) {
        std::cout << "new step" << std::endl;
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        for (int y = 0; y < H; ++y) {
            for (int x = 0; x < W; ++x) {
                auto cr = min_re + (max_re - min_re) * x / W;
                auto ci = min_im + (max_im - min_im) * y / H;
                sf::Vector2f c = { cr, ci };
                auto it = compute_count_of_iterations(c, it_max);
                auto color = compute_color_of_iterations(it, it_max);
                image.setPixel(x, y, color);
            }
        }
        texture.loadFromImage(image);
        sprite.setTexture(texture);
        window.draw(sprite);
        window.display();
    }

    return 0;
}