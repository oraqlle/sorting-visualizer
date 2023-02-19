#ifndef SV_VIEWER
#   define SV_VIEWER

#include <SFML/Graphics.hpp>

#include <visualizer/elements.hxx>

#include <algorithm>
#include <memory>
#include <ranges>
#include <thread>
#include <vector>
#include <utility>

namespace sv
{
    class Viewer : public sf::Sprite
    {
    public:

        using size_type     = std::size_t;
        using colour_type   = sf::Color;
        using rect_type     = sf::RectangleShape;
        using texture_type  = sf::RenderTexture;

    public:

        Viewer() noexcept = default;

        Viewer(Viewer&& vw) noexcept
            : m_window_width{ vw.m_window_width }
            , m_window_height{ vw.m_window_height }
            , m_texture{ texture_type{} }
            , m_elems { std::move(vw.m_elems) }
            , m_colours{ std::move(vw.m_colours) }
            , m_rects{ std::move(vw.m_rects) }
            , m_rect_width{ vw.m_rect_width }
        {
            m_texture.create(
                static_cast<unsigned int>(m_window_width),
                static_cast<unsigned int>(m_window_height)
            );

            vw.m_texture.clear();
            vw.m_rect_width = size_type{};
        }

        explicit Viewer(
            size_type width,
            size_type height,
            std::shared_ptr<Elements> elems
        ) noexcept
            : m_window_width{ width }
            , m_window_height{ height }
            , m_texture{ texture_type{} }
            , m_elems{ elems }
            , m_colours{ std::vector<colour_type>(elems->size(), colour_type::White) }
            , m_rects{ std::vector<rect_type>(elems->size(), rect_type{}) }
            , m_rect_width{ static_cast<float>(width) / static_cast<float>(elems->size()) }
        {
            _M_generate();

            m_texture.create(
                static_cast<unsigned int>(m_window_width),
                static_cast<unsigned int>(m_window_height)
            );
            this->setTexture(m_texture.getTexture());
        }

        ~Viewer() noexcept
        {
            m_rect_width    = float{};
            m_rects.clear();
            m_colours.clear();
            m_texture.clear();
            m_window_height = size_type{};
            m_window_width  = size_type{};
        }

        auto resize(size_type new_size)
            noexcept -> void
        {
            m_rect_width = static_cast<float>(m_window_width) / static_cast<float>(new_size);
            m_colours.resize(new_size);
            m_rects.resize(new_size);
            _M_generate();
            unmark_range(0ul, new_size);
        }

        auto render() 
            noexcept -> void
        {
            m_texture.clear();

            for (auto i { 0uL }; i < m_rects.size(); ++i)
            {
                auto h { m_elems->silent_read(i) };

                m_rects.at(i).setSize(sf::Vector2f(m_rect_width - (m_rect_width * 0.1f), h));
                m_rects.at(i).setPosition(
                    static_cast<float>(i) * m_rect_width,
                    static_cast<float>(m_window_height) - h
                );
                m_rects.at(i).setFillColor(m_colours.at(i));
                m_texture.draw(m_rects.at(i));
            }

            m_texture.display();
        }

        auto mark(size_type idx, colour_type colour)
            noexcept -> void
        {
            if (idx > m_rects.size() - 1uL)
                return;
            
            m_colours.at(idx) = colour;
        }

        auto unmark(size_type idx)
            noexcept -> void
        {
            if (idx > m_rects.size() - 1uL)
                return;
            
            m_colours.at(idx) = colour_type::White;
        }

        auto mark_range(size_type fidx, size_type eidx, colour_type colour)
            noexcept -> void
        { std::fill(m_colours.begin() + fidx, m_colours.begin() + eidx, colour); }

        auto unmark_range(size_type fidx, size_type eidx)
            noexcept -> void
        { std::fill(m_colours.begin() + fidx, m_colours.begin() + eidx, colour_type::White); }

    private:

        auto _M_generate()
            noexcept -> void
        {
            std::ranges::fill(m_rects, rect_type(sf::Vector2f(m_rect_width - (m_rect_width * 0.1f), 0.0f)));
            std::ranges::for_each(
                m_rects,
                [this](auto& r)
                {   
                    r.setOutlineThickness(m_rect_width * 0.1f);
                    r.setOutlineColor(sf::Color::Black);
                }
            );
        }

    private:
        size_type                   m_window_width;
        size_type                   m_window_height;
        texture_type                m_texture;
        std::shared_ptr<Elements>   m_elems;
        std::vector<colour_type>    m_colours;
        std::vector<rect_type>      m_rects;
        float                       m_rect_width;
    };  /// class Viewer

}  /// namespace sv

#endif  // SV_VIEWER
