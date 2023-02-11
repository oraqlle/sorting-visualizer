#ifndef SV_ALGORITHMS_PANCAKE_SORT
#   define SV_ALGORITHMS_PANCAKE_SORT

#include <SFML/Graphics.hpp>

#include <visualizer/elements.hxx>
#include <visualizer/sorter.hxx>
#include <visualizer/viewer.hxx>

namespace sv::algorithms
{
    namespace
    {
        auto pancake_max(
            std::shared_ptr<Elements> elems,
            std::shared_ptr<Viewer> viewer,
            std::size_t first,
            std::size_t last
        ) noexcept -> std::size_t
        {
            auto maxidx { first };

            for (; first < last; ++first)
            {
                viewer->mark(first, sf::Color::Yellow);

                if (elems->compare(first, maxidx, std::ranges::greater{}))
                    maxidx = first;

                viewer->unmark(first);
            }

            return maxidx;
        }

        auto pancake_reverse(
            std::shared_ptr<Elements> elems,
            std::shared_ptr<Viewer> viewer,
            std::size_t first,
            std::size_t last
        ) noexcept -> void
        {
            if (first == last)
                return;

            for (; first < last; ++first, --last)
            {
                viewer->mark(first, sf::Color::Blue);
                viewer->mark(last, sf::Color::Blue);

                elems->swap_elems(first, last);

                viewer->unmark(first);
                viewer->unmark(last);
            }
        }
    }

    auto pancakesort(
        std::shared_ptr<Elements> elems,
        std::shared_ptr<Viewer> viewer
    ) noexcept -> void
    {
        for (auto n { elems->size() }; n > 1uL; --n)
        {
            auto maxidx { pancake_max(elems, viewer, 0uL, n) };

            if (maxidx != n)
            {
                pancake_reverse(elems, viewer, 0uL, maxidx);
                pancake_reverse(elems, viewer, 0uL, n - 1uL);
            }
        }
    }

}  /// namespace sv::algorithms

#endif  // SV_ALGORITHMS_PANCAKE_SORT
