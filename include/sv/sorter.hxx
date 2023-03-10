#ifndef SV_SORTER
#   define SV_SORTER

#include <sv/elements.hxx>
#include <sv/sound.hxx>
#include <sv/viewer.hxx>

#include <algorithm>
#include <chrono>
#include <exception>
#include <functional>
#include <memory>
#include <optional>
#include <random>
#include <ranges>
#include <sstream>
#include <string>
#include <string_view>
#include <thread>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std::literals;

namespace sv
{
    class Sorter 
        : public std::enable_shared_from_this<Sorter>
    {
    public:

        using size_type             = std::size_t;
        using duration_type         = std::chrono::microseconds;
        using clock_type            = std::chrono::high_resolution_clock;
        using time_point_type       = std::chrono::time_point<clock_type>;
        using function_type         = std::function<void(
            std::shared_ptr<Elements> elems,
            std::shared_ptr<Viewer> viewer
        )>;
        using map_type              = std::unordered_map<
            std::string,                                    /// Algorithm name
            std::tuple<
                std::string,                                /// Keybinding
                std::vector<std::string>,                   /// Details
                function_type                               /// Algorithm Implementation
            >
        >;

    private:

        Sorter() noexcept = default;

        explicit Sorter(
            std::shared_ptr<Elements>& elems,
            std::shared_ptr<Viewer>& viewer,
            std::shared_ptr<Sound>& sfx,
            map_type& alg_map
        ) noexcept
            : m_elems{ elems }
            , m_viewer{ viewer }
            , m_sfx{ sfx }
            , m_algorithms{ alg_map }
            , m_current_algorithm_name{ "N/A"s }
            , m_sorting{ false }
            , m_sorted{ true }
            , m_start_time_point{ time_point_type{} }
            , m_end_time_point{ time_point_type{} }
            , m_sorter{ std::thread{} }
        { }

    public:

        [[nodiscard]] static std::shared_ptr<Sorter>
        create(
            std::shared_ptr<Elements>& elems,
            std::shared_ptr<Viewer>& viewer,
            std::shared_ptr<Sound>& sfx,
            map_type& alg_map
        ) noexcept
        {
            return std::shared_ptr<Sorter>(
                new Sorter(elems, viewer, sfx, alg_map)
            );
        }

        auto getptr() noexcept 
            -> std::shared_ptr<Sorter>
        { return shared_from_this(); }

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

            if (m_algorithms.find(m_current_algorithm_name) != m_algorithms.cend() || m_current_algorithm_name == "Check"s || m_current_algorithm_name == "Shuffle"s)
            {
                m_sorting = true;

                m_start_time_point  = time_point_type{};
                m_end_time_point    = time_point_type{};
                auto&& [r, w, a]    = m_elems->delays();
                a                   = duration_type{};

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
                m_sorter.join();

            // m_sfx->toggle_mute();
        }

        auto select_algorithm(const std::string& name) -> void
        {
            if (m_algorithms.find(name) != m_algorithms.cend() || name == "Check"s || name == "Shuffle"s)
            {
                m_current_algorithm_name = name;

                if (!m_sorting)
                {
                    if (m_current_algorithm_name != "Check"s && m_current_algorithm_name != "Shuffle"s)
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
            m_viewer->unmark_range(0uL, m_elems->size());

            if (m_current_algorithm_name == "Check"s)
                check();
            else if (m_current_algorithm_name == "Shuffle"s)
                shuffle();
            else
            {
                m_elems->reset_counters();
                m_start_time_point = clock_type::now();
                std::get<2>(m_algorithms[m_current_algorithm_name])(m_elems, m_viewer);
                m_sorting = false;
                check();
                // m_sfx->stop();
            }

            m_sorting = false;
        }

        auto algorithm_name() 
            -> std::string
        { return m_current_algorithm_name; }

        auto algorithm_details() 
            -> std::vector<std::string>
        { 
            if (m_current_algorithm_name == "Check"s)
                return std::vector{ 
                    " Description: Checks if the array is\n sorted or not."s,
                    " Sorted: Green"s,
                    " Unsorted: Red"s
                };
            else if (m_current_algorithm_name == "Shuffle"s)
                return std::vector{ " Description: Shuffles the blocks into\n a random arrangement."s };
            else if (auto alg { m_algorithms.find(m_current_algorithm_name) }; alg != m_algorithms.cend())
                return std::get<1>(m_algorithms[m_current_algorithm_name]);
            else
                return std::vector{ ""s };
        }

        auto elapsed_time() noexcept
            -> std::chrono::milliseconds
        { 
            if (m_sorting)
                m_end_time_point = clock_type::now();
            return std::chrono::duration_cast<std::chrono::milliseconds>(m_end_time_point - m_start_time_point); 
        }

        auto algorithm_keybinds(std::stringstream& ss) 
            -> void
        {
            auto max  = std::ranges::max(
                m_algorithms | std::views::keys,
                {},
                std::ranges::size
            ).size();

            std::ranges::copy(
                m_algorithms | std::views::transform([&max](const auto& kvs)
                {
                    auto& [k, vs] = kvs;

                    auto fill = std::string(max - k.size(), ' ');
                    return "  "s + k + fill + " : " + std::get<0>(vs);
                }),
                std::ostream_iterator<std::string>(ss, "\n")
            );
        }

        auto adjust_delay(duration_type rdelay, duration_type wdelay)
            noexcept -> void
        {
            auto&& [rdd, wrd, _] = m_elems->delays();

            rdd += rdelay;
            wrd += wdelay;

            rdd = std::ranges::clamp(rdd, std::chrono::microseconds::zero(), std::chrono::microseconds::max());
            wrd = std::ranges::clamp(wrd, std::chrono::microseconds::zero(), std::chrono::microseconds::max());
        }

        auto resize(size_type new_size)
            noexcept -> void
        {
            if (!m_sorting)
            {
                m_elems->resize(new_size);
                m_viewer->resize(new_size);
            }
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

                // m_sfx->play(Sound::SFX_Option::CHECK, 0.5f + (0.5f + (val / m_elems->max_value())));
                m_viewer->mark(c + 1, sf::Color::Green);
                std::this_thread::sleep_for(2000.0ms / n);
            }

            m_sorted = (n == c) ? true : false;

            if (m_sorted)
            {
                m_viewer->mark(n, sf::Color::Green);
                std::this_thread::sleep_for(500ms);
                // m_sfx->play(Sound::SFX_Option::SORTED, 1);
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
        std::shared_ptr<Sound>      m_sfx;
        map_type                    m_algorithms;
        std::string                 m_current_algorithm_name;
        bool                        m_sorting;
        bool                        m_sorted;
        time_point_type             m_start_time_point;
        time_point_type             m_end_time_point;
        std::thread                 m_sorter;
    };  /// Sorter

}  /// namespace sv

#endif  // SV_SORTER
