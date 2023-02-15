#ifndef SV_STATUS_BAR
#   define SV_STATUS_BAR

#include <SFML/Graphics.hpp>

#include <visualizer/elements.hxx>
#include <visualizer/sorter.hxx>

#include <filesystem>
#include <iostream>
#include <memory>
#include <source_location>
#include <sstream>
#include <string>

namespace fs = std::filesystem;

using namespace std::literals;

namespace sv
{
    class Statusbar : public sf::Sprite
    {
    public:
        using size_type         = float;
        using texture_type      = sf::RenderTexture;
        using seperator_type    = sf::RectangleShape;
        using font_type         = sf::Font;
        using text_type         = sf::Text;

    public:

        Statusbar() noexcept = default;

        Statusbar(Statusbar&& other) noexcept
            : m_width{ other.m_width }
            , m_height{ other.m_height }
            , m_texture{ texture_type{} }
            , m_font{ std::move(other.m_font) }
            , m_text{ std::move(other.m_text) }
            , m_elems{ std::move(other.m_elems) }
            , m_sorter{ std::move(other.m_sorter) }
        {
            m_texture.create(static_cast<unsigned int>(m_width), static_cast<unsigned int>(m_height));
            this->setTexture(m_texture.getTexture());
            other.m_texture.clear();
        }

        Statusbar(
            size_type width,
            size_type height,
            // size_type xpos,
            // size_type ypos,
            std::shared_ptr<Elements> elems,
            std::shared_ptr<Sorter> sorter
        ) noexcept
            : m_width{ width }
            , m_height{ height }
            , m_texture{ texture_type{} }
            , m_font{ font_type{} }
            , m_text{ text_type{} }
            , m_elems{ elems }
            , m_sorter{ sorter }
        {
            m_texture.create(static_cast<unsigned int>(m_width), static_cast<unsigned int>(m_height));
            this->setTexture(m_texture.getTexture());
            
            auto srcloc         = std::source_location::current();
            auto src_path       = fs::path(srcloc.file_name());
            auto asset_path     = src_path.remove_filename() / "../../assets"s;

            if (!m_font.loadFromFile(asset_path / "JetBrainsMonoNF.ttf"s))
                std::clog << "Error loading font!" << std::endl;

            m_text.setFont(m_font);
            m_text.setCharacterSize(12);
            // m_text.setPosition(xpos, ypos);
        }

        auto render() noexcept -> void
        {
            auto ss = std::stringstream{};
            const auto& [cmps, reads, writes, swaps]    = m_elems->counters();
            const auto& [rdelay, wdelay]                = m_elems->delays();

            ss << "Current Algorithm: "  << m_sorter->algorithm_name() << "\n"
               << "Description: "        << m_sorter->algorithm_description() << "\n\n"
               << "Stats:\n"
               << "Comparisons: "        << cmps << "\n"
               << "Read count: "         << reads << "\n"
               << "Write count: "        << writes << "\n"
               << "Swap count: "         << swaps << "\n"
               << "Read delay: "         << rdelay.count() << " ms\n"
               << "Write delay: "        << wdelay.count() << " ms\n"
               << "Data size: "          << m_elems->size() << "\n"
               << "Sorting?: "           << (m_sorter->sorting() ? "Yes" : "No")
               << "Sorted?: "            << (m_sorter->sorted() ? "Yes" : "No");

            m_text.setString(ss.str());

            m_texture.clear();
            m_texture.draw(m_text);
            m_texture.display();
        }

    private:

        size_type                   m_width;
        size_type                   m_height;
        texture_type                m_texture;
        font_type                   m_font;
        text_type                   m_text;
        std::shared_ptr<Elements>   m_elems;
        std::shared_ptr<Sorter>     m_sorter;

    };  /// class Statusbar
}

#endif  // SV_STATUS_BAR
