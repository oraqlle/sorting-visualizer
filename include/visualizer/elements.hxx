#ifndef SV_ELEMENTS
#   define SV_ELEMENTS

#include <SFML/Graphics.hpp>

#include <visualizer/sound.hxx>

#include <chrono>
#include <concepts>
#include <functional>
#include <iostream>
#include <memory>
#include <thread>
#include <tuple>
#include <vector>
#include <utility>

namespace sv
{
    class Elements
    {
    public:

        using element_type          = float;
        using size_type             = std::size_t;
        using duration_type         = std::chrono::milliseconds;
        using sound_type            = std::shared_ptr<sv::Sound>;

    public:

        Elements() noexcept = default;

        Elements(Elements&& elems) noexcept
            : m_max_value{ elems.m_max_value }
            , m_sort_amount{ elems.m_sort_amount }
            , m_cmp_counter{ elems.m_cmp_counter }
            , m_read_counter{ elems.m_read_counter }
            , m_write_counter{ elems.m_write_counter }
            , m_swap_counter{ elems.m_swap_counter }
            , m_read_delay{ elems.m_read_delay }
            , m_write_delay{ elems.m_write_delay }
            , m_items{ std::move(elems.m_items) }
            , m_sfx{ std::move(elems.m_sfx) }
        {
            elems.m_max_value       = element_type{};
            elems.m_sort_amount     = size_type{};
            elems.m_cmp_counter     = size_type{};
            elems.m_read_counter    = size_type{};
            elems.m_write_counter   = size_type{};
            elems.m_swap_counter    = size_type{};
            elems.m_read_delay      = duration_type{};
            elems.m_write_delay     = duration_type{};
            elems.m_sfx             = sound_type{};
        }

        explicit Elements(
            element_type max_value,
            size_type sort_amount,
            duration_type read_delay,
            duration_type write_delay,
            sound_type sound
        )
            : m_max_value{ max_value }
            , m_sort_amount{ sort_amount }
            , m_cmp_counter{ size_type{} }
            , m_read_counter{ size_type{} }
            , m_write_counter{ size_type{} }
            , m_swap_counter{ size_type{} }
            , m_read_delay{ read_delay }
            , m_write_delay{ write_delay }
            , m_items{ std::vector<element_type>(m_sort_amount, element_type{}) }
            , m_sfx{ sound }
        { 
            std::ranges::generate(
                m_items,
                [&, i=1]() mutable 
                { 
                    auto v = (static_cast<float>(i) * max_value) / static_cast<float>(sort_amount);
                    i += 1;
                    return v;
                }
            );
        }

        constexpr auto
        size() noexcept 
            -> size_type
        { return m_sort_amount; }

        constexpr auto
        max_value() noexcept 
            -> element_type
        { return m_max_value; }

        auto reset_counters() 
            noexcept -> void
        {
            m_cmp_counter   = size_type{};
            m_read_counter  = size_type{};
            m_write_counter = size_type{};
            m_swap_counter  = size_type{};
        }

        template<std::copy_constructible F = std::ranges::less>
            requires std::invocable<F&, size_type, size_type>
        auto compare(const size_type& x, const size_type& y, F cmp = {})
            noexcept -> bool
        {
            m_cmp_counter += size_type{ 1 };

            auto a { this->read(x) };
            auto b { this->read(y) };

            return cmp(a, b);
        }

        auto swap_elems(const size_type& x, const size_type& y)
            noexcept -> void
        {
            m_swap_counter += 1uL;

            auto a = this->read(x);
            this->write(x, this->read(y));
            this->write(y, a);
        }

        auto read(size_type idx) noexcept
            -> element_type
        {
            if (idx > m_sort_amount)
            {
                std::clog << "Out of bound value: `" << idx << "` selected!" << std::endl;
                return 0.0f;
            }

            m_read_counter += 1uL;
            // m_sfx->play(Sound::SFX_Option::READ, 0.5f + ((1 * m_items.at(idx)) / m_max_value));
            std::this_thread::sleep_for(m_read_delay);
            return m_items.at(idx);
        }

        auto write(size_type idx, element_type value) noexcept
            -> void
        {
            if (idx > m_sort_amount)
            {
                std::clog << "Out of bound value: `" 
                          << idx 
                          << "` selected! No write operation performed";
                return;
            }

            m_write_counter += 1uL;
            // m_sfx->play(Sound::SFX_Option::WRITE, 0.5f + ((0.45f * m_items.at(idx)) / m_max_value));
            std::this_thread::sleep_for(m_write_delay);
            m_items.at(idx) = value;
        }

        auto silent_read(size_type idx) 
            noexcept( noexcept(m_items.at(idx)) )
            -> element_type&
        { return m_items.at(idx); }

        auto items() noexcept
            -> std::vector<element_type>&
        { return m_items; }

        auto counters() noexcept
            -> decltype(auto)
        { 
            return std::forward_as_tuple(
                m_cmp_counter,
                m_read_counter,
                m_write_counter,
                m_swap_counter
            );
        }

        auto delays() noexcept
            -> decltype(auto)
        { 
            return std::forward_as_tuple(
                m_read_delay,
                m_write_delay
            );
        }

    private:
        float                       m_max_value;
        size_type                   m_sort_amount;

        size_type                   m_cmp_counter;
        size_type                   m_read_counter;
        size_type                   m_write_counter;
        size_type                   m_swap_counter;

        duration_type               m_read_delay;
        duration_type               m_write_delay;

        std::vector<element_type>   m_items;

        sound_type                  m_sfx;
        
    };  /// class Elements

}  /// namespace sv

#endif  // SV_ELEMENTS
