#include <SFML/Graphics.hpp>


const int W = 1920 / 2;
const int H = 1080 / 2;

sf::Vector2f compute_next_z(sf::Vector2f z, sf::Vector2f c) {
    auto zr = z.x * z.x - z.y * z.y;
    auto zi = 2.0f * z.x * z.y;
    return sf::Vector2f{ zr, zi } + c;
};

int compute_count_of_iterations(sf::Vector2f c, int max_iter) {
    auto is_in_radius = [](sf::Vector2f z) { return z.x * z.x + z.y * z.y < 4.0; };
    sf::Vector2f z = { 0, 0 };
    int it = 0;
    for (; it < max_iter && is_in_radius(z); it++)
        z = compute_next_z(z, c);
    return it;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(W, H), "Fractal");

    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;
    image.create(W, H);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        for (int y = 0; y < H; ++y)
            for (int x = 0; x < W; ++x) {

            }

        window.display();
    }

    return 0;
}