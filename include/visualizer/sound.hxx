#ifndef SV_SOUND
#   define SV_SOUND

#include <SFML/Audio.hpp>

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <ranges>
#include <source_location>
#include <string>
#include <unordered_map>
#include <utility>

namespace fs = std::filesystem;
using namespace std::literals;

namespace sv
{
    class Sound
    {
    public:

        using sound_buffer_type = sf::SoundBuffer;
        using sound_type        = sf::Sound;

        enum class SFX_Option : short
        { READ, WRITE, CHECK, SORTED, NOT_SORTED };

    public:

        Sound() noexcept
            : m_read{ sound_type{} }
            , m_write{ sound_type{} }
            , m_check{ sound_type{} }
            , m_sorted{ sound_type{} }
            , m_not_sorted{ sound_type{} }
            , m_read_buffer{ sound_buffer_type{} }
            , m_write_buffer{ sound_buffer_type{} }
            , m_check_buffer{ sound_buffer_type{} }
            , m_sorted_buffer{ sound_buffer_type{} }
            , m_not_sorted_buffer{ sound_buffer_type{} }
            , m_muted{ false }
        {
            auto srcloc         = std::source_location::current();
            auto src_path       = fs::path(srcloc.file_name());
            auto asset_path     = src_path.remove_filename() / "../../assets"s;

            auto all_loaded {
                m_read_buffer.loadFromFile(asset_path / "read.wav"s)
             && m_write_buffer.loadFromFile(asset_path / "write.wav"s)
             && m_check_buffer.loadFromFile(asset_path / "check.wav"s)
             && m_sorted_buffer.loadFromFile(asset_path / "hoorah.wav"s)
             && m_not_sorted_buffer.loadFromFile(asset_path / "no.wav"s)
            };

            all_loaded ? (std::clog << "") : (std::clog << "Error loading audio files!" << std::endl);

            m_read.setBuffer(m_read_buffer);
            m_write.setBuffer(m_write_buffer);
            m_check.setBuffer(m_check_buffer);
            m_sorted.setBuffer(m_sorted_buffer);
            m_not_sorted.setBuffer(m_not_sorted_buffer);
        }

        Sound(const Sound& sound) noexcept
            : m_read{ sound.m_read }
            , m_write{ sound.m_write }
            , m_check{ sound.m_check }
            , m_sorted{ sound.m_sorted }
            , m_not_sorted{ sound.m_not_sorted }
            , m_read_buffer{ sound.m_read_buffer }
            , m_write_buffer{ sound.m_write_buffer }
            , m_check_buffer{ sound.m_check_buffer }
            , m_sorted_buffer{ sound.m_sorted_buffer }
            , m_not_sorted_buffer{ sound.m_not_sorted_buffer }
            , m_muted{ sound.m_muted }
        { }

        Sound(Sound&& sound) noexcept
            : m_read{ std::move(sound.m_read) }
            , m_write{ std::move(sound.m_write) }
            , m_check{ std::move(sound.m_check) }
            , m_sorted{ std::move(sound.m_sorted) }
            , m_not_sorted{ std::move(sound.m_not_sorted) }
            , m_read_buffer{ std::move(sound.m_read_buffer) }
            , m_write_buffer{ std::move(sound.m_write_buffer) }
            , m_check_buffer{ std::move(sound.m_check_buffer) }
            , m_sorted_buffer{ std::move(sound.m_sorted_buffer) }
            , m_not_sorted_buffer{ std::move(sound.m_not_sorted_buffer) }
            , m_muted{ sound.m_muted }
        { }

        // ~Sound() noexcept
        // {
        //     std::ranges::for_each(m_sound_map | std::views::values, [](auto& x){ x.stop(); });
        //     m_sound_map.erase(m_sound_map.begin(), m_sound_map.end());
        // }

        auto operator= (const Sound& sound) noexcept
            -> Sound&
        {
            m_read = sound.m_read;
            m_write = sound.m_write;
            m_check = sound.m_check;
            m_sorted = sound.m_sorted;
            m_not_sorted = sound.m_not_sorted;


            m_read_buffer = sound.m_read_buffer;
            m_write_buffer = sound.m_write_buffer;
            m_check_buffer = sound.m_check_buffer;
            m_sorted_buffer = sound.m_sorted_buffer;
            m_not_sorted_buffer = sound.m_not_sorted_buffer;

            m_muted = sound.m_muted;

            return *this;
        }

        auto operator= (Sound&& sound) noexcept
            -> Sound&
        {
            m_read = std::move(sound.m_read);
            m_write = std::move(sound.m_write);
            m_check = std::move(sound.m_check);
            m_sorted = std::move(sound.m_sorted);
            m_not_sorted = std::move(sound.m_not_sorted);

            m_read_buffer = std::move(sound.m_read_buffer);
            m_write_buffer = std::move(sound.m_write_buffer);
            m_check_buffer = std::move(sound.m_check_buffer);
            m_sorted_buffer = std::move(sound.m_sorted_buffer);
            m_not_sorted_buffer = std::move(sound.m_not_sorted_buffer);

            m_muted = std::move(sound.m_muted);

            return *this;
        }

        auto play([[maybe_unused]] SFX_Option sfx_option, [[maybe_unused]] float pitch) noexcept -> void
        {
            if (m_muted)
                return;
            else
                switch (sfx_option)
                {
                case SFX_Option::READ:
                    m_read.play();
                    m_read.setPitch(pitch);
                    break;

                case SFX_Option::WRITE:
                    m_read.play();
                    m_read.setPitch(pitch);
                    break;

                case SFX_Option::CHECK:
                    m_read.play();
                    m_read.setPitch(pitch);
                    break;

                case SFX_Option::SORTED:
                    m_sorted.play();
                    break;

                case SFX_Option::NOT_SORTED:
                    m_not_sorted.play();
                    break;
                
                default:
                    std::clog << "Could not find requested sound" << std::endl;
                }                    
        }
        
        auto stop() noexcept -> void
        {
            m_read.stop();
            m_write.stop();
        }
        
        auto toggle_mute() noexcept -> void
        {
            stop();
            m_muted ^= true;
        }

    private:

        sound_type          m_read;
        sound_type          m_write;
        sound_type          m_check;
        sound_type          m_sorted;
        sound_type          m_not_sorted;

        sound_buffer_type   m_read_buffer;
        sound_buffer_type   m_write_buffer;
        sound_buffer_type   m_check_buffer;
        sound_buffer_type   m_sorted_buffer;
        sound_buffer_type   m_not_sorted_buffer;

        bool                m_muted;

    };  /// class Sound
}

#endif  // SV_SOUND
