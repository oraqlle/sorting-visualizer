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
        auto quicksort_partition(
            std::shared_ptr<Elements> elems,
            std::shared_ptr<Viewer> viewer,
            long long low,
            long long high
        ) noexcept -> long long
        {
            auto N { static_cast<long long>(elems->size()) - 1LL };
            auto i { low - 1LL };
            
            for (auto j { low }; j < high; ++j)
            {
                viewer->mark_range(std::ranges::clamp(i, 0LL, N), j - 1LL, sf::Color::Blue);
                viewer->mark(j - 1LL, sf::Color::Red);

                if (elems->compare(j, high))
                {
                    i += 1LL;
                    
                    if (i != j)
                        elems->swap_elems(i, j);
                }

                viewer->unmark_range(std::ranges::clamp(i - 1LL, 0LL, N), j);
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
            if (low < high)
            {
                auto pivot = quicksort_partition(elems, viewer, low, high);
                
                viewer->mark(pivot, sf::Color::Green);

                quicksort_impl(elems, viewer, low, pivot - 1LL);
                quicksort_impl(elems, viewer, pivot + 1LL, high);

                viewer->unmark(pivot);
            }
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
