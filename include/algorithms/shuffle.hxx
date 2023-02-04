#ifndef SV_ALGORITHMS_SHUFFLE
#   define SV_ALGORITHMS_SHUFFLE

#include <elements.hxx>
#include <sorter.hxx>
#include <viewer.hxx>

#include <algorithm>
#include <memory>
#include <random>

namespace sv::algorithms
{
    auto shuffle(
        std::shared_ptr<Elements> elems,
        std::shared_ptr<Viewer> viewer,
        Sorter* sorter
    ) -> void
    {
        auto rd     = std::random_device{};
        auto eng    = std::default_random_engine{ rd() };
        auto dist   = std::uniform_real_distribution<float>{ 0, elems->size() };

        std::ranges::shuffle(elems->items(), dist(eng));
        sorter->set_sorted() = false;
    }

}  /// namespace sv::algorithms

#endif  // SV_ALGORITHMS_SHUFFLE
