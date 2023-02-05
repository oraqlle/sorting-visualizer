#ifndef SV_ALGORITHMS_CHECK
#   define SV_ALGORITHMS_CHECK

#include <SFML/Graphics.hpp>

#include <elements.hxx>
#include <sorter.hxx>
#include <viewer.hxx>

#include <chrono>
#include <thread>

using namespace std::literals;

namespace sv::algorithms
{
    auto check(
        std::shared_ptr<Elements> elems,
        std::shared_ptr<Viewer> viewer,
        Sorter* sorter
    ) -> void
    {
        auto n { elems->size() - 1uL };
        auto c { 0uL };

        viewer->mark(0uL, sf::Color::Green);

        for (; c < n; ++c)
        {
            auto val = elems->items().at(c);
            if (val > elems->items().at(c + 1uL))
            {
                viewer->mark(c + 1, sf::Color::Red);
                break;
            }

            viewer->mark(c + 1, sf::Color::Green);
            std::this_thread::sleep_for(2000.0ms / n);
        }

        sorter->sorted() = (n == c) ? true : false;

        if (sorter->sorted())
        {
            viewer->mark(n, sf::Color::Green);
            std::this_thread::sleep_for(500ms);
        }
        sorter->sorting() = false;
    }

}  /// namespace sv::algorithms

#endif  // SV_ALGORITHMS_CHECK
