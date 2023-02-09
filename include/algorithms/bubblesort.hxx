#ifndef SV_ALGORITHMS_BUBBLE_SORT
#   define SV_ALGORITHMS_BUBBLE_SORT

#include <SFML/Graphics.hpp>

#include <visualizer/elements.hxx>
#include <visualizer/sorter.hxx>
#include <visualizer/viewer.hxx>

namespace sv::algorithms
{
    auto bubblesort(
        std::shared_ptr<Elements> elems,
        std::shared_ptr<Viewer> viewer
    ) -> void
    {
        auto n { elems->size() };

        for (auto i { 0uL }; i != (n - 1uL); ++i)
            for (auto j { 0uL }; j < (n - i - 1uL); ++j)
            {
                viewer->mark(j, sf::Color::Red);
                if (elems->compare(j , j + 1uL))
                    elems->swap_elems(j, j + 1uL);

                viewer->unmark(j);
            }
    }

}  /// namespace sv::algorithms

#endif  // SV_ALGORITHMS_BUBBLE_SORT
