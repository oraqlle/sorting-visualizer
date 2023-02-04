#ifndef SV_SORTER
#   define SV_SORTER

#include <elements.hxx>
#include <viewer.hxx>

#include <algorithm>
#include <exception>
#include <functional>
#include <unordered_map>
#include <memory>
#include <optional>
#include <ranges>
#include <string>
#include <string_view>
#include <thread>
#include <utility>

using namespace std::literals;

namespace sv
{
    class Sorter
    {
    public:

        using map_type = typename std::unordered_map<
            std::string, 
            std::pair<
                std::string,
                std::function<void(
                    std::shared_ptr<Elements>,
                    std::shared_ptr<Viewer>,
                    Sorter* sorter
                )>
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
            , m_sorted{ false }
            , m_sorter{ std::thread{} }
        { }

        ~Sorter() noexcept
        {
            m_sorter.detach();
            m_sorted    = false;
            m_sorting   = false;
        }

        auto start() noexcept -> void
        {
            if (m_sorting)
                return;

            m_sorting = true;

            if (m_sorter.joinable())
            {
                m_sorter.join();
                m_sorting = false;
            }

            m_elems->reset_counters();
            m_viewer->unmark_range(0uL, m_elems->size());
            m_sorter = std::thread{ 
                m_algorithms[m_current_alg].second,
                m_elems,
                m_viewer,
                this
            };
        }

        auto stop() noexcept -> void 
        {
            m_sorting = false;
            m_sorter.detach();
        }

        auto select_algorithm(const std::string& name) -> void
        {
            if (m_algorithms.find(name) != m_algorithms.cend())
            {
                m_current_alg = name;

                if (!m_sorting)
                {
                    m_elems->reset_counters();
                    m_viewer->unmark_range(0uL, m_elems->size());
                }
            }
            else
            {
                auto alg_names = m_algorithms | std::views::keys;
                throw std::out_of_range{
                    "Could not find selected algorithm `"s
                  + name + "`. "s
                  + "Available algorithms are: \n{"
                  + std::accumulate(
                        alg_names.begin(),
                        alg_names.end(),
                        ""s,
                        [](const auto& x, const auto& y){ return x + ", "s + y; }
                    )
                  + "}\n"s
                };
            }
        }

        auto algorithm_name() -> std::optional<std::string>
        {
            auto alg { m_algorithms.find(m_current_alg) };

            return (alg != m_algorithms.cend()) ? std::optional{ m_current_alg }
                                                : std::nullopt;
        }

        auto algorithm_description() -> std::optional<std::string>
        {
            auto alg { m_algorithms.find(m_current_alg) };

            return (alg != m_algorithms.cend()) ? std::optional{ alg->first }
                                                : std::nullopt;
        }

        constexpr auto 
        set_sorted() 
            noexcept -> bool&
        { return m_sorted }

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
