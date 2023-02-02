#include <SFML/Graphics.hpp>

#include <algorithm>
#include <numeric>
#include <random>
#include <ranges>
#include <vector>

auto main() -> int
{
    std::random_device r{};
    std::default_random_engine eng{ r() };
    std::uniform_real_distribution<long double> dist{ 0.0, 950.0 };

    std::vector<long double> v(0.0, 100);
    std::ranges::generate(v, [&](){ return dist(eng); });

    sf::RenderWindow window(sf::VideoMode(1200, 960), "SFML works!");
    window.setFramerateLimit(60);
    auto [width, height] = window.getSize();

    sf::RectangleShape rect(sf::Vector2f(2.0, 960.0));
    rect.setFillColor(sf::Color(255, 255, 255));
    rect.setPosition(sf::Vector2f(1.0, static_cast<float>(height)));
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
                rect.setFillColor(sf::Color(235, 149, 13));
                ///
                rect.setPosition(static_cast<float>(n), static_cast<float>(height));
                rect.setSize(sf::Vector2f(2.0, x));
                rect.setRotation(180);
                window.draw(rect);
            }
        );

        window.display();
    }

    return 0;
}
