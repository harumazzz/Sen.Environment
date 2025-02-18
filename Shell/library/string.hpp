#pragma once

#include "builtin.hpp"
#include "library/standard.hpp"
#include "library/macro.hpp"

#if WINDOWS
#include <Windows.h>
#endif

namespace Sen::Shell {

    #if WINDOWS

    inline auto u16_to_u8(
        const std::u16string_view& u16
    ) -> std::string
    {
        if (u16.empty()) {
            return std::string{};
        }
        const auto size_needed = static_cast<usize>(WideCharToMultiByte(CP_UTF8, 0, reinterpret_cast<LPCWCH>(u16.data()),
                                              static_cast<int>(u16.size()), nullptr, 0, nullptr, nullptr));
        if (size_needed <= 0) {
            return std::string{};
        }
        auto result = std::string{};
        result.resize(size_needed);
        WideCharToMultiByte(CP_UTF8, 0, reinterpret_cast<LPCWCH>(u16.data()),
                             static_cast<int>(u16.size()), result.data(), static_cast<i32>(size_needed),
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
        const auto size_needed = static_cast<usize>(MultiByteToWideChar(CP_UTF8, 0, u8.data(), static_cast<int>(u8.size()), nullptr, 0));
        if (size_needed <= 0) {
            return std::u16string{};
        }
        auto result = std::u16string{};
        result.resize(size_needed);
        MultiByteToWideChar(CP_UTF8, 0, u8.data(), static_cast<int>(u8.size()), reinterpret_cast<LPWSTR>(result.data()), static_cast<int>(size_needed));
        return result;
    }

    #endif


    inline static auto constexpr offset = u64{4695981039346656037ull};

    inline static auto constexpr prime = u64{1099511628211ull};

    inline constexpr auto hash_string (
        const std::string_view& string
    ) -> std::uint64_t {
        auto result = offset;
        for (auto & element : string) {
            result ^= static_cast<std::uint8_t>(element);
            result *= prime;
        }
        return result;
    }

    inline auto left_trim (
        std::string &s
    ) -> void {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](auto&& ch) {
            return !std::isspace(ch);
        }));
    }
    
    inline auto right_trim (
        std::string &s
    ) -> void {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](auto&& ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

    inline auto trim (
        std::string& s
    ) -> void
    {
        left_trim(s);
        right_trim(s);
    }

}