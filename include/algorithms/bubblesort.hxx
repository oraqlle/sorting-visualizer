#ifndef SV_ALGORITHMS_BUBBLE_SORT
#   define SV_ALGORITHMS_BUBBLE_SORT

#include <SFML/Graphics.hpp>

#include <sv/elements.hxx>
#include <sv/sorter.hxx>
#include <sv/viewer.hxx>

#include <functional>
#include <ranges>

namespace sv::algorithms
{
    auto bubblesort(
        std::shared_ptr<Elements> elems,
        std::shared_ptr<Viewer> viewer
    ) noexcept -> void
    {
        auto N { elems->size() };
        
        for (auto i { 0uL }; i != (N - 1uL); ++i)
            for (auto j { 0uL }; j < (N - i - 1uL); ++j)
            {
                viewer->mark(j, sf::Color::Red);
                if (elems->compare(j, j + 1uL, std::ranges::greater{}))
                    elems->swap_elems(j, j + 1uL);

                viewer->unmark(j);
            }
    }

}  /// namespace sv::algorithms

#endif  // SV_ALGORITHMS_BUBBLE_SORT
