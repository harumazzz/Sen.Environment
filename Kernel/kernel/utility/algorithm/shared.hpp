
#pragma once

#include "kernel/utility/algorithm/utility.hpp"
#include "kernel/utility/container/optional/optional.hpp"
#include "kernel/utility/byte_stream/mapped_stream.hpp"
#include "kernel/utility/byte_stream/common.hpp"

namespace Sen::Kernel {

    static constexpr auto k_block_size = 4_size;

    static constexpr auto k_string_null = 4_size;

    static constexpr auto k_block_width = 32_size;

    static constexpr auto k_chunk_width = 32_size;

    static constexpr auto k_padding_unit_size = 0x1000_size;

    static constexpr auto k_none_size = 0_size;

    static constexpr auto k_begin_index = 0_size;

    static constexpr auto k_string_empty = ""_sv;

    static auto get_iv_from_key(String const &key) -> String
    {
        assert_conditional(key.size() == 32_size, "Key must have 32 characters", "get_iv_from_key");
        return key.sub(4_size, 28_size);
    }

    template <typename Type> requires std::is_arithmetic_v<Type>
    static constexpr auto value_max() -> Type
    {
        return std::numeric_limits<Type>::max();
    }

    template <typename Type> requires std::is_arithmetic_v<Type>
    static constexpr auto value_min() -> Type
    {
        return std::numeric_limits<Type>::min();
    }

    template <typename Type> requires std::is_arithmetic_v<Type>
    static constexpr auto type_bit_count() -> size_t
    {
        return sizeof(Type) * 8_size;
    }

    template <typename Type>
    static auto compare_value_to_reset(Optional<Type>& data, Type const &value)
    {
        if (*data == value)
        {
            data.reset();
        }
    }

    static auto compare_value_to_reset(Optional<String>& data, std::string_view const value)
    {
        if (*data == value)
        {
            data.reset();
        }
    }

    static auto fourcc_from_integer(u32 const &data, String &value) -> void {
        value += static_cast<char>(clip_bit(data, 24_size, 8_size));
        value += static_cast<char>(clip_bit(data, 16_size, 8_size));
        value += static_cast<char>(clip_bit(data, 8_size, 8_size));
        value += static_cast<char>(clip_bit(data, 0_size, 8_size));
        assert_conditional(value.size() == 4_size, "Invalid fourcc siz", "fourcc_from_integer");
    }

    static auto fourcc_to_integer(u32 &data, String const &value) -> void {
        data |= static_cast<uint32_t>(value[0] << 0);
        data |= static_cast<uint32_t>(value[1] << 8);
        data |= static_cast<uint32_t>(value[2] << 16);
        data |= static_cast<uint32_t>(value[3] << 24);
        assert_conditional(value.size() == 4_size, "Invalid fourcc siz", "fourcc_to_integer");
    }


    template <typename Type> requires is_integer<Type>
    static constexpr auto log2n(Type const &value) -> Type {
       return static_cast<Type>(1 << static_cast<Type>((std::log2(value) + 0.5)));
    }

    /*
    template <typename Size, typename Type, typename Exchanger, is_read_stream ReadStream> requires (std::is_void_v<Size> || is_numeric_v<Size>) && is_not_void<Type> && is_exchanger<Exchanger> && is_readable_stream<ReadStream>
   static auto exchange_array(Exchanger const &exchanger, ReadStream &stream, CArray<Type> &value) -> void {
        if constexpr (!std::is_void_v<Size>) {
            value.allocate(static_cast<size_t>(stream.template numeric<Size>()));
        }
        for (auto &element : value) {
            exchanger(stream, element);
        }
    }

    template <typename Size, typename Type, typename Exchanger, is_write_stream WriteStream> requires (std::is_void_v<Size> || is_numeric_v<Size>) && is_not_void<Type> && is_exchanger<Exchanger> && is_writable_stream<WriteStream>
    static auto exchange_array(Exchanger const &exchanger, WriteStream &stream, CArrayView<Type> const &value) -> void {
        if constexpr (!std::is_void_v<Size>) {
            stream.template numeric<Size>(static_cast<Size>(value.size()));
        }
        for (auto const &element : value) {
            exchanger(stream, element);
        }
    }
    */

    template <typename Size, typename Type, typename Exchanger, is_read_stream ReadStream> requires (std::is_void_v<Size> || is_numeric_v<Size>) && is_not_void<Type> && is_exchanger<Exchanger>
    static auto exchange_list(Exchanger const &exchanger, ReadStream &stream, List<Type> &value) -> void {
        if constexpr (!std::is_void_v<Size>) {
            value.resize(static_cast<size_t>(stream.template numeric<Size>()));
        }
        for (auto &element : value) {
            exchanger(stream, element);
        }
    }

    template <typename Size, typename Type, typename Exchanger, is_write_stream WriteStream> requires (std::is_void_v<Size> || is_numeric_v<Size>) && is_not_void<Type> && is_exchanger<Exchanger>
    static auto exchange_list(Exchanger const &exchanger, WriteStream &stream, List<Type> const &value) -> void {
        if constexpr (!std::is_void_v<Size>) {
            stream.template numeric<Size>(static_cast<Size>(value.size()));
        }
        for (auto &element : value) {
            exchanger(stream, element);
        }
    }

    template <is_numeric_v Raw, typename Type, is_read_stream ReadStream> requires std::is_enum_v<Type>
    static auto exchange_enumeration(ReadStream &stream, Type &value) -> void {
        using underlying_type = std::underlying_type_t<Type>;
        auto index = stream.template numeric<Raw>();
        const auto is_valid_enum = index >= static_cast<Raw>(value_min<underlying_type>()) && index <= static_cast<Raw>(static_cast<Type>(value_max<underlying_type>()));
        assert_conditional(is_valid_enum, fmt::format("Failed to convert enum {} by value {}", VAR_TO_STRING(Type), index), "exchange_enumeration");
        value = static_cast<Type>(index);
    }

    template <is_numeric_v Raw, typename Type, is_write_stream WriteStream> requires std::is_enum_v<Type>
    static auto exchange_enumeration(WriteStream &stream, Type const &value) -> void {
        using underlying_type = std::underlying_type_t<Type>;
        const auto index = static_cast<Raw>(value);
        const auto is_valid_enum = index >= static_cast<Raw>(value_min<underlying_type>()) && index <= static_cast<Raw>(static_cast<Type>(value_max<underlying_type>()));
        assert_conditional(is_valid_enum, fmt::format("Failed to convert enum {} by value {}", VAR_TO_STRING(Type), index), "exchange_enumeration");
        stream.template numeric<Raw>(index);
    }

    template<is_numeric_v Type, is_read_stream ReadStream>
    static auto exchange_numeric(ReadStream &stream, Type &value) -> void {
        value = static_cast<Type>(stream.numeric());
    }

    template<is_numeric_v Type, is_write_stream WriteStream>
    static auto exchange_numeric(WriteStream &stream, Type const &value) -> void {
        stream.numeric(value);
    }

    template<is_numeric_v Type, is_read_stream ReadStream>
    static auto exchange_numeric(ReadStream &stream, Optional<Type> &value) -> void {
        value = Type{};
        exchange_numeric(stream, value);
    }

    template<is_numeric_v Type, is_write_stream WriteStream>
    static auto exchange_numeric(WriteStream &stream, Optional<Type> const &value) -> void {
        const auto n_value = value.has_value() ? *value : Type{};
        exchange_numeric(stream, n_value);
    }

    template<is_numeric_v Raw, is_numeric_v Type, is_read_stream ReadStream>
    static auto exchange_numeric_fixed(ReadStream &stream, Type &value) -> void {
        value = static_cast<Type>(stream.template numeric<Raw>());
    }

    template<is_numeric_v Raw, is_numeric_v Type, is_write_stream WriteStream>
    static auto exchange_numeric_fixed(WriteStream &stream, Type const &value) -> void {
        stream.template numeric<Raw>(value);
    }

    template<is_numeric_v Raw, is_numeric_v Type, is_read_stream ReadStream>
    static auto exchange_numeric_fixed(ReadStream &stream, Optional<Type> &value) -> void {
        value = Type{};
        exchange_numeric_fixed<Raw>(stream, value);
    }

    template<is_numeric_v Raw, is_numeric_v Type, is_write_stream WriteStream>
    static auto exchange_numeric_fixed(WriteStream &stream, Optional<Type> const &value) -> void {
        const auto n_value = value.has_value() ? *value : Type{};
        exchange_numeric_fixed<Raw>(stream, n_value);
    }

    template <is_numeric_v Size, typename Type, is_read_stream ReadStream> requires is_list_v<Type>
    static auto exchange_list_size(ReadStream &stream, Type &value) -> void {
        value.resize(static_cast<usize>(stream.template numeric<Size>()));
    }

    template <is_numeric_v Size, typename Type, is_write_stream WriteStream> requires is_list_v<Type>
    static auto exchange_list_size(WriteStream &stream, Type const &value) -> void {
        stream.template numeric<Size>(static_cast<Size>(value.size()));
    }

    template <is_numeric_v Size, typename Type, is_read_stream ReadStream> requires is_array_v<Type>
    static auto exchange_array_size(ReadStream &stream, Type &value) -> void {
        value.allocate(static_cast<usize>(stream.template numeric<Size>()));
    }

    template <is_numeric_v Size, typename Type, is_write_stream WriteStream> requires is_array_v<Type>
    static auto exchange_array_size(WriteStream &stream, Type const &value) -> void {
        stream.template numeric<Size>(static_cast<Size>(value.size()));
    }

    template <is_numeric_v Type, is_read_stream ReadStream>
    static auto exchange_string_block(ReadStream &stream, String &value) -> void {
        value = stream.template string_numeric<Type>();
    }

    template <is_numeric_v Type, is_write_stream WriteStream>
    static auto exchange_string_block(WriteStream &stream, String const &value) -> void {
        stream.template string_numeric<Type>(value);
    }

    template <is_numeric_v Type, is_read_stream ReadStream>
    static auto exchange_string_block(ReadStream &stream, Optional<String> &value) -> void {
        value = String{};
        exchange_string_block<Type>(stream, *value);
    }

    template <is_numeric_v Type, is_write_stream WriteStream>
    static auto exchange_string_block(WriteStream &stream, Optional<String> const &value) -> void {
        auto s_value = value.has_value() ? *value : String{};
        exchange_string_block<Type>(stream, s_value);
    }

    template <is_read_stream ReadStream>
   static auto exchange_boolean(ReadStream &stream, bool &value) -> void {
        value = stream.boolean();
    }

    template <auto block_size, is_read_stream ReadStream>
    static auto exchange_string_block_size(ReadStream &stream, String &value) -> void {
        value = stream.template string_block<block_size>();
    }

    template <auto block_size, is_write_stream WriteStream>
    static auto exchange_string_block_size(WriteStream &stream, String const &value) -> void {
        stream.template string_block<block_size>(value);
    }

    template <is_write_stream WriteStream>
    static auto exchange_boolean(WriteStream &stream, bool const &value) -> void {
        stream.boolean(value);
    }

    template <is_read_stream ReadStream>
    static auto exchange_boolean(ReadStream &stream, Optional<bool> &value) -> void {
        value = false;
        exchange_boolean<ReadStream>(stream, *value);
    }

    template <is_write_stream WriteStream>
    static auto exchange_boolean(WriteStream &stream, Optional<bool> const &value) -> void {
        const auto c_value = value.has_value() ? *value : false;
        exchange_boolean<WriteStream>(stream, c_value);
    }

    template <auto rate, typename Type, is_read_stream ReadStream> requires std::is_same_v<type_of<rate>, f32> && std::is_arithmetic_v<Type>
    static auto exchange_floater_with_rate(ReadStream &stream, double &value) -> void {
        value = static_cast<double>(stream.template numeric<Type>() / rate);
    }

    template <auto rate, typename Type, is_write_stream WriteStream> requires std::is_same_v<type_of<rate>, f32> && std::is_arithmetic_v<Type>
    static auto exchange_floater_with_rate(WriteStream &stream, double const &value) -> void {
        stream.template numeric<Type>(static_cast<Type>(std::round(value * rate)));
    }

    template <is_numeric_v Type, is_read_stream ReadStream>
    static auto exchange_raw_constant(ReadStream &stream, Type const&value) -> void
    {
        auto v = Type{};
        exchange_numeric_fixed<Type>(stream, v);
        assert_conditional(v == value, "exchange_raw_constant failed", "exchange_raw_constant"); //TODO
    }

    template <is_numeric_v Type, is_write_stream WriteStream>
    static auto exchange_raw_constant(WriteStream &stream, Type const&value) -> void
    {
        exchange_numeric_fixed<Type>(stream, value);
    }

    template <is_read_stream ReadStream>
    static auto exchange_null_block(ReadStream &stream, size_t const&size) -> void
    {
        stream.block(size);
    }

    template <is_write_stream WriteStream>
    static auto exchange_null_block(WriteStream &stream, size_t const&size) -> void
    {
        stream.fill(size);
    }

    template <is_numeric_v Type, is_read_stream ReadStream, typename ...Args> requires (std::is_same_v<Args, String> && ...)
    && is_greater_than_zero_v<sizeof...(Args)>
    static auto exchange_mutil_by_string_block(
        ReadStream &stream,
        const std::string_view& delimiter,
        Args & ...args
    ) -> void{
        auto string = stream.template string_numeric<Type>();
        auto list = List<String>{};
        StringHelper::split_string(string, delimiter, list);
        assert_conditional(sizeof...(args) == list.size(), "Wrong number of arguments", "exchange_mutil_by_string_block"); //TODO: add locale
        auto view = list | std::views::transform([](String const &part) { return part; });
        auto index = k_begin_index;
        ((args = view[index++]), ...);
    }

    template <is_numeric_v Type, is_write_stream WriteStream, typename ...Args> requires (std::is_same_v<Args, String> && ...)
    && is_greater_than_zero_v<sizeof...(Args)>
    static auto exchange_mutil_by_string_block(WriteStream &stream, const std::string_view delimiter, Args const & ...args) -> void {
        constexpr auto N = sizeof...(Args);
        auto delimiter_string = String{delimiter};
        auto string = String{};
        auto index = k_begin_index;
        ((string += index++ < N - 1 ? (args + delimiter_string) : args), ...);
        stream.template string_numeric<Type>(string);
    }

}