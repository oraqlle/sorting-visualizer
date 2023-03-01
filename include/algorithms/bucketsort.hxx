#ifndef SV_ALGORITHMS_BUCKET_SORT
#   define SV_ALGORITHMS_BUCKET_SORT

#include <SFML/Graphics.hpp>

#include <sv/elements.hxx>
#include <sv/viewer.hxx>

#include <algorithm>
#include <functional>
#include <list>
#include <ranges>
#include <vector>

namespace sv::algorithms
{
    namespace
    {
        auto bucketsort_min(
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
        
        auto bucketsort_max(
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

    auto bucketsort(
        std::shared_ptr<Elements> elems,
        std::shared_ptr<Viewer> viewer
    ) noexcept -> void
    {
        using element_type = Elements::element_type;

        auto N { elems->size() };
        auto bucket_size { 10uL };
        auto&& [cp, rd, wr, sw] = elems->counters();

        auto min { bucketsort_min(elems, viewer, 0uL, N) };
        auto max { bucketsort_max(elems, viewer, 0uL, N) };
        auto range { (max - min) / static_cast<element_type>(bucket_size) };

        auto buckets = std::vector(bucket_size, std::vector<element_type>{});

        for (auto i { 0uL }; i < elems->size(); ++i)
        {
            viewer->mark(i, sf::Color::Red);
            auto diff { ((elems->read(i) - min) / range) - static_cast<std::size_t>((elems->read(i) - min) / range )};

            if (diff == 0 && elems->read(i) != min)
            {
                buckets.at(static_cast<std::size_t>((elems->read(i) - min) / range) - 1uL).push_back(elems->read(i));
            }
            else
            {
                buckets.at(static_cast<std::size_t>((elems->read(i) - min) / range)).push_back(elems->read(i));
            }
            cp += 1uL;
            viewer->unmark(i);
        }

        for (auto& bckt : buckets)
            if (!bckt.empty())
                std::ranges::sort(bckt);

        auto k { 0uL };
        for (auto& bckt : buckets)
            if (!bckt.empty())
                for (auto& e : bckt)
                {
                    viewer->mark(k, sf::Color::Blue);
                    elems->write(k, e);
                    viewer->unmark(k);
                    k += 1uL;
                }
    }

}  /// namespace sv::algorithms

#endif  // SV_ALGORITHMS_BUCKET_SORT
