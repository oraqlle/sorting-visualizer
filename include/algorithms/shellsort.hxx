#ifndef SV_ALGORITHMS_SHELL_SORT
#   define SV_ALGORITHMS_SHELL_SORT

#include <SFML/Graphics.hpp>

#include <visualizer/elements.hxx>
#include <visualizer/sorter.hxx>
#include <visualizer/viewer.hxx>

#include <functional>
#include <ranges>

namespace sv::algorithms
{
    auto shellsort(
        std::shared_ptr<Elements> elems,
        std::shared_ptr<Viewer> viewer
    ) noexcept -> void
    {
        auto N { elems->size() };

        for (auto gap { N / 2uL }; gap > 0uL; gap /= 2uL)
            for (auto i { gap }; i < N; i += 1uL)
            {
                viewer->mark(i, sf::Color::Red);
                auto&& [rd, wr, cp, sw] = elems->counters();

                auto t { elems->read(i) };
                auto j { 0uL };
                for (j = i; j >= gap && elems->read(j - gap) > t; j -= gap)
                {
                    cp += 1uL;
                    elems->write(j, elems->read(j - gap));
                }

                viewer->mark(j, sf::Color::Blue);
                elems->write(j, t);
                viewer->unmark(j);
                viewer->unmark(i);
            }
    }

}  /// namespace sv::algorithms

#endif  // SV_ALGORITHMS_SHELL_SORT
