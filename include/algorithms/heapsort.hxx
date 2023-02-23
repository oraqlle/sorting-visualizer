#ifndef SV_ALGORITHMS_HEAP_SORT
#   define SV_ALGORITHMS_HEAP_SORT

#include <SFML/Graphics.hpp>

#include <sv/elements.hxx>
#include <sv/sorter.hxx>
#include <sv/viewer.hxx>

#include <cmath>
#include <functional>
#include <random>
#include <ranges>

namespace sv::algorithms
{
    namespace
    {
        auto make_heap(
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
                max = left;

            if (right < N && elems->compare(right, max, std::ranges::greater{}))
                max = right;

            if (max != i)
            {
                elems->swap_elems(i, max);
                make_heap(elems, viewer, N, max);
            }
        }
    }

    auto heapsort(
        std::shared_ptr<Elements> elems,
        std::shared_ptr<Viewer> viewer
    ) noexcept -> void
    {
        auto rd   = std::random_device{};
        auto reng = std::mt19937{ rd() };

        auto N { static_cast<long>(elems->size()) };
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
        
        for (auto i { (N / 2L) - 1L }; i >= 0L; --i)
            make_heap(elems, viewer, N, i);

        for (auto i { N - 1L}; i >= 0L; --i)
        {
            viewer->mark(0uL, sf::Color::Magenta);
            viewer->mark(i, sf::Color::Magenta);
            elems->swap_elems(0uL, i);
            make_heap(elems, viewer, i, 0uL);
            viewer->unmark(0uL);
            viewer->unmark(i);
        }
    }

}  /// namespace sv::algorithms

#endif  // SV_ALGORITHMS_HEAP_SORT
