#pragma once

#include "kernel/utility/library.hpp"

namespace Sen::Kernel {

    inline constexpr auto hash_string (
        std::string_view const & string
    ) -> std::uint64_t {
        auto offset = std::uint64_t{14695981039346656037ull};
        auto prime = std::uint64_t{1099511628211ull};
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
        auto old_sub_len = old_sub.size();
        auto new_sub_len = new_sub.size();
        while (true) {
            auto found = str.find(old_sub, pos);
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

    inline auto split(
        std::string_view str,
        std::string_view delimiter
    ) -> List<string>
    {
        auto result = List<std::string>{};
        auto pos = 0;
        auto max_splits = std::count(str.begin(), str.end(), delimiter.front()) + 1; 
        result.reserve(max_splits);
        while ((pos = str.find(delimiter)) != std::string_view::npos) {
            result.push_back(std::string{str.substr(0, pos)});
            str.remove_prefix(pos + delimiter.length());
        }
        result.push_back(std::string{str});
        return result;
    }

    inline auto join(
        const List<string> &data,
        std::string_view delimiter
    ) -> std::string
    {
        auto result = std::string{};
        result.reserve((delimiter.size() * (data.size() - 1)) + std::accumulate(data.begin(), data.end(), size_t{0}, [](auto old, auto& current){
            return old + current.size();
        }));
        for(auto i : Range{data.size()})
        {
            result += data[i];
            if (i == (data.size() - 1)) {
                break;
            }
            else {
                result += delimiter;
            }
        }
        return result;
    }

    inline auto to_posix_style(
        std::string_view source
    ) -> std::string
    {
        auto destination = std::string{source.data(), source.size()};
        std::replace(destination.begin(), destination.end(), '\\', '/');
        return destination;
    }

    inline auto to_windows_style(
        std::string_view source
    ) -> std::string
    {
        auto destination = std::string{source.data(), source.size()};
        std::replace(destination.begin(), destination.end(), '/', '\\');
        return destination;
    }

    inline auto to_windows_style(
        std::wstring_view source
    ) -> std::wstring
    {
        auto destination = std::wstring{source.data(), source.size()};
        std::replace(destination.begin(), destination.end(), L'/', L'\\');
        return destination;
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

    template<typename... Args> requires (std::is_same<Args, std::string_view>::value && ...) or 
    (std::is_same<Args, std::string>::value && ...)
    inline auto format(
        std::string str, 
        Args... args
    ) -> std::string
    {
        auto placeholder = "{}"_sv;
        auto replacePlaceholder = [&](auto arg) -> void {
            auto pos = str.find(placeholder);
            if (pos != std::string::npos) {
                str.replace(pos, placeholder.length(), arg);
            }
        };
        (replacePlaceholder(args), ...);
        return str;
    }

    template <typename T> requires std::is_integral<T>::value || std::is_floating_point<T>::value
    inline auto decimal_to_hexadecimal (
        T decNumber
    ) -> std::string 
    {
        auto ss = std::stringstream{};
        ss << std::hex << decNumber;
        return ss.str();
    }

}