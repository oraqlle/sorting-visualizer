#ifndef SV_SORTER
#   define SV_SORTER

#include <visualizer/elements.hxx>
#include <visualizer/viewer.hxx>

#include <algorithm>
#include <exception>
#include <functional>
#include <memory>
#include <optional>
#include <ranges>
#include <string>
#include <string_view>
#include <thread>
#include <unordered_map>
#include <utility>

using namespace std::literals;

namespace sv
{
    class Sorter
    {
    public:

        using function_type = typename std::function<void(
            std::shared_ptr<Elements>,
            std::shared_ptr<Viewer>,
            Sorter* sorter
        )>;

        using map_type = typename std::unordered_map<
            std::string, 
            std::pair<
                std::string,
                function_type
            >
        >;

        // friend class std::function<void(Sorter*)>;

    public:

        Sorter() noexcept = default;

        Sorter(Sorter&& sorter) noexcept
            : m_elems{ std::move(sorter.m_elems) }
            , m_viewer{ std::move(sorter.m_viewer) }
            , m_algorithms{ std::move(sorter.m_algorithms) }
            , m_current_alg{ std::move(sorter.m_current_alg) }
            , m_sorting{ sorter.m_sorting }
            , m_sorted{ sorter.m_sorted }
            , m_sorter{ std::move(sorter.m_sorter) }
        { }

        explicit Sorter(
            std::shared_ptr<Elements>& elems,
            std::shared_ptr<Viewer>& viewer,
            map_type& alg_map
        ) noexcept
            : m_elems{ elems }
            , m_viewer{ viewer }
            , m_algorithms{ alg_map }
            , m_current_alg{ ""s }
            , m_sorting{ false }
            , m_sorted{ true }
            , m_sorter{ std::thread{} }
        { }

        ~Sorter() noexcept
        {
            if (m_sorter.joinable())
                m_sorter.detach();

            m_sorted    = false;
            m_sorting   = false;
        }

        auto start() noexcept -> void
        {
            if (m_sorting)
                return;

            if (
                auto alg { m_algorithms.find(m_current_alg) };
                alg != m_algorithms.cend() 
             || m_current_alg == "Check"s || m_current_alg == "Shuffle"s
            ) {
                m_sorting = true;

                if (m_sorter.joinable())
                    m_sorter.join();

                m_sorter = std::thread{ std::mem_fn(&Sorter::sort), this };
            }
            else
                std::clog << "No algorithm selected to start." << std::endl;            
        }

        auto stop() noexcept -> void 
        {
            m_sorting = false;

            if (m_sorter.joinable())
                m_sorter.detach();
        }

        auto select_algorithm(const std::string& name) -> void
        {
            if (m_algorithms.find(name) != m_algorithms.cend() || name == "Check"s || name == "Shuffle"s)
            {
                m_current_alg = name;

                if (!m_sorting)
                {
                    if (m_current_alg != "Check"s && m_current_alg != "Shuffle"s)
                        m_elems->reset_counters();

                    m_viewer->unmark_range(0uL, m_elems->size());
                }
            }
            else
            {
                auto alg_names = m_algorithms | std::views::keys;
                auto fmt_alg_names = std::accumulate(
                        alg_names.begin(),
                        alg_names.end(),
                        ""s,
                        [](const auto& x, const auto& y){ return x + ", "s + y; }
                    ).substr(2);

                throw std::out_of_range{
                    "Could not find selected algorithm `"s
                  + name + "`. "s
                  + "Available algorithms are: \n{ "
                  + fmt_alg_names
                  + " }\n"s
                };
            }
        }

        auto sort() noexcept
            -> void
        {
            m_elems->reset_counters();
            m_viewer->unmark_range(0uL, m_elems->size());

            if (m_current_alg == "Check"s)
                check();
            else if (m_current_alg == "Shuffle"s)
                shuffle();
            else
            {
                m_algorithms[m_current_alg].second(m_elems, m_viewer, this);
                check();
                /// Sound
            }

            m_sorting = false;
        }

        auto algorithm_name() 
            -> std::string
        { return m_current_alg; }

        auto algorithm_description() 
            -> std::string
        { 
            if (m_current_alg == "Check"s)
                return "Checks if the array is sorted or not. Colors green for sorted blocks and red for unsorted block."s;
            else if (m_current_alg == "Shuffle"s)
                return "Shuffles the blocks into a random arrangement."s;
            else
                return m_algorithms[m_current_alg].first;
        }

        constexpr auto 
        sorted() 
            noexcept -> bool&
        { return m_sorted; }

        constexpr auto 
        sorting() 
            noexcept -> bool&
        { return m_sorting; }

        auto check() -> void
        {
            auto n { m_elems->size() - 1uL };
            auto c { 0uL };

            m_viewer->mark(0uL, sf::Color::Green);

            for (; c < n; ++c)
            {
                auto val = m_elems->items().at(c);
                if (val > m_elems->items().at(c + 1uL))
                {
                    m_viewer->mark(c + 1, sf::Color::Red);
                    break;
                }

                m_viewer->mark(c + 1, sf::Color::Green);
                std::this_thread::sleep_for(2000.0ms / n);
            }

            m_sorted = (n == c) ? true : false;

            if (m_sorted)
            {
                m_viewer->mark(n, sf::Color::Green);
                std::this_thread::sleep_for(500ms);
            }
        }

        auto shuffle() -> void
        {
            auto rd     = std::random_device{};
            auto eng    =  std::mt19937{ rd() };

            std::ranges::shuffle(m_elems->items(), eng);
            m_sorted = false;
        }

    private:
        std::shared_ptr<Elements>   m_elems;
        std::shared_ptr<Viewer>     m_viewer;
        map_type                    m_algorithms;
        std::string                 m_current_alg;
        bool                        m_sorting;
        bool                        m_sorted;
        std::thread                 m_sorter;
    };  /// Sorter

}  /// namespace sv

#endif  // SV_SORTER
