#pragma once

#include "kernel/utility/encoding/apng/common.hpp"

namespace Sen::Kernel::Encoding::APNG {

	template <typename T> requires std::is_arithmetic<T>::value
	struct Setting {

        CList<T> delay_frames_list{};

        T loop{};

        T width{};

        T height{};

        bool trim{};

        explicit Setting(

        ) = default;

        ~Setting(

        ) = default;

		auto operator=(
			const Setting&
		) -> Setting& = delete;

		Setting(
			Setting&&
		) = delete;

		auto operator=(
			Setting&&
		) -> Setting& = delete;

        explicit Setting(
            CList<T> &&delay_frames_list,
            const T& loop,
            const T& width,
            const T& height,
            bool trim
		) : delay_frames_list{std::move(delay_frames_list)}, loop{loop}, width{width}, height{height}, trim{trim}
        {
        }
    };

}