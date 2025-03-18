#pragma once

#include "kernel/utility/container/base/base.hpp"
#include "kernel/utility/container/string/basic_string.hpp"

namespace Sen::Kernel {

	struct StringView : public BaseContainer<char> {

    protected:

        using Base = BaseContainer<char>;

		inline static auto constexpr none = static_cast<size_t>(-1);

    public:

        constexpr StringView(
        ) noexcept : BaseContainer<char>{nullptr, 0_size} {}

        constexpr StringView(
            Pointer<char> data,
            const std::size_t& size
        ) noexcept : Base{data, size} {}

		constexpr StringView(
			const char* data,
			const std::size_t& size
		) noexcept : Base{const_cast<char*>(data), size} {}

		constexpr StringView(
			const char* data
		) noexcept : StringView{data, std::strlen(data)} {}

		constexpr explicit StringView(
			const std::string& other
		) noexcept : StringView{other.data(), other.size()} {}

		constexpr StringView(
			const std::string_view& data
		) noexcept : StringView{data.data(), data.size()} {}

		constexpr StringView(
			std::string_view&& data
		) noexcept : StringView{data.data(), data.size()} {}

        explicit constexpr StringView(const BasicString& other) noexcept : Base{other.begin(), other.size()} {

        }

        constexpr auto subview(
            const std::size_t& offset,
            const std::size_t& count = std::dynamic_extent
        ) const -> StringView {
            assert_conditional(offset <= thiz.size(), "Accessed index is larger than the size of the array", "subview");
            const auto new_size = count == std::dynamic_extent || offset + count > thiz.size() ? thiz.size() - offset : count;
            return StringView{thiz.begin() + offset, new_size};
        }

        constexpr auto operator ==(const StringView& other) const noexcept -> bool {
            return this->size() == other.size() && std::memcmp(this->begin(), other.begin(), this->size()) == 0;
        }

        constexpr auto operator!=(const StringView& other) const noexcept -> bool {
            return !(thiz == other);
        }

        constexpr auto operator<(const StringView& other) const noexcept -> bool {
            return std::ranges::lexicographical_compare(thiz, other);
        }

        constexpr auto operator>(const StringView& other) const noexcept -> bool {
            return other < thiz;
        }

        constexpr auto operator<=(const StringView& other) const noexcept -> bool {
            return other >= thiz;
        }

        constexpr auto operator>=(const StringView& other) const noexcept -> bool {
            return !(thiz < other);
        }

        constexpr auto remove_prefix(const usize& n) noexcept -> void {
            assert_conditional(n <= this->size(), "Accessed index is larger than the size of the array", "remove_prefix");
            thiz = StringView{thiz.data() + n, thiz.size() - n};
        }

        constexpr auto remove_suffix(const usize& n) noexcept -> void {
            assert_conditional(n <= thiz.size(), "Accessed index is larger than the size of the array", "remove_suffix");
            thiz = StringView{thiz.data(), thiz.size() - n};
        }

        constexpr auto trim(
        ) const noexcept -> StringView {
            auto start = 0_size;
            auto end = thiz.size();
            while (start < end && std::isspace(thiz.begin()[start])) {
                ++start;
            }
            while (end > start && std::isspace(thiz.begin()[end - 1])) {
                --end;
            }
            return StringView{thiz.begin() + start, end - start};
        }

        inline auto to_string(
        ) const -> BasicString {
            return BasicString{thiz.begin(), thiz.size()};
        }

        [[nodiscard]] constexpr auto find (
    		const char pattern,
			const Size& pos = 0
    	) const -> Size {
    		return Detail::find(thiz, pattern, pos);
    	}

    	constexpr auto find(
			const char* pattern,
			const size_t& size,
			const Size& pos = 0
		) const -> Size {
    		return Detail::find(thiz, pattern, size, pos);
    	}

    	[[nodiscard]] constexpr auto find(
			const String& pattern,
			const Size& pos = 0
		) const -> Size {
    		return find(pattern.begin(), pattern.size(), pos);
    	}

    	[[nodiscard]] constexpr auto find(
			const StringView& pattern,
			const Size& pos = 0
		) const -> Size {
        	return find(pattern.begin(), pattern.size(), pos);
        }

    	[[nodiscard]] constexpr auto find(
			const std::string_view& pattern,
			const Size& pos = 0
		) const -> Size {
    		return find(pattern.data(), pattern.size(), pos);
    	}

    	constexpr auto find(
			const char* pattern,
			const Size& pos = 0
		) const -> Size {
    		return find(pattern, std::strlen(pattern), pos);
    	}

    	constexpr auto rfind(
			const char* pattern,
			const size_t& size,
			const Size& pos = none
		) const -> Size {
			return Detail::rfind(thiz, pattern, size, pos);
    	}

    	constexpr auto rfind(
			const char* pattern,
			const Size& pos = none
		) const -> Size {
			return rfind(pattern, std::strlen(pattern), pos);
    	}

		[[nodiscard]] constexpr auto rfind(
			const String& pattern,
			const Size& pos = none
		) const -> Size {
			return rfind(pattern.begin(), pattern.size(), pos);
		}

    	[[nodiscard]] constexpr auto rfind(
			const StringView& pattern,
			const Size& pos = none
		) const -> Size {
        	return rfind(pattern.begin(), pattern.size(), pos);
        }

		[[nodiscard]] constexpr auto rfind(
			const std::string_view& pattern,
			const Size& pos = none
		) const -> Size {
			return rfind(pattern.data(), pattern.size(), pos);
		}

    	constexpr auto starts_with (
			const char* pattern,
			const Size& m
		) const -> bool {
    		return Detail::starts_with(thiz, pattern, m);
    	}

    	[[nodiscard]] constexpr auto starts_with (
			const std::string_view& value
		) const -> bool {
    		return thiz.starts_with(value.data(), value.size());
    	}

    	[[nodiscard]] constexpr auto starts_with (
			const StringView& value
		) const -> bool {
        	return thiz.starts_with(value.begin(), value.size());
        }

    	constexpr auto starts_with (
			const char* value
		) const -> bool {
    		return thiz.starts_with(value, std::strlen(value));
    	}

    	[[nodiscard]] constexpr auto starts_with (
			const String& value
		) const -> bool {
    		return thiz.starts_with(value.begin(), value.size());
    	}

    	constexpr auto ends_with (
			const char* pattern,
			const Size& m
		) const -> bool {
    		return Detail::ends_with(thiz, pattern, m);
    	}

    	[[nodiscard]] constexpr auto ends_with (
			const std::string_view& value
		) const -> bool {
    		return thiz.ends_with(value.data(), value.size());
    	}

    	[[nodiscard]] constexpr auto ends_with (
			const String& value
		) const -> bool {
    		return thiz.ends_with(value.begin(), value.size());
    	}

    	[[nodiscard]] constexpr auto ends_with (
			const StringView& value
		) const -> bool {
        	return thiz.ends_with(value.begin(), value.size());
        }

    	[[nodiscard]] constexpr auto contains (
    		const String& pattern
    	) const -> bool {
    		return thiz.find(pattern) != none;
    	}

    	[[nodiscard]] constexpr auto contains (
			const StringView& pattern
		) const -> bool {
        	return thiz.find(pattern) != none;
        }

    	[[nodiscard]] constexpr auto contains (
			const char* pattern
		) const -> bool {
        	return thiz.find(pattern) != none;
        }

    	[[nodiscard]] constexpr auto contains (
			const std::string_view& pattern
		) const -> bool {
        	return thiz.find(pattern) != none;
        }

    	[[nodiscard]] constexpr auto rcontains (
			const String& pattern
		) const -> bool {
    		return thiz.rfind(pattern) != none;
    	}

    	[[nodiscard]] constexpr auto rcontains (
			const StringView& pattern
		) const -> bool {
        	return thiz.rfind(pattern) != none;
        }

    	[[nodiscard]] constexpr auto rcontains (
			const char* pattern
		) const -> bool {
        	return thiz.rfind(pattern) != none;
        }

    	[[nodiscard]] constexpr auto rcontains (
			const std::string_view& pattern
		) const -> bool {
        	return thiz.rfind(pattern) != none;
        }

    	friend auto operator <<(std::ostream &os, const StringView& other) -> std::ostream& {
        	if (other.begin() != nullptr) {
        		os.write(other.begin(), static_cast<std::streamsize>(other.size()));
        	}
        	return os;
        }

    	[[nodiscard]] constexpr auto string(

		) const -> std::string
        {
        	return std::string{ thiz.value, thiz._size };
        }

		constexpr auto operator [](
			Size const& index
		) const -> char {
			assert_index(index < thiz._size, fmt::format("Accessed index is larger than the size of the list"), fmt::format("operator[]({})", index));
			return thiz.value[index];
        }

        virtual auto view (
		) const -> std::string_view {
			return std::string_view{ thiz.value, thiz._size };
        }

		constexpr auto operator ==(
			const BasicString &other
		) const -> bool {
        	if (thiz._size != other.size()) {
        		return false;
        	}
        	return std::memcmp(thiz.value, other.begin(), thiz._size) == 0;
        }

    	[[nodiscard]] auto wstring(

		) const -> std::wstring
        {
			#if _WIN32
        	const auto size = static_cast<size_t>(MultiByteToWideChar(CP_UTF8, 0, thiz.value, static_cast<int>(thiz._size), nullptr, 0));
        	auto destination = std::wstring{};
        	destination.resize(size);
        	MultiByteToWideChar(CP_UTF8, 0, thiz.value, static_cast<int>(thiz._size), &destination[0], static_cast<int>(size));
        	return destination;
			#else
        	auto convert = std::wstring_convert<std::codecvt_utf8<wchar_t>>{};
        	return convert.from_bytes(thiz.value, thiz.value + thiz._size);
			#endif
        }

    	[[nodiscard]] auto warray (
		) const -> CArray<wchar_t> {
			#if _WIN32
        	auto size = static_cast<size_t>(MultiByteToWideChar(CP_UTF8, 0, thiz.value, static_cast<int>(thiz._size), nullptr, 0));
        	auto destination = CArray<wchar_t>{size};
        	MultiByteToWideChar(CP_UTF8, 0, thiz.value, static_cast<int>(thiz._size), &destination[0], static_cast<int>(size));
        	return destination;
			#else
        	auto convert = std::wstring_convert<std::codecvt_utf8<wchar_t>>{};
        	auto wstr = convert.from_bytes(thiz.value, thiz.value + thiz._size);
        	auto destination = CArray<wchar_t>{wstr.size()};
        	std::memcpy(destination.begin(), wstr.data(), wstr.size() * sizeof(wchar_t));
        	return destination;
		#endif
        }

    	constexpr operator std::string_view(
		) const {
        	return std::string_view{ thiz.value, thiz._size };
        }

    	[[nodiscard]] constexpr auto length(

		) const -> Size
        {
        	return thiz._size;
        }

    };

	constexpr String::operator StringView() const {
		return StringView{ thiz.value, thiz._size };
	}

	constexpr auto String::as_view() const -> StringView {
		return StringView{ thiz.value, thiz._size };
	}

	inline auto String::operator==(
		const StringView &other
	) const -> bool {
		if (thiz._size != other.size()) {
			return false;
		}
		return std::memcmp(thiz.value, other.begin(), thiz._size) == 0;
	}


	inline auto constexpr make_string_view (
		const std::string& value
	) -> StringView {
		return StringView{value};
	}



}