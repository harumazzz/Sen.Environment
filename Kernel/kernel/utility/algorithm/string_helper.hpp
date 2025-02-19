#pragma once

#include "kernel/utility/container/array/byte_array.hpp"
#include "kernel/utility/container/string/basic_string.hpp"
#include "kernel/utility/trait/trait.hpp"

namespace Sen::Kernel::StringHelper {
    using String = String;

    template <typename T>
    using List = CList<T>;

    template <typename T, auto number> requires std::is_floating_point_v<T> && std::is_integral_v<type_of<number>> && is_greater_than_zero_v<number>
    auto to_fixed (
        const T& value
    ) -> String {
        auto scale = std::pow(10, number);
        auto scaled_value = static_cast<i64>(value * scale + 0.5);
        auto integer_part = scaled_value / static_cast<i64>(scale);
        auto fractional_part = scaled_value % static_cast<i64>(scale);
        auto buffer = std::array<char, 64>{};
        auto len = static_cast<usize>(std::snprintf(buffer.data(), buffer.size(), "%lld", integer_part));
        buffer[len++] = '.';
        std::snprintf(buffer.data() + len, buffer.size() - len, "%0*lld", number, fractional_part);
        return String{ buffer.data(), len + number };
    }

    template <typename T> requires std::is_floating_point_v<T>
    auto to_fixed_6 (
        const T& value
    ) -> String {
        return to_fixed<T, 6>(value);
    }

    namespace Detail {

        constexpr auto uppercase_lookup(
        ) -> std::array<char, 256> {
            auto table = std::array<char, 256>{};
            for (auto i = 0; i < 256; ++i) {
                table[i] = (i >= 'a' && i <= 'z') ? i - 32 : i;
            }
            return table;
        }

        constexpr auto lowercase_lookup(
        ) -> std::array<char, 256> {
            auto table = std::array<char, 256>{};
            for (auto i = 0; i < 256; ++i) {
                table[i] = (i >= 'A' && i <= 'Z') ? i + 32 : i;
            }
            return table;
        }

        inline static constexpr auto toupper_lookup = uppercase_lookup();

        inline static constexpr auto tolower_lookup = lowercase_lookup();

    }

    inline auto to_uppercase(
        const char c
    ) -> char
    {
        return Detail::toupper_lookup[static_cast<uchar>(c)];
    }

    inline auto to_lowercase(
        const char c
    ) -> char {
        return Detail::tolower_lookup[static_cast<uchar>(c)];
    }

    inline auto make_uppercase_string (
        String& source
    ) -> void {
        for (auto & e : source) {
            e = to_uppercase(e);
        }
    }

    inline auto make_lowercase_string (
        String& source
    ) -> void {
        for (auto & e : source) {
            e = to_lowercase(e);
        }
    }

    inline auto compare_ordinal (
        const String& first,
        const String& second
    ) -> bool {
        if (first.size() != second.size()) {
            return false;
        }
        for (auto index : Range{first.size()}) {
            auto c1 = first[index];
            auto c2 = second[index];
            if (c1 == c2) {
                continue;
            }
            if (to_lowercase(static_cast<uchar>(first[index])) != to_lowercase(static_cast<uchar>(c2))) {
                return false;
            }
        }
        return true;
    }

    template <typename T> requires std::is_base_of_v<BaseContainer<String>, T>
    inline auto calculate_size (
        T& source
    ) -> usize {
        auto result = 0_size;
        for (auto & e : source) {
            result += e.size();
        }
        return result;
    }

    inline auto join_strings (
        List<String>& source,
        String& destination,
        const String& separator
    ) -> void {
        auto source_size = calculate_size(source);
        auto size = source.size() - 1;
        destination.allocate(source_size + separator.size() * size);
        for (auto& e : source | std::views::take(size)) {
            destination += e;
            destination += separator;
        }
        destination += source[size];
    }

    inline auto join_strings (
        List<String>& source,
        String& destination,
        char separator
    ) -> void {
        auto source_size = calculate_size(source);
        auto size = source.size() - 1;
        destination.allocate(source_size + sizeof(char) * size);
        for (auto& e : source | std::views::take(size)) {
            destination += e;
            destination += separator;
        }
        destination += source[size];
    }

    inline auto split_string (
        const String& source,
        const std::string_view& separator,
        List<String>& destination
    ) -> void {
        auto start = 0_size;
        auto end = 0_size;
        const auto sep_len = separator.size();
        while ((end = source.find(separator, start)) != String::none) {
            destination.append(source.sub(start, end - start));
            start = end + sep_len;
        }
        destination.append(source.sub(start));
    }

    inline auto split_string (
        const String& source,
        const String& separator,
        List<String>& destination
    ) -> void {
        return split_string(source, separator.view(), destination);
    }

    inline auto split_string (
        const String& source,
        const char separator,
        List<String>& destination
    ) -> void {
        auto start = 0_size;
        auto end = 0_size;
        while ((end = source.find(separator, start)) != String::none) {
            constexpr auto sep_len = 1;
            destination.append(source.sub(start, end - start));
            start = end + sep_len;
        }
        destination.append(source.sub(start));
    }

    inline auto make_string (
        const std::u8string_view& source
    ) -> String {
        return String{ reinterpret_cast<const char *>(source.data()), source.size() };
    }

    inline auto make_string (
        const std::string_view& source
    ) -> String {
        return String{ reinterpret_cast<const char *>(source.data()), source.size() };
    }

    namespace Detail {

        inline auto constexpr Table = std::array<char, 16>{
            '0', '1', '2', '3', '4', '5', '6', '7', '8',
            '9', 'a', 'b', 'c', 'd', 'e', 'f',
        };

    }

    template <typename T> requires std::is_base_of_v<BaseContainer<u8>, T>
    inline constexpr auto compute_bytes_to_hex_string_size(
        const T &data
    ) -> usize {
        return data.size() * 2;
    }

    template <typename T> requires std::is_base_of_v<BaseContainer<u8>, T>
    inline auto bytes_to_hex_string(
        const T &data
    ) -> String {
        auto result = String{compute_bytes_to_hex_string_size<T>(data)};
        auto i = 0_size;
        for (auto v : data) {
            result[i++] = Detail::Table[v >> 4];
            result[i++] = Detail::Table[v & 0xF];
        }
        return result;
    }

    inline auto make_string_view (
        const char* str
    ) -> std::string_view {
        return std::string_view{ str };
    }
}