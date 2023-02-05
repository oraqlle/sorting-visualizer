#include <SFML/Graphics.hpp>

#include <elements.hxx>
#include <sorter.hxx>
#include <viewer.hxx>
#include <algorithms/bubblesort.hxx>

#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

using namespace std::literals;

constexpr int width     = 1200;
constexpr int height    = 940;

auto main() -> int
{
    sf::RenderWindow window(sf::VideoMode(width, height), "SV - Sorter");
    window.setFramerateLimit(60);

    auto map = sv::Sorter::map_type{
        { "Bubble Sort"s, std::pair{ 
                "Bubblesort O(n^2) | Reading:: Red"s,
                sv::Sorter::function_type(sv::algorithms::bubblesort)
            }}
    };

    auto elems = std::make_shared<sv::Elements>(
        static_cast<sv::Elements::element_type>(height - 10),
        100uL,
        10ms,
        10ms
    );

    auto viewer = std::make_shared<sv::Viewer>(
        width,
        height,
        elems
    );

    viewer->setPosition(0.0f, 0.0f);

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
            if (event.type == sf::Event::KeyPressed)
                switch (event.key.code)
                {
                    case sf::Keyboard::Escape:
                        sorter.stop();
                        // window.close();
                        break;
                    
                    case sf::Keyboard::Enter:
                        sorter.start();
                        break;

                    case sf::Keyboard::Space:
                        sorter.select_algorithm("Shuffle"s);
                        break;

                    case sf::Keyboard::C:
                        sorter.select_algorithm("Check"s);
                        break;

                    case sf::Keyboard::B:
                        sorter.select_algorithm("Bubble Sort"s);
                        break;
                    
                    default:
                        std::clog << "No algorithm bound to that key." << std::endl;
                        break;
                }

            if (event.type == sf::Event::Closed)
                window.close();
        }

        viewer->render();

        window.clear(sf::Color::Black);
        window.draw(*viewer);
        window.display();
    }

    return 0;
}
