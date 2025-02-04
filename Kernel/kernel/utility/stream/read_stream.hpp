#pragma once

#include "kernel/utility/algorithm/pointer.hpp"
#include "kernel/utility/stream/base_stream.hpp"

namespace Sen::Kernel {

    class ReadStream : public ReadableStream {

    protected:

        Uint8Array m_data{};

        usize m_position{};

    public:

        explicit ReadStream(
            Uint8Array &source
        ) : m_data{}, m_position{0_size} {
            m_data.assign(source);
        }

        ~ReadStream(

        ) = default;

        ReadStream(
                const ReadStream &other
        ) = delete;

        auto operator=(
                const ReadStream &other
        ) -> ReadStream & = delete;

        ReadStream(
                ReadStream &&other
        ) = delete;

        auto operator=(
                ReadStream &&
        ) -> ReadStream & = delete;

        auto u8(

        ) -> uint8_t {
            return thiz.read<uint8_t>();
        }

        auto i8(

        ) -> int8_t {
            return thiz.read<int8_t>();
        }

        auto u16(

        ) -> uint16_t {
            return thiz.read<uint16_t>();
        }

        auto i16(

        ) -> int16_t {
            return thiz.read<int16_t>();
        }

        auto u24(

        ) -> uint32_t {
            return thiz.read<uint32_t>(3_size);
        }

        auto i24(

        ) -> int32_t {
            return thiz.read<int32_t>(3_size);
        }

        auto u32(

        ) -> uint32_t {
            return thiz.read<uint32_t>();
        }

        auto i32(

        ) -> int32_t {
            return thiz.read<int32_t>();
        }

        auto u64(

        ) -> uint64_t {
            return thiz.read<uint64_t>();
        }

        auto i64(

        ) -> int64_t {
            return thiz.read<int64_t>();
        }

        //integer_variable_length_unsigned_32
        auto v32(

        ) -> uint32_t {
            auto value = 0_ui;
            for (auto shift_count: Range<size_t>{0, 32, 7}) {
                auto byte = thiz.u8();
                if ((byte & 128_byte) != 0_byte) {
                    value |= static_cast<uint32_t>(byte & 127) << shift_count;
                } else {
                    value |= static_cast<uint32_t>(byte) << shift_count;
                    break;
                }
            }
            return value;
        }

        //integer_variable_length_unsigned_64
        auto v64(

        ) -> uint64_t {
            auto value = 0_ul;
            for (auto shift_count: Range<size_t>{0, 64, 7}) {
                auto byte = thiz.u8();
                if ((byte & 128_byte) != 0_byte) {
                    value |= static_cast<uint64_t>(byte & 127) << shift_count;
                } else {
                    value |= static_cast<uint64_t>(byte) << shift_count;
                    break;
                }
            }
            return value;
        }

        //zigzag_32
        auto z32(

        ) -> int32_t {
            auto value = thiz.v32();
            return static_cast<int32_t>(value >> 1_size) ^ -static_cast<int32_t>(value & 1_ui);
        }

        //zigzag_64
        auto z64(

        ) -> int64_t {
            auto value = thiz.v64();
            return static_cast<int64_t>(value >> 1_size) ^ -static_cast<int64_t>(value & 1_ul);
        }

        auto boolean(

        ) -> bool {
            return thiz.u8() == 1_byte;
        }

         auto string(
            const usize& from,
            const usize &to
        ) -> String {
            assert_conditional(from <= thiz.m_data.size(), "From index must be smaller than Stream size", "string");
            assert_conditional(to <= thiz.m_data.size(), "To index must be smaller than Stream size", "string");
            assert_conditional(from < to, "From index must be smaller than To index", "string");
            auto destination = String{to - from};
            std::memcpy(destination.data(), thiz.m_data.begin() + from, to - from);
            return destination;
        }

        auto string(
            const usize &size
        ) -> String {
            return thiz.string(thiz.m_position, thiz.m_position + size);
        }

        template<typename T> requires is_numeric_v<T>
        auto string_of(

        ) -> String {
            return thiz.string(static_cast<usize>(thiz.read<T>()));
        }

        auto string_v32(

        ) -> String {
            return thiz.string(static_cast<usize>(thiz.v32()));
        }

        auto string_null_terminator(

        ) -> String {
            auto value = String{};
            while (true) {
                auto byte = thiz.i8();
                if (byte == 0) {
                    break;
                }
                value += byte;
            }
            return value;
        }

        auto bytes(
            const usize &size
        )  -> Uint8Array  {
            return thiz.read(thiz.m_position, thiz.m_position + size);
        }

        template<typename T> requires is_numeric_v<T>
        auto read(
            const usize &size
        ) -> T {
            assert_conditional(thiz.m_position + sizeof(T) <= thiz.m_data.size(), "Outside bounds of ReadStream", "read");
            auto value = T{};
            std::memcpy(&value, thiz.m_data.begin() + thiz.m_position, size);
            thiz.m_position += size;
            return value;
        }

        template<typename T> requires is_numeric_v<T>
        auto read(

        ) -> T {
            assert_conditional(thiz.m_position + sizeof(T) <= thiz.m_data.size(), "Outside bounds of ReadStream","read");
            auto value = T{};
            std::memcpy(&value, thiz.m_data.begin() + thiz.m_position, sizeof(T));
            thiz.m_position += sizeof(T);
            return value;
        }

        auto read(
            const usize &from,
            const usize &to
        ) -> Uint8Array {
            assert_conditional(from <= thiz.m_data.size(), "From index must be smaller than Stream size", "read");
            assert_conditional(to <= thiz.m_data.size(), "To index must be smaller than Stream size", "read");
            assert_conditional(from < to, "From index must be smaller than To index", "read");
            auto destination = Uint8Array{to - from};
            std::memcpy(destination.data(), thiz.m_data.begin() + from, to - from);
            return destination;
        }

        constexpr auto current_position(

        ) const -> usize {
            return thiz.m_position;
        }

        constexpr auto size(

        ) const -> usize {
            return thiz.m_data.size();
        }

        constexpr auto current_iterator(

        ) -> decltype(thiz.m_data.begin() + thiz.m_position) {
            return thiz.m_data.begin() + thiz.m_position;
        }

        constexpr auto begin() -> decltype(thiz.m_data.begin()) {
            return thiz.m_data.begin();
        }

        constexpr auto end() -> decltype(thiz.m_data.end()) {
            return thiz.m_data.end();
        }

        auto view(

        ) -> Uint8Array & {
            return thiz.m_data;
        }

        constexpr auto set_position(
            const usize &index
        ) -> void {
            assert_conditional(index < thiz.m_data.size(), "Index must be smaller than data size", "set_position");
            thiz.m_position = index;
        }

        auto operator + (
            const usize& index
        ) -> ReadStream& {
			assert_conditional(index <= thiz.m_data.size(), "Index must be smaller than data size", "operator_plus");
            thiz.m_position += index;
            return thiz;
        }

        auto operator -(
            const usize& index
        ) -> ReadStream& {
            assert_conditional(index <= thiz.m_data.size(), "Index must be smaller than data size", "operator_minus");
            thiz.m_position -= index;
            return thiz;
        }

        auto has_space(
            const usize& index
        ) const -> bool {
            return thiz.m_position + index <= thiz.m_data.size();
        }

    };

}