#include <SFML/Graphics.hpp>

#include <algorithm>
#include <atomic>
#include <chrono>
#include <iostream>
#include <numeric>
#include <random>
#include <ranges>
#include <thread>
#include <vector>

using namespace std::literals;

template<std::input_iterator I, std::sentinel_for<I> S>
void pancake_sort(I first, S last, long& current_pos)
{
    auto n { std::distance(first, last) };

    for (; n > 0; --n)
    {   
        auto max_elem { std::max_element(first, std::next(first, n)) };
        auto max_index { std::distance(first, max_elem) };
        current_pos = max_index;

        if (max_index != n)
        {               
            std::reverse(first, std::next(max_elem, 1));            
            std::reverse(first, std::next(first, n));
        }
        std::this_thread::sleep_for(1s);
    }
}

auto main() -> int
{

    std::vector<float> v(100, 0.0);
    std::ranges::generate(v, [&](){ return dist(eng); });

    sf::RenderWindow window(sf::VideoMode(1200, 940), "SFML works!");
    window.setFramerateLimit(60);
    auto [width, height] = window.getSize();
    auto col_width { static_cast<float>(width) / static_cast<float>(v.size()) };

    sf::RectangleShape rect(sf::Vector2f(col_width, 960.0f));
    rect.setFillColor(sf::Color::White);
    rect.setRotation(180);

    auto current_pos { 0L };
    auto sorter = std::thread{ [&](){ pancake_sort(v.begin(), v.end(), current_pos); } };

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);

        std::ranges::for_each(v, [&, n=0](const auto& x) mutable
            {
                ((n == current_pos) ? rect.setFillColor(sf::Color::Red) 
                                    : rect.setFillColor(sf::Color::White));

                // rect.setFillColor(sf::Color::White);
                rect.setPosition(static_cast<float>(n), static_cast<float>(height));
                rect.setSize(sf::Vector2f(col_width, x));
                window.draw(rect);
                n += static_cast<int>(col_width);
            }
        );

        window.display();
    }

    sorter.join();

    return 0;
}
