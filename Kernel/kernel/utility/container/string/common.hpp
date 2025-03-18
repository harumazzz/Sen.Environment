#pragma once

#include "kernel/utility/library.hpp"
#include "kernel/utility/container/array/byte_array.hpp"
#include "kernel/utility/container/list/list.hpp"

namespace Sen::Kernel {

    inline constexpr auto hash_string (
        std::string_view const & string
    ) -> std::uint64_t {
        constexpr auto offset = std::uint64_t{14695981039346656037ull};
        constexpr auto prime = std::uint64_t{1099511628211ull};
        auto result = offset;
        for (auto & element : string) {
            result ^= static_cast<std::uint8_t>(element);
            result *= prime;
        }
        return result;
    }

    inline auto replace_all(
        const std::string& str, 
        const std::string& old_sub, 
        const std::string& new_sub
    ) -> std::string
    {
        auto result = std::string{};
        result.reserve(str.size());
        auto pos = size_t{0};
        const auto old_sub_len = old_sub.size();
        while (true) {
            const auto found = str.find(old_sub, pos);
            if (found == std::string::npos) {
                result.append(str.substr(pos));
                break;
            }
            result.append(str, pos, found - pos);
            result.append(new_sub);
            pos = found + old_sub_len;
        }
        return result;
    }

    inline auto utf8_to_utf16(
        const std::string_view& str
    ) -> std::wstring
    {
        auto myconv = std::wstring_convert<std::codecvt_utf8<wchar_t>>{};
        return myconv.from_bytes(str.data(), str.data() + str.size());
    }

    inline auto utf16_to_utf8(
        const std::wstring_view& wstr
    ) -> std::string
    {
        auto myconv = std::wstring_convert<std::codecvt_utf8<wchar_t>>{};
        return myconv.to_bytes(wstr.data(), wstr.data() + wstr.size());
    }

    template<typename... Args>
    inline auto format(
        const std::string& str, 
        Args&&... args
    ) -> std::string
    {
        return fmt::format(fmt::runtime(str), std::forward<Args>(args)...);
    }

    namespace Detail {

        inline static auto constexpr Table = std::array<char, 16>{
            '0', '1', '2', '3', '4', '5', '6', '7', '8',
            '9', 'a', 'b', 'c', 'd', 'e', 'f',
        };

    }

    template <typename T> requires std::is_integral_v<T>
    inline constexpr auto decimal_to_hexadecimal(
        const T& number
    ) -> std::array<char, sizeof(T) * 2 + 1> {
        auto buffer = std::array<char, sizeof(T) * 2 + 1>{};
        auto value = static_cast<std::make_unsigned_t<T>>(number);
        if (value == 0) {
            buffer[0] = '0';
            buffer[1] = '\0';
            return buffer;
        }
        auto index = buffer.size() - 2;
        while (value > 0) {
            buffer[index--] = Detail::Table[value % 16];
            value /= 16;
        }
        buffer[buffer.size() - 1] = '\0';
        return buffer;
    }

    template <typename T> requires std::is_integral_v<T>
    inline constexpr bool is_whitespace_character(
        const T& c
    ) noexcept {
        return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v';
    }

    namespace Detail {

        inline static auto constexpr none = static_cast<size_t>(-1);

        template <typename Container>
        [[nodiscard]] inline constexpr auto find (
            const Container& other,
            const char pattern,
            const size_t& pos = 0
        ) -> size_t {
            for (auto i = pos; i < other.size(); ++i) {
                if (other[i] == pattern) {
                    return i;
                }
            }
            return none;
        }

        inline constexpr auto build_longest_prefab_suffix(
            const char* pattern,
            const size_t& m,
            SizeArray& lps
        ) -> void {
            auto len = 0_size;
            auto i = 1_size;
            while (i < m) {
                if (pattern[i] == pattern[len]) {
                    len++;
                    lps[i] = len;
                    ++i;
                }
                else {
                    if (len != 0) {
                        len = lps[len - 1];
                    }
                    else {
                        lps[i] = 0;
                        ++i;
                    }
                }
            }
        }

        template <typename Container>
        inline constexpr auto find(
            const Container& other,
            const char* pattern,
            const size_t& size,
            const usize& pos = 0
        ) -> usize {
            const auto m = size;
            const auto n = other.size();
            if (m == 0_size) return pos;
            if (n < m || pos >= n) return none;
            auto lps = SizeArray{m};
            build_longest_prefab_suffix(pattern, m, lps);
            auto i = pos;
            auto j = 0_size;
            while (i < n) {
                if (pattern[j] == other[i]) {
                    ++i;
                    ++j;
                }
                if (j == m) {
                    return i - j;
                }
                if (i < n && pattern[j] != other[i]) {
                    if (j != 0) {
                        j = lps[j - 1];
                    }
                    else {
                        ++i;
                    }
                }
            }
            return none;
        }

        template <typename Container>
        inline constexpr auto rfind(
            const Container& other,
            const char* pattern,
            const size_t& size,
            const usize& pos = none
        ) -> usize {
            const auto m = size;
            const auto n = other.size();
            if (m == 0_size) return pos;
            if (n < m) return none;
            auto bad_character = std::array<size_t, 256>{};
            for (auto i = 0_size; i < m; ++i) {
                bad_character[pattern[i]] = i;
            }
            const auto start = (pos == none) ? n - m : std::min(pos, n - m);
            for (auto i = start; i >= m - 1; --i) {
                auto j = m - 1;
                while (pattern[j] == other[i + j]) {
                    if (j == 0_size) {
                        return i;
                    }
                    --j;
                }
                auto shift = m - 1 - bad_character[other[i]];
                if (shift < 1_size) shift = 1;
                i -= shift;
            }
            return none;
        }

        template <typename Container>
        inline constexpr auto find_all (
            const Container& other,
            const char* target,
            const size_t& length
        ) -> List<usize>  {
            auto positions = List<usize>{};
            if (length == 0 || other.size() < length) {
                return positions;
            }
            auto pos = 0_size;
            while ((pos = find(other, target, length, pos)) != none) {
                positions.append(pos);
                pos += length;
            }
            return positions;
        }

        template <typename Container>
        [[nodiscard]] inline constexpr auto find_all (
            const Container& other,
            const char target
        ) -> List<usize>  {
            auto positions = List<usize>{};
            if (other.size() < 1) {
                return positions;
            }
            auto pos = 0_size;
            while ((pos = find(other, target, pos)) != none) {
                positions.append(pos);
                pos += 1;
            }
            return positions;
        }

        template <typename Container>
        inline constexpr auto starts_with (
            const Container& other,
            const char* pattern,
            const usize& m
        ) -> bool {
            if (m > other.size()) {
                return false;
            }
            return std::memcmp(other.begin(), pattern, m) == 0;
        }

        template <typename Container>
        inline constexpr auto ends_with (
            const Container& other,
            const char* pattern,
            const usize& m
        ) -> bool {
            if (m > other.size()) {
                return false;
            }
            return std::memcmp(other.begin() + other.size() - m, pattern, m) == 0;
        }

    }

    #if WINDOWS
    inline auto u16_to_u8(
        const std::u16string_view& u16
    ) -> std::string
    {
        if (u16.empty()) {
            return std::string{};
        }
        const auto size_needed = static_cast<size_t>(WideCharToMultiByte(CP_UTF8, 0, reinterpret_cast<LPCWCH>(u16.data()),
            static_cast<int>(u16.size()), nullptr, 0, nullptr, nullptr));
        if (size_needed <= 0) {
            return std::string{};
        }
        auto result = std::string{};
        result.resize(size_needed);
        WideCharToMultiByte(CP_UTF8, 0, reinterpret_cast<LPCWCH>(u16.data()),
            static_cast<int>(u16.size()), result.data(), static_cast<int>(size_needed),
            nullptr, nullptr);
        return result;
    }

    inline auto u8_to_u16(
        const std::string_view& u8
    ) -> std::u16string
    {
        if (u8.empty()) {
            return std::u16string{};
        }
        const auto size_needed = static_cast<size_t>(MultiByteToWideChar(CP_UTF8, 0, u8.data(), static_cast<int>(u8.size()), nullptr, 0));
        if (size_needed <= 0) {
            return std::u16string{};
        }
        auto result = std::u16string{};
        result.resize(size_needed);
        MultiByteToWideChar(CP_UTF8, 0, u8.data(), static_cast<int>(u8.size()), reinterpret_cast<LPWSTR>(result.data()), static_cast<int>(size_needed));
        return result;
    }
    #endif

}
