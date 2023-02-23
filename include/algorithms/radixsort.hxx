#ifndef SV_ALGORITHMS_RADIX_SORT
#   define SV_ALGORITHMS_RADIX_SORT

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
        auto radix_max(
            std::shared_ptr<Elements> elems,
            std::shared_ptr<Viewer> viewer
        ) noexcept -> sv::Elements::element_type
        {
            auto maxidx { 0uL };

            for (auto i { 0uL }; i < elems->size(); ++i)
            {
                viewer->mark(i, sf::Color::Yellow);

                if (elems->compare(i, maxidx, std::ranges::greater{}))
                    maxidx = i;

                viewer->unmark(i);
            }

            return elems->read(maxidx);
        }

        auto radix_impl(
            std::shared_ptr<Elements> elems,
            std::shared_ptr<Viewer> viewer,
            int place
        ) noexcept -> void
        {
            auto s { elems->size() };

            using element_type = sv::Elements::element_type;
            auto output = std::vector<element_type>(s, element_type{});
            auto count  = std::vector<std::size_t>(10uL, std::size_t{});

            for (auto i { 0uL }; i < s; ++i)
            {
                viewer->mark(i, sf::Color::Red);
                count.at((static_cast<unsigned int>(elems->read(i)) / place) % 10) += 1uL;
                viewer->unmark(i);
            }

            for (auto i { 1uL }; i < 10; ++i)
                count.at(i) += count.at(i - 1uL);

            for (auto i { 9uL }; i > 0uL; --i)
                count.at(i) = count.at(i - 1uL);

            count.at(0uL) = 0uL;

            for (auto i { 0uL }; i < s; ++i)
            {
                viewer->mark(i, sf::Color::Red);
                auto val { static_cast<unsigned int>(elems->read(i)) };
                auto digit { (val / place) % 10 };
                output.at(count.at(digit)) = static_cast<element_type>(val);
                count.at(digit) += 1uL;
                viewer->unmark(i);
            }

            for (auto i { 0uL }; i < s; ++i)
            {
                viewer->mark(i, sf::Color::Blue);
                elems->write(i, output.at(i));
                viewer->unmark(i);
            }
        }
    }

    auto radixsort(
        std::shared_ptr<Elements> elems,
        std::shared_ptr<Viewer> viewer
    ) noexcept -> void
    {
        auto max { static_cast<unsigned int>(radix_max(elems, viewer)) };

        for (auto p { 1u }; (max / p) > 0u; p *= 10u)
            radix_impl(elems, viewer, p);
    }

}  /// namespace sv::algorithms

#endif  // SV_ALGORITHMS_RADIX_SORT
