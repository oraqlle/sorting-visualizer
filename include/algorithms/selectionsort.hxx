#ifndef SV_ALGORITHMS_SELECTION_SORT
#   define SV_ALGORITHMS_SELECTION_SORT

#include <SFML/Graphics.hpp>

#include <sv/elements.hxx>
#include <sv/viewer.hxx>

namespace sv::algorithms
{
    auto selectionsort(
        std::shared_ptr<Elements> elems,
        std::shared_ptr<Viewer> viewer
    ) noexcept -> void
    {
        auto minidx { 0uL };

        for (auto i { 0uL }, n { elems->size() }; i < n; ++i)
        {
            minidx = i;

            for (auto j { i + 1uL }; j < n; ++j)
            {
                viewer->mark(j, sf::Color::Yellow);
                
                if (elems->compare(j, minidx))
                    minidx = j;

                viewer->unmark(j);
            }

            if (minidx != i)
            {
                viewer->mark(i, sf::Color::Blue);
                elems->swap_elems(i, minidx);
                viewer->unmark(i);
            }
        }
    }

}  /// namespace sv::algorithms

#endif  // SV_ALGORITHMS_SELECTION_SORT
