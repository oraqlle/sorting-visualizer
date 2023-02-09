#ifndef SV_ALGORITHMS_MERGE_SORT
#   define SV_ALGORITHMS_MERGE_SORT

#include <SFML/Graphics.hpp>

#include <visualizer/elements.hxx>
#include <visualizer/sorter.hxx>
#include <visualizer/viewer.hxx>

#include <functional>
#include <memory>
#include <ranges>
#include <vector>

namespace sv::algorithms
{
    namespace
    {
        auto merge_impl(
            std::shared_ptr<Elements> elems,
            std::shared_ptr<Viewer> viewer,
            std::size_t first,
            std::size_t middle,
            std::size_t last
        ) noexcept -> void;

        auto mergesort_impl(
            std::shared_ptr<Elements> elems,
            std::shared_ptr<Viewer> viewer,
            std::size_t first,
            std::size_t last
        ) noexcept -> void
        {
            if (first >= last)
                return;

            auto middle { (first + last) / 2uL };

            mergesort_impl(elems, viewer, first, middle);
            mergesort_impl(elems, viewer, middle + 1uL, last);

            merge_impl(elems, viewer, first, middle, last);
        }

        auto merge_impl(
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

    auto mergesort(
        std::shared_ptr<Elements> elems,
        std::shared_ptr<Viewer> viewer
    ) noexcept -> void
    {
        auto left  { 0uL };
        auto right { elems->size() - 1 };

        mergesort_impl(elems, viewer, left, right);
    }

}  /// namespace sv::algorithms

#endif  // SV_ALGORITHMS_MERGE_SORT
