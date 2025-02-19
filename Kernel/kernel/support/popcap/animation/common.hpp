#pragma once

#include "kernel/utility/utility.hpp"

namespace Sen::Kernel::Support::PopCap::Animation
{
    
    struct Common
    {
        static constexpr auto k_magic_identifier = 0xBAF01954_ui;

        static constexpr auto version_list = {1_ui, 2_ui, 3_ui, 4_ui, 5_ui, 6_ui};

        static constexpr auto vertical_bar = "|"_sv;

        inline static auto k_version = 0_ui;

        inline static auto k_frame_rate = static_cast<double>(0);

        struct LayerRemoveFlag {
            static constexpr auto k_count = 5_byte;

            // ----------------

            static constexpr auto unused_1 = 0_byte;

            static constexpr auto unused_2 = 1_byte;

            static constexpr auto unused_3 = 2_byte;

            static constexpr auto unused_4 = 3_byte;

            static constexpr auto unused_5 = 4_byte;
        };

        struct LayerAppendFlag {
            static constexpr auto k_count = 5_byte;

            // ----------------

            static constexpr auto time_scale = 0_byte;

            static constexpr auto name = 1_byte;

            static constexpr auto preload_frame = 2_byte;

            static constexpr auto additive = 3_byte;

            static constexpr auto sprite = 4_byte;
        };

        struct LayerChangeFlag {
            static constexpr auto k_count = 6_byte;

            // ----------------

            static constexpr auto sprite_frame_number = 0_byte;

            static constexpr auto long_coord = 1_byte;

            static constexpr auto matrix = 2_byte;

            static constexpr auto color = 3_byte;

            static constexpr auto rotate = 4_byte;

            static constexpr auto source_rectangle = 5_byte;
        };

        struct FrameFlag {
            static constexpr auto k_count = 8_byte;

            // ----------------

            static constexpr auto remove = 0_byte;

            static constexpr auto append = 1_byte;

            static constexpr auto change = 2_byte;

            static constexpr auto label = 3_byte;

            static constexpr auto stop = 4_byte;

            static constexpr auto command = 5_byte;

            static constexpr auto unused_7 = 6_byte;

            static constexpr auto unused_8 = 7_byte;
        };

        struct ValueRate {

            static constexpr auto time = 65536.0f;

            static constexpr auto size = 20.0f;

            static constexpr auto angle = 1000.0f;

            static constexpr auto matrix = 65536.0f;

            static constexpr auto matrix_exact = 20.0f * 65536.0f;

            static constexpr auto color = 255.0f;
        };

        static constexpr auto contain_version(u32 const & version) -> bool
        {
            return std::ranges::find(version_list, version) != version_list.end();
        }

        static auto check_version(u32 const & version, std::initializer_list<u32> const & list) -> bool {
            auto result = true;
            result &= std::ranges::find(version_list, version) != list.end();
            result &= (list.size() < 1_size || *(list.begin() + 0) <= version) && (list.size() < 2_size || *(list.begin() + 1) > version);
            return result;
        }
    };
}