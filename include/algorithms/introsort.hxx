#ifndef SV_ALGORITHMS_INTRO_SORT
#   define SV_ALGORITHMS_INTRO_SORT

#include <SFML/Graphics.hpp>

#include <algorithms/heapsort.hxx>
#include <algorithms/insertionsort.hxx>
#include <algorithms/quicksort.hxx>

#include <visualizer/elements.hxx>
#include <visualizer/sorter.hxx>
#include <visualizer/viewer.hxx>

#include <cmath>
#include <functional>
#include <ranges>

namespace sv::algorithms
{
    namespace
    {
        auto introsort_insertion(
            std::shared_ptr<Elements> elems,
            std::shared_ptr<Viewer> viewer,
            long long low,
            long long high
        ) noexcept -> void
        {
            for (auto i { low + 1LL }; i < high; ++i)
            {
                viewer->mark(i, sf::Color::Red);
                auto current { elems->read(i) };
                auto j { static_cast<long long>(i) - 1LL };

                auto&& [cmp, r, w, s] = elems->counters();

                while (j >= low && elems->read(j) > current)
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

        auto introsort_make_heap(
            std::shared_ptr<Elements> elems,
            std::shared_ptr<Viewer> viewer,
            std::size_t N,
            std::size_t i
        ) noexcept -> void
        {
            auto max   { i };
            auto left  { 2uL * i + 1uL };
            auto right { 2uL * i + 2uL };

            if (left < N && elems->compare(left, max, std::ranges::greater{}))
            {
                max = left;
            }

            if (right < N && elems->compare(right, max, std::ranges::greater{}))
            {
                max = right;
            }

            if (max != i)
            {
                elems->swap_elems(i, max);
                make_heap(elems, viewer, N, max);
            }
        }

        auto introsort_heapsort(
            std::shared_ptr<Elements> elems,
            std::shared_ptr<Viewer> viewer,
            long long low,
            long long high
        ) noexcept -> void
        {
            auto rd   = std::random_device{};
            auto reng = std::mt19937{ rd() };

            auto N { static_cast<long>(high) };
            auto levels { static_cast<long>(std::floor(std::log2(N))) + 1L };

            auto last_level { 0L };
            for (auto l { 0L }; l < levels; ++l)
            {
                auto lsize { static_cast<long>(std::pow(2.0f, l)) };

                auto r { static_cast<unsigned char>(reng() % 255) };
                auto g { static_cast<unsigned char>(reng() % 255) };
                auto b { static_cast<unsigned char>(reng() % 255) };

                viewer->mark_range(last_level, lsize, sf::Color(r, g, b));
                last_level = lsize;
            }
            
            for (auto i { (N / 2L) - 1L }; i >= low; --i)
                introsort_make_heap(elems, viewer, N, i);

            for (auto i { N - 1L}; i >= 0L; --i)
            {
                viewer->mark(low, sf::Color::Magenta);
                viewer->mark(i, sf::Color::Magenta);
                elems->swap_elems(0uL, i);
                introsort_make_heap(elems, viewer, i, low);
                viewer->unmark(low);
                viewer->unmark(i);
            }
        }

        auto introsort_quicksort_pivot(
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

        auto introsort_impl(
            std::shared_ptr<Elements> elems,
            std::shared_ptr<Viewer> viewer,
            std::size_t low,
            std::size_t high,
            std::size_t maxdepth
        ) noexcept -> void
        {
            auto N { elems->size() };

            if (N < 16uL)
                introsort_insertion(elems, viewer, low, high);
            else if (maxdepth == 0uL)
                introsort_heapsort(elems, viewer, low, high);
            else
            {
                auto pivot = introsort_quicksort_pivot(elems, viewer, low, high);
                introsort_impl(elems, viewer, 0uL, pivot, maxdepth);
                introsort_impl(elems, viewer, pivot + 1uL, N - 1uL, maxdepth);
            }
        }
    }

    auto introsort(
        std::shared_ptr<Elements> elems,
        std::shared_ptr<Viewer> viewer
    ) noexcept -> void
    {
        auto N { elems->size() };
        auto maxdepth { static_cast<std::size_t>(std::floor(std::log2(N))) * 2uL };

        introsort_impl(elems, viewer, 0uL, N - 1uL, maxdepth);
    }

}  /// namespace sv::algorithms

#endif  // SV_ALGORITHMS_INTRO_SORT
