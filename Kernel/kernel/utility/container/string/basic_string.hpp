#pragma once

#include "kernel/utility/container/string/common.hpp"
#include "kernel/utility/macro.hpp"
#include "kernel/utility/algorithm/pointer.hpp"
#include "kernel/utility/container/array/byte_array.hpp"
#include "kernel/utility/container/list/list.hpp"

namespace Sen::Kernel {

	class BasicString;

	using String = BasicString;

    class BasicString : public BaseContainer<char> {

	public:

		using Size = std::size_t;

		template <typename U>
		using Pointer = U*;

		using Character = char;

		using Iterator = Pointer<Character>;

    	using allocator_type = std::allocator<char>;

    	using Alloc = Allocator<char>;

    	template <typename T>
    	using List = CList<T>;

		inline static auto constexpr none = static_cast<size_t>(-1);

    	usize _capacity{};

	public:

		constexpr explicit BasicString(

		) = default;

		constexpr explicit BasicString(
			const Size& size
		) : BaseContainer<char>{Alloc::allocate(size + 1), size}, _capacity{size + 1}
		{
			thiz.value[size] = '\0';
		}

		explicit BasicString(
			const char* data
		) : BaseContainer<char>{nullptr, std::strlen(data)}, _capacity{}
		{
			thiz._capacity = thiz._size + 1;
			thiz.value = Alloc::allocate(thiz._capacity);
			thiz.value[thiz._size] = '\0';
			std::memcpy(thiz.value, data, thiz._size);
		}

    	explicit BasicString(
			char* data,
			const Size& size
		) : BaseContainer<char>{ Alloc::allocate(size + 1), size }, _capacity{size + 1}
		{
			thiz.value[thiz._size] = '\0';
			std::memcpy(thiz.value, data, thiz._size);
		}

		explicit BasicString(
			const char* data,
			const Size& size
		) : BaseContainer<char>{ Alloc::allocate(size + 1), size }, _capacity{size + 1}
		{
			thiz.value[thiz._size] = '\0';
			std::memcpy(thiz.value, data, thiz._size);
		}

    	explicit BasicString(
		   const std::string_view data
	   ) : BaseContainer<char>{Alloc::allocate(data.size() + 1), data.size()}, _capacity{data.size() + 1} {
			thiz.value[thiz._size] = '\0';
			std::memcpy(thiz.value, data.data(), thiz._size);
		}


		auto allocate (
			Size const& size
		) -> void
		{
			thiz.allocate_full(size);
			thiz._size = size;
		}

    	auto allocate_full (
			Size const& size
		) -> void
		{
			Alloc::deallocate(thiz.value);
			thiz._capacity = size + 1;
			thiz.value = Alloc::allocate(thiz._capacity);
			std::memset(thiz.value, 0, thiz._capacity);
			thiz._size = 0;
		}

    	auto reallocate (
			Size const& size
		) -> void
		{
			thiz._capacity = size + 1;
			const auto new_value = Alloc::allocate(thiz._capacity);
			std::memcpy(new_value, thiz.value, thiz._size);
			Alloc::deallocate(thiz.value);
			thiz.value = new_value;
			std::memset(thiz.value + thiz._size, 0, thiz._capacity - thiz._size);
			thiz._size = size;
		}

		constexpr ~BasicString(

		) override
		{
			if (thiz.value != nullptr) {
			Alloc::deallocate(thiz.value);
				thiz.value = nullptr;
			}
		}

		auto operator==(
			const StringView & other
		) const -> bool;

		BasicString(
			const BasicString& other
		) {
			thiz.value = Alloc::allocate(other._capacity);
			thiz._size = other._size;
			thiz._capacity = other._capacity;
			std::memcpy(thiz.value, other.value, other._size);
			thiz.value[thiz._size] = '\0';
		}

		auto operator =(
			const BasicString& other
		) -> BasicString & {
			Alloc::deallocate(thiz.value);
			thiz._capacity = other._capacity;
			thiz.value = Alloc::allocate(thiz._capacity);
			thiz._size = other._size;
			std::memcpy(thiz.value, other.value, other._size);
			thiz.value[thiz._size] = '\0';
			return thiz;
		}

    	constexpr BasicString(
			BasicString&& other
		) noexcept : BaseContainer{}, _capacity{} {
			thiz = as_move(other);
		}

    	auto operator+(const BasicString& other) const -> BasicString {
			auto string = BasicString{};
			string += thiz;
			string += other;
			return string;
		}

    	auto operator+(const std::string_view other) const -> BasicString {
			auto string = BasicString{};
			string += thiz;
			string += other;
			return string;
		}

		constexpr auto operator=(
			BasicString&& other
		) noexcept -> BasicString& {
			if (this != &other) {
				Alloc::deallocate(thiz.value);
				thiz._size = other._size;
				thiz.value = other.value;
				thiz._capacity = other._capacity;
				other.value = nullptr;
				other._size = 0;
				other._capacity = 0;
			}
			return thiz;
		}

    	constexpr auto operator [](
			Size const& index
		) const -> Character {
    		assert_index(index < thiz._size, fmt::format("Accessed index is larger than the size of the list"), fmt::format("operator[]({})", index));
    		return thiz.value[index];
    	}

    	constexpr auto operator [](
			Size const& index
		) -> Character& override {
    		assert_index(index < thiz._size, fmt::format("Accessed index is larger than the size of the list"), fmt::format("operator[]({})", index));
    		return thiz.value[index];
    	}

		constexpr auto operator ==(
			const BasicString &other
		) const -> bool {
			if (thiz._size != other._size) {
				return false;
			}
			return std::memcmp(thiz.value, other.value, thiz._size) == 0;
		}

    	constexpr auto operator ==(
			 std::string_view other
		) const -> bool {
			if (thiz._size != other.size()) {
				return false;
			}
			return std::memcmp(thiz.value, other.data(), thiz._size) == 0;
		}

		constexpr auto operator != (
			const BasicString& other
		) const -> bool
		{
			return !(thiz.operator==(other));
		}

    	constexpr auto operator != (
    		std::string_view other
		) const -> bool
		{
			return !(thiz.operator==(other));
		}

		auto operator < (
			const BasicString& other
		) -> bool = delete;

		auto operator > (
			const BasicString& other
		) = delete;

		auto operator <= (
			const BasicString& other
		) -> bool = delete;

		auto operator >= (
			const BasicString& other
		) -> bool = delete;

		[[nodiscard]] constexpr auto size(

		) const -> Size override {
			return thiz._size;
		}

		friend auto operator <<(std::ostream &os, const BasicString& other) -> std::ostream& {
			if (other.value != nullptr) {
				os.write(other.value, static_cast<std::streamsize>(other._size));
			}
			return os;
		}

		[[nodiscard]] constexpr auto string(

		) const -> std::string
		{
			return std::string{ thiz.value, thiz._size };
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

    	constexpr operator StringView(
		) const;

		[[nodiscard]] constexpr auto length(

		) const -> Size
		{
			return thiz._size;
		}

    	constexpr auto take_ownership (
			BasicString& other
		) -> void {
			Alloc::deallocate(thiz.value);
			thiz.value = other.value;
			thiz._size = other._size;
			thiz._capacity = other._capacity;
			other.value = nullptr;
			other._size = 0;
			other._capacity = 0;
		}

		constexpr virtual auto insert(
			const Size& index,
			const Pointer<const char> str,
			const Size& len
		) -> void {
			assert_conditional(index < thiz._size, fmt::format("Inserted index is larger than the size of the string"), "insert");
			const auto new_size = thiz._size + len;
			if (new_size <= thiz._capacity) {
				std::memmove(thiz.value + index + len, thiz.value + index, thiz._size - index);
				std::memcpy(thiz.value + index, str, len);
			} else {
				const auto new_capacity = std::max(new_size, thiz._capacity + 4096_size);
				const auto raw = Alloc::allocate(new_capacity + 1);
				std::memcpy(raw, thiz.value, index);
				std::memcpy(raw + index, str, len);
				std::memcpy(raw + index + len, thiz.value + index, thiz._size - index);
				raw[new_size] = '\0';
				Alloc::deallocate(thiz.value);
				thiz.value = raw;
				thiz._capacity = new_capacity;
			}
			thiz._size = new_size;
			thiz.value[new_size] = '\0';
		}

		constexpr virtual auto insert(
			const Size& index,
			const Character& ch
		) -> void {
			const auto temporary = std::array<char, 1>{ ch };
			return insert(index, temporary.data(), temporary.size());
		}

		constexpr virtual auto insert(
			const Size& index,
			const BasicString& other
		) -> void {
			return insert(index, other.value, other._size);
		}

		constexpr virtual auto erase(
			const Size& index,
			const Size& len
		) -> BasicString& {
			assert_conditional(index < thiz._size, fmt::format("Erased index is larger than the size of the string"), "erase");
			return erase(thiz.value + index, thiz.value + std::min(index + len, thiz._size));
		}

		virtual auto erase(
			const Iterator& first,
			const Iterator& last
		) -> BasicString& {
			assert_conditional(first >= thiz.value && first <= thiz.end(), fmt::format("Erase begin iterator is outside bounds of the String iterator"), "erase");
			assert_conditional(last >= thiz.value && last <= thiz.end(), fmt::format("Erase end iterator is outside bounds of the String iterator"), "erase");
			assert_conditional(first <= last, fmt::format("Cannot erase if the first iterator is larger than last iterator"), "erase");
			const auto erase_len = static_cast<Size>(last - first);
			const auto new_size = thiz._size - erase_len;
			std::memcpy(first, last, thiz.end() - last);
			thiz._size = new_size;
			thiz.value[thiz._size] = '\0';
			return thiz;
		}

    	constexpr virtual auto push(
    		const Character& c
    	) -> void {
			const auto new_size = thiz._size + 1;
			if (new_size <= thiz._capacity) {
				thiz.value[thiz._size] = c;
			} else {
				const auto new_capacity = std::max(new_size, thiz._capacity + 4096_size);
				const auto new_value = Alloc::allocate(new_capacity + 1);
				std::memcpy(new_value, thiz.value, thiz._size);
				new_value[thiz._size] = c;
				Alloc::deallocate(thiz.value);
				thiz.value = new_value;
				thiz._capacity = new_capacity;
			}
			thiz._size = new_size;
			thiz.value[new_size] = '\0';
		}


		virtual constexpr auto pop(
		) -> void {
			if (thiz._size > 0) {
				--thiz._size;
				thiz.value[thiz._size] = '\0';
			}
		}

    	constexpr virtual auto append(
    		const Size& count,
    		const Character ch
    	) -> BasicString& {
			const auto new_size = thiz._size + count;
			if (new_size <= thiz._capacity) {
				std::memset(thiz.value + thiz._size, ch, count);
			} else {
				const auto new_capacity = std::max(new_size, thiz._capacity + 4096_size);
				const auto new_value = Alloc::allocate(new_capacity + 1);
				std::memcpy(new_value, thiz.value, thiz._size);
				std::memset(new_value + thiz._size, ch, count);
				Alloc::deallocate(thiz.value);
				thiz.value = new_value;
				thiz._capacity = new_capacity;
			}
			thiz._size = new_size;
			thiz.value[new_size] = '\0';
			return thiz;
		}


    	constexpr virtual auto append(
    		Pointer<const Character> s,
    		const Size& count
    	) -> BasicString& {
			const auto new_size = thiz._size + count;
			if (new_size <= thiz._capacity) {
				std::memcpy(thiz.value + thiz._size, s, count);
			} else {
				const auto new_capacity = std::max(new_size, thiz._capacity + 4096_size);
				const auto new_value = Alloc::allocate(new_capacity + 1);
				std::memcpy(new_value, thiz.value, thiz._size);
				std::memcpy(new_value + thiz._size, s, count);
				Alloc::deallocate(thiz.value);
				thiz.value = new_value;
				thiz._capacity = new_capacity;
			}
			thiz._size = new_size;
			thiz.value[new_size] = '\0';
			return thiz;
		}


		constexpr virtual auto append(
			const Pointer<const Character>& s
		) -> BasicString& {
			return append(s, std::strlen(s));
		}

		constexpr auto append(
			const BasicString& str
		) -> BasicString& {
			return append(str.value, str._size);
		}

		constexpr auto append(
			const BasicString& str,
			const Size& pos,
			const Size& count = none
		) -> BasicString& {
			assert_conditional(pos <= str._size, fmt::format("Cannot append if the position is larger than the size of the string. The position: {}, String size: {}", pos, str._size), "append");
			return append(str.value + pos, std::min(count, str._size - pos));
		}

		template <typename Iterate>
		constexpr auto append(
			const Iterate& first,
			const Iterate& last
		) -> BasicString& {
			auto count = last - first;
			return append(&(*first), count);
		}

		constexpr virtual auto append(
			const std::initializer_list<Character>& list
		) -> BasicString& {
			return append(list.begin(), list.size());
		}

		constexpr auto replace(
			const Size& pos,
			const Size& count,
			const BasicString& str
		) -> BasicString& {
			return replace(pos, count, str.value, str._size);
		}

		constexpr auto replace(
			Pointer<Character> first,
			Pointer<Character> last,
			const BasicString& str
		) -> BasicString& {
			return replace(first - value, last - first, str.value, str._size);
		}

		constexpr auto replace(
			const Size& pos,
			const Size& count,
			const BasicString& str,
			const Size& pos2,
			const Size& count2 = none
		) -> BasicString& {
			assert_conditional(pos2 <= str._size, fmt::format("Cannot replace if the position is larger than the size of the string. The position: {}, String size: {}", pos2, str._size), "replace");
			return replace(pos, count, str.value + pos2, std::min(count2, str._size - pos2));
		}

    	constexpr virtual auto replace(
			const Size& pos,
			const Size& count,
			Pointer<const Character> cstr,
			const Size& count2
		) -> BasicString& {
			const auto new_size = thiz._size - count + count2;
			if (new_size <= thiz._capacity) {
				if (count2 != count) {
					std::memmove(thiz.value + pos + count2, thiz.value + pos + count, thiz._size - pos - count);
				}
				std::memcpy(thiz.value + pos, cstr, count2);
			} else {
				const auto new_capacity = std::max(new_size, thiz._capacity + 4096_size);
				const auto new_value = Alloc::allocate(new_capacity + 1);
				std::memcpy(new_value, thiz.value, pos);
				std::memcpy(new_value + pos, cstr, count2);
				std::memcpy(new_value + pos + count2, thiz.value + pos + count, thiz._size - pos - count);
				Alloc::deallocate(thiz.value);
				thiz.value = new_value;
				thiz._capacity = new_capacity;
			}
			thiz._size = new_size;
			thiz.value[new_size] = '\0';
			return thiz;
		}


		constexpr virtual auto replace(
			const Pointer<Character>& first,
			const Pointer<Character>& last,
			const Pointer<const Character>& cstr,
			const Size& count2
		) -> BasicString& {
			return replace(first - value, last - first, cstr, count2);
		}

		constexpr virtual auto replace(
			const Size& pos,
			const Size& count,
			const Pointer<const Character>& cstr
		) -> BasicString& {
			return replace(pos, count, cstr, std::strlen(cstr));
		}

		constexpr auto replace(
			const Pointer<Character>& first,
			const Pointer<Character>& last,
			const Pointer<const Character>& cstr
		) -> BasicString& {
			return replace(first - thiz.value, last - first, cstr, std::strlen(cstr));
		}

		virtual auto replace(
			const Size& pos,
			const Size& count,
			const Size& count2,
			const Character ch
		) -> BasicString& {
			const auto new_str = BasicString{ count2 };
			std::memset(new_str.value, ch, count2);
			return replace(pos, count, new_str);
		}

		virtual auto replace(
			const Pointer<Character>& first,
			const Pointer<Character>& last,
			const Size& count2,
			const Character ch
		) -> BasicString& {
			return replace(first - value, last - first, count2, ch);
		}

		template <typename It>
		auto replace(
			const Pointer<Character>& first,
			const Pointer<Character>& last,
			It first2,
			It last2
		) -> BasicString& {
			return replace(first - value, last - first, &(*first2), last2 - first2);
		}

		virtual auto replace(
			const Pointer<Character>& first,
			const Pointer<Character>& last,
			const std::initializer_list<Character>& list
		) -> BasicString& {
			return replace(first - value, last - first, list.begin(), list.size());
		}

		auto copy(
			const Pointer<Character>& source,
			const Size& size
		) const -> void {
			std::memcpy(source, thiz.value, size);
		}

		constexpr auto operator+=(
			const BasicString& other
		) -> BasicString& {
			return thiz.add(other.value, other._size);
		}

    protected:

		constexpr auto add(
			const char* other,
			const Size& other_size
		) -> BasicString& {
		    const auto new_size = thiz._size + other_size;
		    if (new_size <= thiz._capacity) {
		        std::memcpy(thiz.value + thiz._size, other, other_size);
		    } else {
		        const auto new_capacity = std::max(new_size, thiz._capacity + 4096_size);
		        const auto new_value = Alloc::allocate(new_capacity + 1);
		        std::memcpy(new_value, thiz.value, thiz._size);
		        std::memcpy(new_value + thiz._size, other, other_size);
		    	Alloc::deallocate(thiz.value);
		        thiz.value = new_value;
		        thiz._capacity = new_capacity;
		    }
		    thiz._size = new_size;
		    thiz.value[new_size] = '\0';
		    return thiz;
		}

    public:

    	constexpr virtual auto operator+=(
			const char* other
		) -> BasicString& {
    		return thiz.add(other, std::strlen(other));
    	}

		constexpr virtual auto operator+=(
			const Character& other
		) -> BasicString& {
			const auto temporary = std::array<char, 1>{other};
		    return thiz.add(temporary.data(), temporary.size());
		}

		constexpr virtual auto operator+=(
			const std::initializer_list<Character> &value_list
		) -> BasicString& {
		    return thiz.add(value_list.begin(), value_list.size());
		}

		template <typename StringViewLike>
		requires requires(StringViewLike str) {
		    { str.data() } -> std::same_as<Pointer<const char>>;
		    { str.size() } -> std::same_as<Size>;
		}
		constexpr auto operator+=(
			const StringViewLike& str
		) -> BasicString& {
		    return thiz.add(str.data(), str.size());
		}

    	[[nodiscard]] constexpr auto view (
    	) const -> std::string_view {
			return std::string_view{ thiz.value, thiz._size };
		}

    	constexpr auto as_view (
		) const -> StringView;

    	[[nodiscard]] auto sub (
    		const Size& from,
			const Size& to
		) const -> String {
			assert_index(to <= thiz._size, "To index must be smaller than string size", "sub");
			assert_index(from <= to, fmt::format("From index must be smaller than to index, from: {}, to: {}", from, to), "sub");
			auto result = String{to - from};
			std::memcpy(result.value, thiz.value + from, to - from);
			return result;
		}

    	[[nodiscard]] auto sub (
			const Size& from
		) const -> String {
			return sub(from, thiz._size);
		}

		virtual auto substring (
			const Size& from,
			const Size& to
		) -> void {
			assert_conditional(to <= thiz._size, "To index must be smaller than string size", "substring");
			assert_conditional(from <= to, fmt::format("From index must be smaller than to index, from: {}, to: {}", from, to), "substring");
			const auto result = Alloc::allocate(to - from + 1);
			std::memcpy(result, thiz.value + from, to - from);
    		Alloc::deallocate(thiz.value);
    		result[to - from] = '\0';
			thiz.value = result;
			thiz._size = to - from;
		}

		virtual auto substring (
			const Size& from
		) -> void {
			return thiz.substring(from, thiz._size);
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
    		return find(pattern.value, pattern._size, pos);
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
			return rfind(pattern.value, pattern._size, pos);
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

    	[[nodiscard]] constexpr auto contains (
    		const String& pattern
    	) const -> bool {
    		return thiz.find(pattern) != none;
    	}

    	[[nodiscard]] constexpr auto rcontains (
			const String& pattern
		) const -> bool {
    		return thiz.rfind(pattern) != none;
    	}

		constexpr virtual auto trim_left(
    	) -> void {
    		auto index = 0_size;
    		while (index < thiz._size && is_whitespace_character(value[index])) {
    			++index;
    		}
    		if (index > 0) {
    			const auto new_size = thiz._size - index;
    			const auto new_value = Alloc::allocate(new_size + 1);
    			std::memcpy(new_value, value + index, new_size + 1);
    			Alloc::deallocate(thiz.value);
    			thiz.value = new_value;
    			thiz._size = new_size;
    		}
    	}

		constexpr virtual auto trim_right(

    	) -> void
    	{
    		auto index = thiz._size;
    		while (index > 0 && is_whitespace_character(value[index - 1])) {
    			--index;
    		}
    		if (index < thiz._size) {
    			value[index] = '\0';
    			thiz._size = index;
    		}
    	}

		constexpr virtual auto trim (

    	) -> void {
			thiz.trim_right();
			thiz.trim_left();
    	}

    	constexpr auto find_all (
    		const char* target,
    		const size_t& length
    	) const -> List<Size>  {
    		return Detail::find_all(thiz, target, length);
		}

    	[[nodiscard]] constexpr auto find_all (
			const char target
		) const -> List<Size>  {
    		return Detail::find_all(thiz, target);
    	}

		[[nodiscard]] constexpr auto find_all (
			const std::string_view& target
		) const -> List<Size> {
			return thiz.find_all(target.data(), target.size());
		}

		[[nodiscard]] constexpr auto find_all (
			const String& target
		) const -> List<Size> {
			return thiz.find_all(target.begin(), target.size());
		}

		constexpr auto find_all (
			const char* target
		) const -> List<Size> {
			return thiz.find_all(target, std::strlen(target));
		}

		constexpr virtual auto replace_all(
    		const char* target,
			const size_t& target_length,
    		const char* replacement,
			const size_t& replacement_length
		) -> void
    	{
    		if (target_length == 0) {
    			return;
    		}
    		const auto positions = thiz.find_all(target);
    		if (positions.empty()) {
    			return;
    		}
    		const auto count = positions.size();
		    if (const auto new_length = thiz._size + count * (replacement_length - target_length); new_length == thiz._size) {
    			for (const auto& pos : positions) {
    				std::memcpy(thiz.value + pos, replacement, replacement_length);
    			}
    		}
    		else {
    			if (new_length > thiz._capacity) {
    				const auto new_capacity = std::max(new_length, thiz._capacity * 3);
    				const auto new_data = new Character[new_capacity + 1];
    				auto source_index = 0_size;
    				auto destination_index = 0_size;
    				auto position_index = 0_size;
    				while (source_index < thiz._size) {
    					if (position_index < count && source_index == positions[position_index]) {
    						std::memcpy(new_data + destination_index, replacement, replacement_length);
    						source_index += target_length;
    						destination_index += replacement_length;
    						position_index++;
    					} else {
    						new_data[destination_index++] = thiz.value[source_index++];
    					}
    				}
    				new_data[new_length] = '\0';
    				Alloc::deallocate(thiz.value);
    				thiz.value = new_data;
    				thiz._capacity = new_capacity;
    			} else {
    				auto source_index = thiz._size;
    				auto destination_index = new_length;
    				auto position_index = count;
    				while (source_index > 0) {
    					--source_index;
    					--destination_index;
    					if (position_index > 0 && source_index == positions[position_index - 1]) {
    						std::memcpy(thiz.value + destination_index - replacement_length + 1, replacement, replacement_length);
    						destination_index -= replacement_length - 1;
    						source_index -= target_length - 1;
    						--position_index;
    					} else {
    						thiz.value[destination_index] = thiz.value[source_index];
    					}
    				}
    				thiz.value[new_length] = '\0';
    			}
    			thiz._size = new_length;
    		}
    	}

		constexpr virtual auto replace_all(
			const std::string_view& target,
			const std::string_view& replacement
		) -> void {
			return thiz.replace_all(target.data(), target.size(), replacement.data(), replacement.size());
    	}

		constexpr auto replace_all(
			const String& target,
			const String& replacement
		) -> void {
			return thiz.replace_all(target.begin(), target.size(), replacement.begin(), replacement.size());
		}

		constexpr virtual auto replace_all(
			const char* target,
			const char* replacement
		) -> void {
			return thiz.replace_all(target, std::strlen(target), replacement, std::strlen(replacement));
		}

		constexpr virtual auto replace_all(
    		const char target,
    		const char replacement
		) const -> void
    	{
    		const auto positions = thiz.find_all(target);
    		if (positions.empty()) {
    			return;
    		}
    		for (const auto& pos : positions) {
    			std::memcpy(thiz.value + pos, &replacement, 1);
    		}
    	}

    	constexpr auto assign (
    		String& other
    	) -> void {
			Alloc::deallocate(thiz.value);
			thiz.value = other.value;
			other.value = nullptr;
			thiz._size = other._size;
			other._size = 0;
    	}

	};

	inline auto operator "" _s (
		const char* value,
		size_t size
	) -> String {
		return String{value, size};
	}

}

template <>
struct std::hash<Sen::Kernel::String> {
	auto operator()(const Sen::Kernel::String& str) const noexcept -> size_t {
		return Sen::Kernel::hash_string(str);
	}
};

template<typename Json>
struct jsoncons::json_type_traits<Json, Sen::Kernel::String> {
	using class_type = Sen::Kernel::String;
	using allocator_type = typename Json::allocator_type;
	using char_type = typename Json::char_type;
	using string_view_type = typename Json::string_view_type;

	static bool is(const Json &ajson) noexcept {
		if (!ajson.is_string()) return false;
		return true;
	}

	static auto as(const Json &ajson) -> class_type {
		if (!is(ajson)) throw conv_error(conv_errc::conversion_failed, "Not a String");
		std::string_view sv(ajson.as_string_view());
		class_type class_instance{sv.data(), sv.size()};
		return class_instance;
	}

	static Json to_json(const class_type &class_instance, allocator_type alloc = allocator_type()) {
		Json ajson(semantic_tag::none, alloc);
		ajson = class_instance.view();
		return ajson;
	}
};

template<>
struct jsoncons::is_json_type_traits_declared<Sen::Kernel::String> : public std::true_type {
};

template <>
struct jsoncons::extension_traits::is_string<Sen::Kernel::String> : std::true_type {};
