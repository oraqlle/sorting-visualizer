#ifndef SV_ALGORITHMS_COUNTING_SORT
#   define SV_ALGORITHMS_COUNTING_SORT

#include <SFML/Graphics.hpp>

#include <sv/elements.hxx>
#include <sv/sorter.hxx>
#include <sv/viewer.hxx>

#include <functional>
#include <ranges>
#include <vector>

namespace sv::algorithms
{
    namespace
    {
        auto countingsort_min(
            std::shared_ptr<Elements> elems,
            std::shared_ptr<Viewer> viewer,
            std::size_t first,
            std::size_t last
        ) noexcept -> sv::Elements::element_type
        {
            auto minidx { first };

            for (auto i { 0uL }; i < last; ++i)
            {
                viewer->mark(i, sf::Color::Yellow);

                if (elems->compare(i, minidx))
                    minidx = i;

                viewer->unmark(i);
            }

            return elems->read(minidx);
        }
        
        auto countingsort_max(
            std::shared_ptr<Elements> elems,
            std::shared_ptr<Viewer> viewer,
            std::size_t first,
            std::size_t last
        ) noexcept -> sv::Elements::element_type
        {
            auto maxidx { first };

            for (auto i { 0uL }; i < last; ++i)
            {
                viewer->mark(i, sf::Color::Cyan);

                if (elems->compare(i, maxidx, std::ranges::greater{}))
                    maxidx = i;

                viewer->unmark(i);
            }

            return elems->read(maxidx);
        }
    }

    auto countingsort(
        std::shared_ptr<Elements> elems,
        std::shared_ptr<Viewer> viewer
    ) noexcept -> void
    {
        using sort_type     = unsigned int;
        using element_type  = Elements::element_type;

        auto N { static_cast<long long>(elems->size()) };
        
        auto min { static_cast<sort_type>(countingsort_min(elems, viewer, 0LL, N)) };
        auto max { static_cast<sort_type>(countingsort_max(elems, viewer, 0LL, N)) };
        auto range { max - min + 1uL };

        auto count  = std::vector(range, std::size_t{});
        auto output = std::vector(N, element_type{});

        for (auto i { 0LL }; i < N; ++i)
        {
            viewer->mark(i, sf::Color::Red);
            count.at(static_cast<sort_type>(elems->read(i)) - min) += 1uL;
            viewer->unmark(i);
        }

        for (auto i { 1LL }; i < static_cast<long long>(count.size()); ++i)
            count.at(i) += count.at(i - 1uL);

        for (auto i { N - 1LL }; i >= 0LL; --i)
        {
            output.at(count.at(static_cast<sort_type>(elems->read(i)) - min) - 1uL) = elems->read(i);
            count.at(static_cast<sort_type>(elems->read(i)) - min) -= 1uL;
        }

        for (auto i { 0LL }; i < N; ++i)
        {
            viewer->mark(i, sf::Color::Blue);
            elems->write(i, output.at(i));
            viewer->unmark(i);
        }
    }

}  /// namespace sv::algorithms

#endif  // SV_ALGORITHMS_COUNTING_SORT
