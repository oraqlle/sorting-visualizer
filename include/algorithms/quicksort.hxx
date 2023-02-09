#ifndef SV_ALGORITHMS_QUICK_SORT
#   define SV_ALGORITHMS_QUICK_SORT

#include <SFML/Graphics.hpp>

#include <visualizer/elements.hxx>
#include <visualizer/sorter.hxx>
#include <visualizer/viewer.hxx>

namespace sv::algorithms
{
    namespace
    {   
        auto quicksort_fix(
            std::shared_ptr<Elements> elems,
            std::shared_ptr<Viewer> viewer,
            long long low,
            long long high
        ) noexcept -> long long
        {
            auto i { low - 1LL };

            for (auto j { low }; j < high; ++j)
            {
                viewer->mark_range(i, j - 1LL, sf::Color::Blue);
                viewer->mark(j - 1LL, sf::Color::Red);

                if (elems->compare(j, high))
                {
                    i += 1LL;
                    
                    if (i != j)
                        elems->swap_elems(i, j);
                }

                viewer->unmark_range(i - 1LL, j);
            }

            elems->swap_elems(i + 1LL, high);
            return i + 1LL;
        }

        auto quicksort_impl(
            std::shared_ptr<Elements> elems,
            std::shared_ptr<Viewer> viewer,
            long long low,
            long long high
        ) noexcept -> void
        {
            if (low >= high)
                return;

            auto pivot = quicksort_fix(elems, viewer, low, high);
            
            viewer->mark(pivot, sf::Color::Green);

            quicksort_impl(elems, viewer, low, pivot - 1LL);
            quicksort_impl(elems, viewer, pivot + 1LL, high);

            viewer->unmark(pivot);
        }
    }

    auto quicksort(
        std::shared_ptr<Elements> elems,
        std::shared_ptr<Viewer> viewer
    ) noexcept -> void
    {
        auto low  { 0LL };
        auto high { elems->size() - 1 };

        quicksort_impl(elems, viewer, low, high);
    }

}  /// namespace sv::algorithms

#endif  // SV_ALGORITHMS_QUICK_SORT
