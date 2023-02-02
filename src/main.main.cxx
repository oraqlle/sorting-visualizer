#include <SFML/Graphics.hpp>

#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <ranges>
#include <vector>

auto main() -> int
{
    std::random_device r{};
    std::default_random_engine eng{ r() };
    std::uniform_real_distribution<long double> dist{ 0.0, 950.0 };

    std::vector<float> v(600, 0.0);
    std::ranges::generate(v, [&](){ return dist(eng); });

    sf::RenderWindow window(sf::VideoMode(1200, 960), "SFML works!");
    window.setFramerateLimit(60);
    auto [width, height] = window.getSize();

    sf::RectangleShape rect(sf::Vector2f(2.0f, 960.0f));
    rect.setFillColor(sf::Color::White);
    rect.setPosition(sf::Vector2f(15.0f, static_cast<float>(height)));
    rect.setRotation(180);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);

        std::ranges::for_each(v, [n=0, &rect, &window, &height](const auto& x) mutable
            {
                rect.setFillColor(sf::Color::White);
                ///
                rect.setPosition(static_cast<float>(n), static_cast<float>(height));
                rect.setSize(sf::Vector2f(2.0f, x));
                window.draw(rect);
                n += 2;
            }
        );

        window.display();
    }

    return 0;
}
