#ifndef SV_ALGORITHMS_TIM_SORT
#   define SV_ALGORITHMS_TIm_SORT

#include <SFML/Graphics.hpp>

#include <sv/elements.hxx>
#include <sv/viewer.hxx>

#include <functional>
#include <ranges>

namespace sv::algorithms
{
    namespace
    {
        auto timsort_insertion(
            std::shared_ptr<Elements> elems,
            std::shared_ptr<Viewer> viewer,
            long long low,
            long long high
        ) noexcept -> void
        {
            for (auto i { low + 1LL }; i <= high; ++i)
            {
                viewer->mark(i, sf::Color::Yellow);
                auto current { elems->read(i) };
                auto j { static_cast<long long>(i) - 1LL };

                auto&& [cmp, r, w, s] = elems->counters();

                while (j >= low && elems->read(j) > current)
                {
                    viewer->mark(j + 1LL, sf::Color::Cyan);
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
    
        auto timsort_merge(
            std::shared_ptr<Elements> elems,
            std::shared_ptr<Viewer> viewer,
            std::size_t first,
            std::size_t middle,
            std::size_t last
        ) noexcept -> void
        {
            viewer->mark(first, sf::Color::Cyan);
            viewer->mark(last, sf::Color::Yellow);

            auto first_n { middle - first + 1uL };
            auto last_n  { last - middle };

            using element_type = typename decltype(elems)::element_type::element_type;
            auto left   = std::vector<element_type>(first_n, element_type{});
            auto right  = std::vector<element_type>(last_n,  element_type{});

            for (auto i { 0uL }; i < first_n; ++i)
            {
                viewer->mark(first + 1uL + i, sf::Color::Red);
                left.at(i) = elems->read(first + i);
                viewer->unmark(first + 1uL + i);
            }

            for (auto i { 0uL }; i < last_n; ++i)
            {
                viewer->mark(middle + 1uL + i, sf::Color::Red);
                right.at(i) = elems->read(middle + 1uL + i);
                viewer->unmark(middle + 1uL + i);
            }

            auto i { 0uL };
            auto j { 0uL };
            auto k { first };

            while (i < first_n && j < last_n)
            {
                viewer->mark(k - 1uL, sf::Color::Blue);

                auto&& [cpm, r, w, s] = elems->counters();

                if (left.at(i) <= right.at(j))
                {
                    elems->write(k, left.at(i));
                    i += 1uL;
                }
                else
                {   
                    elems->write(k, right.at(j));
                    j += 1uL;
                }

                cpm += 1uL;
                viewer->unmark(k - 1uL);
                k += 1uL;

            }

            while (i < first_n)
            {
                viewer->mark(k - 1uL, sf::Color::Blue);
                elems->write(k, left.at(i));
                i += 1uL;
                viewer->unmark(k - 1uL);
                k += 1uL;
            }

            while (j < last_n)
            {
                viewer->mark(k - 1uL, sf::Color::Blue);
                elems->write(k, right.at(j));
                j += 1uL;
                viewer->unmark(k - 1uL);
                k += 1uL;
            }

            viewer->unmark(first);
            viewer->unmark(last);
        }
    }

    auto timsort(
        std::shared_ptr<Elements> elems,
        std::shared_ptr<Viewer> viewer
    ) noexcept -> void
    {
        constexpr auto runs { 32uL };
        auto N { elems->size() };
        
        for (auto i { 0uL }; i < N; i += runs)
        {   
            viewer->mark(i, sf::Color::Magenta);
            timsort_insertion(elems, viewer, i, std::ranges::min(i + runs - 1uL, N - 1uL));
            viewer->unmark(i);
        }

        for (auto size { runs }; size < N; size *= 2uL)
            for (auto left { 0uL }; left < N; left += 2uL * size)
            {
                auto mid { left + size - 1uL };
                auto right = std::ranges::min(left + (2uL * size) - 1uL, N - 1uL);

                if (mid < right)
                    timsort_merge(elems, viewer, left, mid, right);
            }
    }

}  /// namespace sv::algorithms

#endif  // SV_ALGORITHMS_TIM_SORT
