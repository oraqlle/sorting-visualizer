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
        [[maybe_unused]] std::shared_ptr<Viewer> viewer,
        Sorter* sorter
    ) -> void
    {
        auto rd     = std::random_device{};
        auto eng    =  std::mt19937{ rd() };

        std::ranges::shuffle(elems->items(), eng);
        sorter->sorted() = false;

        sorter->sorting() = false;
    }

}  /// namespace sv::algorithms

#endif  // SV_ALGORITHMS_SHUFFLE
