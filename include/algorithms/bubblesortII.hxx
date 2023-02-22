#ifndef SV_ALGORITHMS_BUBBLE_SORT_II
#   define SV_ALGORITHMS_BUBBLE_SORT_II

#include <SFML/Graphics.hpp>

#include <visualizer/elements.hxx>
#include <visualizer/sorter.hxx>
#include <visualizer/viewer.hxx>

#include <functional>
#include <ranges>

namespace sv::algorithms
{
    auto bubblesortII(
        std::shared_ptr<Elements> elems,
        std::shared_ptr<Viewer> viewer
    ) noexcept -> void
    {
        auto N { elems->size() };
        auto swapped { false };

        for (auto i { 0uL }; i != (N - 1uL); ++i)
        {
            swapped = false;
            for (auto j { 0uL }; j < (N - i - 1uL); ++j)
            {
                viewer->mark(j, sf::Color::Red);
                if (elems->compare(j, j + 1uL, std::ranges::greater{}))
                {
                    elems->swap_elems(j, j + 1uL);
                    swapped = true;
                }

                viewer->unmark(j);
            }

            if (!swapped)
                break;
        }
    }

}  /// namespace sv::algorithms

#endif  // SV_ALGORITHMS_BUBBLE_SORT_II
