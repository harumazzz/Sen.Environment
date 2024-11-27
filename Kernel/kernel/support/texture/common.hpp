#pragma once

namespace Sen::Kernel::Support::Texture {

	template <typename T> requires std::is_arithmetic<T>::value
	inline auto constexpr calculate_area(
		T width, 
		T height
	) -> T
	{
		return width * height;
	}

	template <typename T> requires std::is_arithmetic<T>::value
	inline auto constexpr set_pixel(
		T x, 
		T y, 
		T width
	) -> T
	{
		return (y * width + x) * 4;
	}

	template <typename T> requires std::is_arithmetic<T>::value
	inline auto constexpr pixel_area(
		T area
	) -> T
	{
		return area * 4;
	}

	template <typename T> requires std::is_arithmetic<T>::value
	inline auto constexpr pixel_area_rgba(
		T width, 
		T height
	) -> T
	{
		return pixel_area(calculate_area(width, height));
	}

}
