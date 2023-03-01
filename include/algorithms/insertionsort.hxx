#ifndef SV_ALGORITHMS_INSERTION_SORT
#   define SV_ALGORITHMS_INSERTION_SORT

#include <SFML/Graphics.hpp>

#include <sv/elements.hxx>
#include <sv/viewer.hxx>

#include <functional>
#include <ranges>

namespace sv::algorithms
{
    auto insertionsort(
        std::shared_ptr<Elements> elems,
        std::shared_ptr<Viewer> viewer
    ) noexcept -> void
    {
        for (auto i { 1LL }; i < static_cast<long long>(elems->size()); ++i)
        {
            viewer->mark(i, sf::Color::Red);
            auto current { elems->read(i) };
            auto j { static_cast<long long>(i) - 1LL };

            auto&& [cmp, r, w, s] = elems->counters();

            while (j >= 0LL && elems->read(j) > current)
            {
                viewer->mark(j + 1LL, sf::Color::Blue);
                elems->write(j + 1LL, elems->read(j));
                viewer->unmark(j + 1LL);
                j -= 1LL;
                cmp += 1uL;
            }

            cmp += 1uL;
            elems->write(j + 1LL, current);
            viewer->unmark(i);
        }
    }

}  /// namespace sv::algorithms

#endif  // SV_ALGORITHMS_INSERTION_SORT
