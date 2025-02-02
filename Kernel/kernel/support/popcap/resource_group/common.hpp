#pragma once

#include "kernel/utility/utility.hpp"

namespace Sen::Kernel::Support::PopCap::ResourceGroup {

	struct Common {

		constexpr explicit Common(

		) noexcept = default;

		constexpr ~Common(

		) = default;

		inline static auto constexpr DefaultCoordinateOffset = 0;

		inline static auto constexpr DefaultLayoutOffset = 1;

		inline static auto const WindowStyle = std::string{"\\"};

		inline static auto const PosixStyle = std::string{"/"};

		inline static auto const Array = std::string{"array"};

		inline static auto const String = std::string{"string"};

		inline static auto const emptyString = std::string{""};

		inline static auto const emptyType = std::string{"0"};
	};

	enum class PathStyle : uint8_t
	{

		ArrayStyle,

		WindowStyle,

	};

}