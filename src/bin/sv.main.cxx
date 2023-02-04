#include <SFML/Graphics.hpp>

#include <elements.hxx>
#include <sorter.hxx>
#include <viewer.hxx>
#include <algorithms/check.hxx>
#include <algorithms/shuffle.hxx>
#include <algorithms/bubblesort.hxx>

#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <unorderd_map>
#include <utility>

using namespace std::literals;

constexpr int width     = 1200;
constexpr int height    = 940;

auto main() -> int
{
    sf::RenderWindow window(sf::VideoMode(width, height), "SV - Sorter");
    window.setFramerateLimit(60);

    auto map = sv::Sorter::map_type{
        { "Check"s, std::pair{ 
                "Checks if the array is sorted or not. Colors green for sorted blocks and red for unsorted block."s,
                std::function{ sv::algorithms::check }
            }},
        { "Shuffle"s, std::pair{ 
                "Shuffles the blocks into a random arrangement"s,
                std::function{ sv::algorithms::shuffle }
            }},
        { "Bubble Sort"s, std::pair{ 
                "Bubblesort O(n^2) | Reading:: Red"s,
                std::function{ sv::algorithms::bubblesort }
            }}
    };

    auto elems = std::make_shared<sv::Elements>(
        static_cast<sv::Elements::element_type>(height - 10),
        100uL,
        100ms,
        100ms
    );

    auto viewer = std::make_shared<sv::Viewer>(
        width,
        height,
        elems
    );

    auto sorter = sv::Sorter{
        elems,
        viewer,
        map
    };

    while (window.isOpen())
    {
        auto event = sf::Event{};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::KeyEvent::)

            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);

                

        window.display();
    }

    return 0;
}
