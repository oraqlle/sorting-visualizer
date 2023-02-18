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

auto main() -> int
{
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "SV - Sorter", sf::Style::Fullscreen);
    window.setFramerateLimit(120);
    const auto [width, height] = static_cast<sf::Vector2f>(window.getSize());
    const auto v_width { width - (width * 0.15f) };
    const auto max_num_elements { static_cast<std::size_t>(v_width * (5.0f / 48.0f)) };

    auto map = sv::Sorter::map_type{
        { "Bubble Sort"s, std::tuple{
                "B"s,
                std::vector{ 
                    " Time Complexity: O(n^2)"s,
                    " Read & Swap: Red"s 
                },
                sv::algorithms::bubblesort
            }},
        { "Mergesort"s, std::tuple{ 
                "M"s,
                std::vector{ 
                    " Time Complexity: O(nlog(n))"s,
                    " Reading: Red"s,
                    " Writing: Blue"s,
                    " Left Bound: Cyan"s,
                    " Right Bound: Yellow"s
                },
                sv::algorithms::mergesort
            }},
        { "Quicksort"s, std::tuple{ 
                "Q"s,
                std::vector{
                    " Time Complexity: O(nlog(n))"s,
                    " Reading: Red"s,
                    " Swapping: Blue"s,
                    " Pivot: Green"s
                },
                sv::algorithms::quicksort
            }},
        { "Radix Sort"s, std::tuple{ 
                "R"s,
                std::vector{
                    " Time Complexity: O(d*(n + b))"s,
                    " Reading: Red"s,
                    " Writing: Blue"s,
                    " Find Maximum: Yellow"s
                },
                sv::algorithms::radixsort
            }},
        { "Insertion Sort"s, std::tuple{ 
                "I"s,
                std::vector{
                    " Time Complexity: O(n^2)"s,
                    " Reading: Red"s,
                    " Writing: Blue"s
                },
                sv::algorithms::insertionsort
            }},
        { "Selection Sort"s, std::tuple{ 
                "S"s,
                std::vector{
                    " Time Complexity: O(n^2)"s,
                    " Writing: Blue"s,
                    " Finding Minimum: Yellow"s
                },
                sv::algorithms::selectionsort
            }},
        { "Pancake Sort"s, std::tuple{ 
                "P"s,
                std::vector{
                    " Time Complexity: O(n^2)"s,
                    " Reverse Swap: Blue"s,
                    " Finding Minimum: Yellow"s
                },
                sv::algorithms::pancakesort
            }}
    };

    auto sfx = std::make_shared<sv::Sound>();

    auto elems = std::make_shared<sv::Elements>(
        height - (0.005f * height),
        max_num_elements,
        10ms,
        10ms,
        sfx
    );

    auto viewer = std::make_shared<sv::Viewer>(
        v_width,
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
        (width * 0.15f) - 10.0f,
        height,
        14u,
        elems,
        sorter
    };

    viewer->setPosition(0.0f, 0.0f);
    statusbar.setPosition(v_width + 10.0f, 0.0f);

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

                    case sf::Keyboard::Up:
                        sorter->adjust_delay(5ms, 0ms);
                        break;

                    case sf::Keyboard::Down:
                        sorter->adjust_delay(-5ms, 0ms);
                        break;

                    case sf::Keyboard::Right:
                        sorter->adjust_delay(0ms, 5ms);
                        break;

                    case sf::Keyboard::Left:
                        sorter->adjust_delay(0ms, -5ms);
                        break;

                    case sf::Keyboard::LBracket:
                        if (auto csize { elems->size() }; csize != 0uL) [[likely]]
                            sorter->resize(csize - 1uL);
                        else
                            std::clog << "The number of elements cannot be lower than 0!" << std::endl;
                        break;

                    case sf::Keyboard::RBracket:
                        if (auto csize { elems->size() }; csize != max_num_elements) [[likely]]
                            sorter->resize(csize + 1uL);
                        else
                            std::clog << "The number of elements cannot be higher than "<< max_num_elements << "!" << std::endl;
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
