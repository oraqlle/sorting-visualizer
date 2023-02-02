#ifndef SV_VIEWER
#   define SV_VIEWER

#include <SFML/Graphics.hpp>

#include <elements.hxx>

#include <memory>
#include <thread>
#include <vector>
#include <utility>

namespace sv
{
    class Viewer
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
            , m_colours{ std::move(vw.m_colours) }
            , m_rects{ std::move(vw.m_rects) }
            , m_rect_width{ vw.m_rect_width }
            , m_sorter{ std::move(vw.m_sorter) }
        {
            m_texture.create(m_window_width, m_window_height);

            vw.m_texture.clear();
            vw.m_rect_width = size_type{};
        }

        explicit Viewer(size_type width, size_type height/*, Elements elems*/) noexcept
            : m_window_width{ width }
            , m_window_height{ height }
            , m_texture{ texture_type() }
            , m_colours{ }
        { }

        ~Viewer() noexcept
        {
            m_sorter.detach();
            m_rect_width    = float{};
            // m_rects         = std::vector<rect_type>{};
            // m_colours       = std::vector<colour_type>{};
            m_texture.clear();
            m_window_height = size_type{};
            m_window_width  = size_type{};
        }

        void render() noexcept
        {

        }


    private:
        size_type                   m_window_width;
        size_type                   m_window_height;
        texture_type                m_texture;
        std::shared_ptr<Elements>   m_elems;
        std::vector<colour_type>    m_colours;
        std::vector<rect_type>      m_rects;
        float                       m_rect_width;

        std::thread m_sorter;
    };  /// class Viewer

}  /// namespace sv

#endif  // SV_VIEWER
