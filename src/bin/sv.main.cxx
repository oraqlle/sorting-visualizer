#include <SFML/Graphics.hpp>

#include <algorithms/bubblesort.hxx>
#include <algorithms/insertionsort.hxx>
#include <algorithms/mergesort.hxx>
#include <algorithms/pancakesort.hxx>
#include <algorithms/quicksort.hxx>
#include <algorithms/radixsort.hxx>
#include <algorithms/selectionsort.hxx>

#include <visualizer/elements.hxx>
#include <visualizer/sorter.hxx>
#include <visualizer/statusbar.hxx>
#include <visualizer/sound.hxx>
#include <visualizer/viewer.hxx>

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <functional>
#include <iostream>
#include <memory>
#include <source_location>
#include <string>
#include <unordered_map>
#include <utility>

namespace fs = std::filesystem;
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
                sv::algorithms::bubblesort
            }},
        { "Mergesort"s, std::pair{ 
                "Mergesort O(nlog(n)) | Reading: Red | Writing: Blue | Left: Cyan | Right: Yellow"s,
                sv::algorithms::mergesort
            }},
        { "Quicksort"s, std::pair{ 
                "Quicksort O(nlog(n)) | Reading: Red | Swapping elements: Blue | Pivot: Green"s,
                sv::algorithms::quicksort
            }},
        { "Radix Sort"s, std::pair{ 
                "Radix Sort O(d*(n + b)) | Reading: Red | Writing: Blue | Finding Max: Yellow"s,
                sv::algorithms::radixsort
            }},
        { "Insertion Sort"s, std::pair{ 
                "Insertion Sort O(n^2) | Reading: Red | Writing: Blue"s,
                sv::algorithms::insertionsort
            }},
        { "Selection Sort"s, std::pair{ 
                "Selection Sort O(n^2) | Writing: Blue | Finding Min: Yellow"s,
                sv::algorithms::selectionsort
            }},
        { "Pancake Sort"s, std::pair{ 
                "Pancake Sort O(n^2) | Reverse swap: Blue | Finding Max: Yellow"s,
                sv::algorithms::pancakesort
            }}
    };

    auto sfx = std::make_shared<sv::Sound>();

    auto elems = std::make_shared<sv::Elements>(
        static_cast<sv::Elements::element_type>(height - (0.05f * height)),
        200uL,
        10ms,
        10ms,
        sfx
    );

    auto viewer = std::make_shared<sv::Viewer>(
        width,
        height,
        elems
    );

    viewer->setPosition(0.0f, 0.0f);

    auto sorter = sv::Sorter::create(
        elems,
        viewer,
        sfx,
        map
    );

    auto statusbar = sv::Statusbar{
        width,
        0.05f * height,
        elems,
        sorter
    };

    while (window.isOpen())
    {
        viewer->render();
        statusbar.render();

        window.clear(sf::Color::Black);
        window.draw(*viewer);
        window.draw(statusbar);
        window.display();

        auto event = sf::Event{};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::KeyPressed)
                switch (event.key.code)
                {
                    case sf::Keyboard::Escape:
                        sorter->stop();
                        window.close();
                        break;
                    
                    case sf::Keyboard::Enter:
                        if (auto name { sorter->algorithm_name() }; (name != "Check"s) && (name != "Shuffle"s))
                            sorter->start();
                        break;

                    case sf::Keyboard::Space:
                        if (!sorter->sorting())
                        {
                            sorter->select_algorithm("Shuffle"s);
                            sorter->start();
                        }
                        break;

                    case sf::Keyboard::C:
                        if (!sorter->sorting())
                        {
                            sorter->select_algorithm("Check"s);
                            sorter->start();
                        }
                        break;

                    case sf::Keyboard::B:
                        sorter->select_algorithm("Bubble Sort"s);
                        break;

                    case sf::Keyboard::M:
                        sorter->select_algorithm("Mergesort"s);
                        break;

                    case sf::Keyboard::Q:
                        sorter->select_algorithm("Quicksort"s);
                        break;
                    
                    case sf::Keyboard::R:
                        sorter->select_algorithm("Radix Sort"s);
                        break;

                    case sf::Keyboard::I:
                        sorter->select_algorithm("Insertion Sort"s);
                        break;

                    case sf::Keyboard::S:
                        sorter->select_algorithm("Selection Sort"s);
                        break;

                    case sf::Keyboard::P:
                        sorter->select_algorithm("Pancake Sort"s);
                        break;
                    
                    default:
                        std::clog << "No algorithm bound to that key." << std::endl;
                        break;
                }

            if (event.type == sf::Event::Closed)
            {
                sorter->stop();
                window.close();
            }
        }
    }

    return 0;
}
