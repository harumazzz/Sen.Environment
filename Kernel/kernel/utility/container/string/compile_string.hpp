#pragma once

#include <string_view>
#include "kernel/utility/trait/trait.hpp"

namespace Sen::Kernel {

    template <auto N> requires std::is_same_v<type_of<N>, std::size_t>
	struct CompileTimeString;

	constexpr auto to_string(const auto& input) { return CompileTimeString(input); }


	template <auto N> requires std::is_same_v<type_of<N>, std::size_t>
	struct CompileTimeString
	{
		char elems[N]{};

		CompileTimeString() { elems[N - 1] = '\0'; }

		constexpr explicit CompileTimeString(const char(&s)[N])
		{
			std::copy_n(s, N, this->elems);
		}

		constexpr auto size(

		) const -> std::size_t
		{
			return N - 1;
		}

		constexpr auto sz(

		) const -> std::size_t
		{
			return N;
		}

		constexpr auto data() const -> const char*  {
			return elems;
		}

		constexpr CompileTimeString(const CompileTimeString<N>(&s))
		{
			std::copy_n(s.elems, N, this->elems);
		}

		constexpr CompileTimeString(const std::array<char, N>(&s))
		{
			std::copy_n(s.data(), N, this->elems);
		}

		template <auto... Ni> 
		constexpr void _copy(const CompileTimeString<Ni>(&... input))
		{
			auto pos = elems;
			((pos = std::copy_n(input.elems, Ni - 1, pos)), ...);
			*pos = 0;
		}

		constexpr CompileTimeString(const auto&... input) requires (sizeof...(input) > 1)
		{
			std::invoke([this](const auto&... s) constexpr { this->_copy(s...); }, to_string(input)...);
		}

		template <template <typename...> typename container, typename... T>
		constexpr CompileTimeString(const container<T...>& input)
		{
			std::apply([this](const auto&... s) constexpr { this->_copy(to_string(s)...); }, input);
		}

		constexpr operator const char* () const { return elems; }
	};

	template<auto N> requires std::is_same_v<type_of<N>, std::size_t> CompileTimeString(const char(&)[N])
		-> CompileTimeString<N>;

	template<auto N> requires std::is_same_v<type_of<N>, std::size_t> CompileTimeString(const std::array<char, N>& input)
		-> CompileTimeString<N>;

	// source: https://codereview.stackexchange.com/questions/272265/c20-compile-time-string-utility

	template<CompileTimeString ms>
	inline constexpr auto operator"" _ms() noexcept
	{
		return ms;
	}

	template <auto N> requires std::is_same_v<type_of<N>, std::size_t>
	inline auto constexpr make_string_view(
		const CompileTimeString<N>& str
	) -> std::string_view
	{
		return std::string_view{ str.data(), str.size() };
	}

}
