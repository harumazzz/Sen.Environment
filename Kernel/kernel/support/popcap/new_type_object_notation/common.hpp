#pragma once

#include "kernel/utility/utility.hpp"

namespace Sen::Kernel::Support::PopCap::NewTypeObjectNotation {

    struct Common {

        inline static auto constexpr k_default_x_texture = 0_i32;

        inline static auto constexpr k_default_x_atlas = 0x7FFFFFFF_i32;

        inline static auto constexpr k_default_y_texture = 0_i32;

        inline static auto constexpr k_default_y_atlas = 0x7FFFFFFF_i32;

        inline static auto constexpr k_default_ax = 0_i32;

        inline static auto constexpr k_default_ay = 0_i32;

        inline static auto constexpr k_default_aw = 0_i32;

        inline static auto constexpr k_default_ah = 0_i32;

        inline static auto constexpr k_default_cols = 1_i32;

        inline static auto constexpr k_default_rows = 1_i32;

        inline static auto constexpr k_default_version = 1_u8;

        inline static auto constexpr k_empty = 0_ui;

        inline static auto constexpr k_null_block_count = 2_ui;

    };

}