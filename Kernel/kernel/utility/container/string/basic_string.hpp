#pragma once

#include "kernel/utility/macro.hpp"
#include "kernel/utility/container/string/common.hpp"

namespace Sen::Kernel {

	class BasicString;

	using String = BasicString;

    class BasicString {

	public:

		using Size = std::size_t;

		template <typename U>
		using Pointer = U*;

		using Character = char;

		using Iterator = Pointer<Character>;

		inline static auto constexpr none = static_cast<size_t>(-1);

	protected:

		Pointer<char> value{ nullptr };

		Size _size{ 0 };

	public:

		constexpr explicit BasicString(

		) = default;

		constexpr explicit BasicString(
			const Size& size
		) : value{ new Character[size + 1] }, _size{ size }
		{
			thiz.value[size] = '\0';
		}

		explicit BasicString(
			const char* data
		) : value{nullptr}, _size{ std::strlen(data) }
		{
			thiz.value = new Character[thiz._size + 1];
			thiz.value[thiz._size] = '\0';
			std::memcpy(thiz.value, data, thiz._size);
		}

		explicit BasicString(
			const char* data,
			const Size& size
		) : value{ new Character[size + 1] }, _size{ size }
		{
			thiz.value[thiz._size] = '\0';
			std::memcpy(thiz.value, data, thiz._size);
		}

		constexpr auto data(

		) -> Pointer<Character>&
		{
			return thiz.value;
		}

		constexpr auto begin(
		) -> Pointer<Character> {
			return thiz.value;
		}

		constexpr auto end(
		) -> Pointer<Character> {
			return thiz.value + thiz._size;
		}

		auto allocate(
			Size const& size
		) -> void
		{
			delete[] thiz.value;
			thiz.value = new Character[size + 1];
			thiz._size = size;
			std::memset(thiz.value, 0, size + 1);
		}

		~BasicString(

		)
		{
			if (thiz.value != nullptr) {
				delete[] value;
				thiz.value = nullptr;
			}
		}

    	BasicString(
			const BasicString& other
		) = delete;

		auto operator =(
			const BasicString& other
		)->BasicString & = delete;

    	BasicString(
			BasicString&& other
		) noexcept : value{ other.value }, _size{ other._size }
		{
			other.value = nullptr;
			other._size = 0;
		}

		constexpr auto operator=(
			BasicString&& other
			) noexcept -> BasicString& {
			if (this != &other) {
				delete[] thiz.value;
				thiz._size = other._size;
				thiz.value = other.value;
				other.value = nullptr;
				other._size = 0;
			}
			return *this;
		}

		constexpr auto operator [](
			Size const& index
		) -> Character&
		{
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

		constexpr auto operator != (
			const BasicString& other
		) const -> bool
		{
			return !(thiz.operator==(other));
		}

		auto operator < (
			const BasicString& other
		) -> bool const = delete;

		auto operator > (
			const BasicString& other
		) = delete;

		auto operator <= (
			const BasicString& other
		) -> bool const = delete;

		auto operator >= (
			const BasicString& other
		) -> bool const = delete;

		constexpr auto size(

		) -> Size
		{
			return thiz._size;
		}

		friend auto operator <<(std::ostream &os, const BasicString& other) -> std::ostream& {
			if (other.value != nullptr) {
				os.write(other.value, other._size);
			}
			return os;
		}

		constexpr auto front(
		) -> Pointer<char> {
			return thiz.value;
		}

		constexpr auto back(

		) -> Pointer<char>
		{
			return thiz.value + thiz._size;
		}

		constexpr auto rbegin(
		) -> Pointer<char> {
			return thiz.value + thiz._size - 1;
		}

		constexpr auto rend(
		) -> Pointer<char> {
			return thiz.value + thiz._size - 1;
		}

		constexpr auto cbegin(
		) const -> Pointer<char> {
			return thiz.value;
		}

		constexpr auto cend(
		) const -> Pointer<char> {
			return thiz.value + thiz._size;
		}

		constexpr auto empty(

		) -> bool
		{
			return thiz._size = 0;
		}

		constexpr auto string(

		) const -> std::string
		{
			return std::string{ thiz.value, thiz._size };
		}

		auto wstring(

		) -> std::wstring
		{
			#if _WIN32
			auto size = static_cast<size_t>(MultiByteToWideChar(CP_UTF8, 0, thiz.value, static_cast<int>(thiz._size), nullptr, 0));
			auto destination = std::wstring{};
			destination.resize(size);
			MultiByteToWideChar(CP_UTF8, 0, thiz.value, static_cast<int>(thiz._size), &destination[0], size);
			return destination;
			#else
			auto convert = std::wstring_convert<std::codecvt_utf8<wchar_t>>{};
			return convert.from_bytes(thiz.value, thiz.value + thiz._size);
			#endif
		}

		constexpr operator std::string_view() {
			return std::string_view{ thiz.value, thiz._size };
		}

		constexpr auto length(

		) -> Size
		{
			return thiz._size;
		}

		static constexpr auto max_size(
		) -> Size {
			return std::numeric_limits<Size>::max();
		}

		auto clone(

		) const -> BasicString
		{
			return BasicString{ thiz.value, thiz._size };
		}

		constexpr auto release(

		) -> std::tuple<Pointer<char>, Size>
		{
			auto raw = thiz.value;
			auto size = thiz._size;
			thiz.value = nullptr;
			thiz._size = 0;
			return std::make_tuple(raw, size);
		}

		auto clear(

		) -> void
		{
			thiz._size = 0;
			if (thiz.value != nullptr) {
				delete[] thiz.value;
				thiz.value = nullptr;
			}
		}

		auto insert(
			const Size& index,
			Pointer<const char> str,
			const Size& len
		) -> void {
			assert_conditional(index < thiz._size, fmt::format("Inserted index is larger than the size of the string"), "insert");
			auto new_size = thiz._size + len;
			auto raw = new Character[new_size + 1];
			std::memcpy(raw, thiz.value, index);
			std::memcpy(raw + index, str, len);
			std::memcpy(raw + index + len, thiz.value + index, thiz._size - index);
			raw[new_size] = '\0';
			delete[] thiz.value;
			thiz.value = raw;
			thiz._size = new_size;
		}

		auto insert(
			const Size& index,
			const Character& ch
		) -> void {
			auto temporary = std::array<char, 1>{ ch };
			return insert(index, temporary.data(), temporary.size());
		}

		auto insert(
			const Size& index,
			const BasicString& other
		) -> void {
			return insert(index, other.value, other._size);
		}

		auto erase(
			const Size& index,
			const Size& len
		) -> BasicString& {
			assert_conditional(index < thiz._size, fmt::format("Erased index is larger than the size of the string"), "erase");
			return erase(thiz.value + index, thiz.value + std::min(index + len, thiz._size));
		}

		auto erase(
			Iterator first,
			Iterator last
		) -> BasicString& {
			assert_conditional(first >= thiz.value && first <= thiz.end(), fmt::format("Erase begin iterator is outside bounds of the String iterator"), "erase");
			assert_conditional(last >= thiz.value && last <= thiz.end(), fmt::format("Erase end iterator is outside bounds of the String iterator"), "erase");
			assert_conditional(first <= last, fmt::format("Cannot erase if the first iterator is larger than last iterator"), "erase");
			auto erase_len = static_cast<Size>(last - first);
			auto new_size = thiz._size - erase_len;
			std::memcpy(first, last, thiz.end() - last);
			thiz._size = new_size;
			thiz.value[thiz._size] = '\0';
			return thiz;
		}

		auto push(
			const Character& c
		) -> void
		{
			auto new_size = thiz._size + 1;
			auto new_value = new Character[new_size + 1];
			std::memcpy(new_value, thiz.value, thiz._size);
			new_value[thiz._size] = c;
			new_value[new_size] = '\0';
			if (thiz.value != nullptr) {
				delete[] thiz.value;
			}
			thiz.value = new_value;
			thiz._size = new_size;
			return;
		}

		constexpr auto pop(
		) -> void {
			if (thiz._size > 0) {
				--thiz._size;
				thiz.value[thiz._size] = '\0';
			}
		}

		auto append(
			const Size& count,
			Character ch
		) -> BasicString& {
			auto new_size = thiz._size + count;
			auto new_value = new Character[new_size + 1];
			std::memcpy(new_value, thiz.value, thiz._size);
			std::memset(new_value + thiz._size, ch, count);
			new_value[new_size] = '\0';
			delete[] thiz.value;
			thiz.value = new_value;
			thiz._size = new_size;
			return thiz;
		}

		auto append(
			Pointer<const Character> s,
			const Size& count
		) -> BasicString& {
			auto new_size = thiz._size + count;
			auto new_value = new Character[new_size + 1];
			std::memcpy(new_value, thiz.value, thiz._size);
			std::memcpy(new_value + thiz._size, s, count);
			new_value[new_size] = '\0';
			delete[] thiz.value;
			thiz.value = new_value;
			thiz._size = new_size;
			return thiz;
		}

		auto append(
			Pointer<const Character> s
		) -> BasicString& {
			return append(s, std::strlen(s));
		}

		auto append(
			const BasicString& str
		) -> BasicString& {
			return append(str.value, str._size);
		}

		auto append(
			const BasicString& str,
			const Size& pos,
			const Size& count = none
		) -> BasicString& {
			assert_conditional(pos <= str._size, fmt::format("Cannot append if the position is larger than the size of the string. The position: {}, String size: {}", pos, str._size), "append");
			return append(str.value + pos, std::min(count, str._size - pos));
		}

		template <typename Iterate>
		auto append(
			const Iterate& first,
			const Iterate& last
		) -> BasicString& {
			auto count = last - first;
			return append(&(*first), count);
		}

		auto append(
			std::initializer_list<Character> ilist
		) -> BasicString& {
			return append(ilist.begin(), ilist.size());
		}

		auto replace(
			const Size& pos,
			const Size& count,
			const BasicString& str
		) -> BasicString& {
			return replace(pos, count, str.value, str._size);
		}

		auto replace(
			Pointer<Character> first,
			Pointer<Character> last,
			const BasicString& str
		) -> BasicString& {
			return replace(first - value, last - first, str.value, str._size);
		}

		auto replace(
			const Size& pos,
			const Size& count,
			const BasicString& str,
			const Size& pos2,
			const Size& count2 = none
		) -> BasicString& {
			assert_conditional(pos2 <= str._size, fmt::format("Cannot replace if the position is larger than the size of the string. The position: {}, String size: {}", pos2, str._size), "replace");
			return replace(pos, count, str.value + pos2, std::min(count2, str._size - pos2));
		}

		auto replace(
			const Size& pos,
			const Size& count,
			Pointer<const Character> cstr,
			const Size& count2
		) -> BasicString& {
			auto new_size = _size - count + count2;
			auto new_value = new Character[new_size + 1];
			std::memcpy(new_value, thiz.value, pos);
			std::memcpy(new_value + pos, cstr, count2);
			std::memcpy(new_value + pos + count2, thiz.value + pos + count, thiz._size - pos - count);
			new_value[new_size] = '\0';
			delete[] thiz.value;
			thiz.value = new_value;
			thiz._size = new_size;
			return thiz;
		}

		auto replace(
			Pointer<Character> first,
			Pointer<Character> last,
			Pointer<const Character> cstr,
			const Size& count2
		) -> BasicString& {
			return replace(first - value, last - first, cstr, count2);
		}

		auto replace(
			const Size& pos,
			const Size& count,
			Pointer<const Character> cstr
		) -> BasicString& {
			return replace(pos, count, cstr, std::strlen(cstr));
		}

		auto replace(
			Pointer<Character> first,
			Pointer<Character> last,
			Pointer<const Character> cstr
		) -> BasicString& {
			return replace(first - thiz.value, last - first, cstr, std::strlen(cstr));
		}

		auto replace(
			const Size& pos,
			const Size& count,
			const Size& count2,
			Character ch
		) -> BasicString& {
			auto new_str = BasicString{ count2 };
			std::memset(new_str.value, ch, count2);
			return replace(pos, count, new_str);
		}

		auto replace(
			Pointer<Character> first,
			Pointer<Character> last,
			const Size& count2,
			Character ch
		) -> BasicString& {
			return replace(first - value, last - first, count2, ch);
		}

		template <typename It>
		auto replace(
			Pointer<Character> first,
			Pointer<Character> last,
			It first2,
			It last2
		) -> BasicString& {
			return replace(first - value, last - first, &(*first2), last2 - first2);
		}

		auto replace(
			Pointer<Character> first,
			Pointer<Character> last,
			std::initializer_list<Character> ilist
		) -> BasicString& {
			return replace(first - value, last - first, ilist.begin(), ilist.size());
		}

		auto copy(
			Pointer<Character> source,
			const Size& size
		) -> void
		{
			std::memcpy(source, thiz.value, size);
		}

		auto operator+=(
			const BasicString& other
		) -> BasicString& {
			auto new_size = thiz._size + other._size;
			auto new_value = new Character[new_size + 1];
			std::memcpy(new_value, thiz.value, thiz._size);
			std::memcpy(new_value + thiz._size, other.value, other._size);
			new_value[new_size] = '\0';
			delete[] thiz.value;
			thiz.value = new_value;
			thiz._size = new_size;
			return thiz;
		}

		auto operator+=(
			const char* other
		) -> BasicString& {
			auto other_size = std::strlen(other);
			auto new_size = thiz._size + other_size;
			auto new_value = new Character[new_size + 1];
			std::memcpy(new_value, thiz.value, thiz._size);
			std::memcpy(new_value + thiz._size, other, other_size);
			new_value[new_size] = '\0';
			delete[] thiz.value;
			thiz.value = new_value;
			thiz._size = new_size;
			return thiz;
		}

		auto operator+=(
			Character other
		) -> BasicString& {
			auto new_size = thiz._size + 1;
			auto new_value = new Character[new_size + 1];
			std::memcpy(new_value, thiz.value, thiz._size);
			new_value[thiz._size] = other;
			new_value[new_size] = '\0';
			delete[] thiz.value;
			thiz.value = new_value;
			thiz._size = new_size;
			return thiz;
		}

		auto operator+=(
			std::initializer_list<Character> ilist
		) -> BasicString& {
			auto new_size = thiz._size + ilist.size();
			auto new_value = new Character[new_size + 1];
			std::memcpy(new_value, thiz.value, thiz._size);
			std::memcpy(new_value + thiz._size, ilist.begin(), ilist.size());
			new_value[new_size] = '\0';
			delete[] thiz.value;
			thiz.value = new_value;
			thiz._size = new_size;
			return thiz;
		}

		template <typename StringViewLike> requires requires (StringViewLike str){
			{ str.data() } -> std::same_as<Pointer<const char>>;
			{ str.size() } -> std::same_as<Size>;
		}
		auto operator+=(
			const StringViewLike& str
		) -> BasicString& {
			auto new_size = thiz._size + str.size();
			auto new_value = new Character[new_size + 1];
			std::memcpy(new_value, thiz.value, thiz._size);
			std::memcpy(new_value + thiz._size, str.data(), str.size());
			new_value[new_size] = '\0';
			delete[] thiz.value;
			thiz.value = new_value;
			thiz._size = new_size;
			return thiz;
		}

	};

}