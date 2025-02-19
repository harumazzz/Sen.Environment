#pragma once

#include "kernel/utility/container/array/byte_array.hpp"
#include "kernel/utility/container/string/basic_string.hpp"

namespace Sen::Kernel::StringParser {

    using String = BasicString;

    template <typename T> requires (std::is_floating_point_v<T> && !std::is_integral_v<T>)
    constexpr auto compute_floater_size(
    ) -> usize {
        constexpr auto max_digits = std::numeric_limits<T>::max_digits10;
        constexpr auto extra_chars = 10_size;
        return max_digits + extra_chars;
    }

    template <typename T> requires (!std::is_floating_point_v<T> && std::is_integral_v<T>)
    constexpr auto compute_integer_size(
    ) -> usize {
        constexpr auto max_digits = std::numeric_limits<T>::digits10;
        constexpr auto extra_chars = 1_size;
        return max_digits + extra_chars;
    }

    template <typename T> requires (std::is_floating_point_v<T> && !std::is_integral_v<T>)
    auto to_float (
        const String& data,
        const std::string_view& error
    ) -> T {
        auto destination = T{};
        auto result = Subprojects::mscharconv::from_chars(data.begin(), data.cend(), destination, Subprojects::mscharconv::chars_format::fixed);
        assert_conditional(result.ec == std::errc{}, fmt::format("{}", error), "to_float");
        return destination;
    }


    template <typename T> requires (!std::is_floating_point_v<T> && std::is_integral_v<T>)
    auto to_integer (
        const String& data,
        const std::string_view& error
    ) -> T {
        auto destination = T{};
        auto result = Subprojects::mscharconv::from_chars(data.begin(), data.cend(), destination, 10);
        assert_conditional(result.ec == std::errc{}, fmt::format("{}", error), "to_integer");
        return destination;
    }

    template <typename T> requires (std::is_floating_point_v<T> && !std::is_integral_v<T>)
    auto from_float (
        const T& value,
        const std::string_view& error
    ) -> String {
        auto destination = String{compute_floater_size<T>()};
        auto result = Subprojects::mscharconv::to_chars(destination.begin(), destination.end(), value, Subprojects::mscharconv::chars_format::fixed);
        assert_conditional(result.ec == std::errc{}, fmt::format("{}", error), "from_float");
        return destination;
    }

    template <typename T> requires (!std::is_floating_point_v<T> && std::is_integral_v<T>)
    auto from_integer (
        const T& value,
        const std::string_view& error
    ) -> String {
        auto destination = String{compute_integer_size<T>()};
        auto result = Subprojects::mscharconv::to_chars(destination.begin(), destination.end(), value, 10);
        assert_conditional(result.ec == std::errc{}, fmt::format("{}", error), "from_integer");
        return destination;
    }

    inline auto to_int8(
        const String& str,
        const std::string_view& error
    ) -> i8
    {
        return to_integer<i8>(str, error);
    }

    inline auto to_uint8(
        const String& str,
        const std::string_view& error
    ) -> u8
    {
        return to_integer<u8>(str, error);
    }

    inline auto to_size (
        const String& str,
        const std::string_view& error
    ) -> usize
    {
        return to_integer<usize>(str, error);
    }

   inline auto to_int16(
        const String& str,
        const std::string_view& error
    ) -> i16
    {
        return to_integer<i16>(str, error);
    }

   inline auto to_uint16(
        const String& str,
        const std::string_view& error
    ) -> u16
    {
        return to_integer<u16>(str, error);
    }

   inline auto to_int32(
        const String& str,
        const std::string_view& error
    ) -> i32
    {
        return to_integer<i32>(str, error);
    }

   inline auto to_uint32(
        const String& str,
        const std::string_view& error
    ) -> u32
    {
        return to_integer<u32>(str, error);
    }

   inline auto to_int64(
        const String& str,
        const std::string_view& error
    ) -> i64
    {
        return to_integer<i64>(str, error);
    }

   inline auto to_uint64(
        const String& str,
        const std::string_view& error
    ) -> u64
    {
        return to_integer<u64>(str, error);
    }

   inline auto to_float32(
        const String& str,
        const std::string_view& error
    ) -> f32
    {
        return to_float<float>(str, error);
    }

   inline auto to_float64(
        const String& str,
        const std::string_view& error
    ) -> f64
    {
        return to_float<f64>(str, error);
    }

    inline auto from_int8(
        i8 value,
        const std::string_view& error
    ) -> String {
        return from_integer<i8>(value, error);
    }

    inline auto from_uint8(
        u8 value,
        const std::string_view& error
    ) -> String {
        return from_integer<u8>(value, error);
    }

    inline auto from_int16(
        i16 value,
        const std::string_view& error
    ) -> String {
        return from_integer<i16>(value, error);
    }

    inline auto from_uint16(
        u16 value,
        const std::string_view& error
    ) -> String {
        return from_integer<u16>(value, error);
    }

    inline auto from_int32(
        i32 value,
        const std::string_view& error
    ) -> String {
        return from_integer<i32>(value, error);
    }

    inline auto from_uint32(
        u32 value,
        const std::string_view& error
    ) -> String {
        return from_integer<u32>(value, error);
    }

    inline auto from_int64(
        const i64& value,
        const std::string_view& error
    ) -> String {
        return from_integer<i64>(value, error);
    }

    inline auto from_uint64(
        const u64& value,
        const std::string_view& error
    ) -> String {
        return from_integer<u64>(value, error);
    }

    inline auto from_float32(
        f32 value,
        const std::string_view& error
    ) -> String {
        return from_float<f32>(value, error);
    }

    inline auto from_float64(
        const f64& value,
        const std::string_view& error
    ) -> String {
        return from_float<f64>(value, error);
    }


}