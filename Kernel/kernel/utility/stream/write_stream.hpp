#pragma once

#include "kernel/utility/stream/base_stream.hpp"


namespace Sen::Kernel {

    class WriteStream : public WritableStream {

    protected:

        Uint8List m_data{};

        usize m_position{0};

    public:

        template<typename T>
        requires std::is_same_v<T, Uint8Array> || std::is_same_v<T, Uint8List>
        explicit WriteStream(T &source) : m_data{}, m_position{0} {
            m_data.assign(source);
        }

        constexpr explicit WriteStream(
        ) = default;

        ~WriteStream(

        ) = default;

        WriteStream(
                const WriteStream &other
        ) = delete;

        auto operator=(
                const WriteStream &other
        ) -> WriteStream & = delete;

        WriteStream(
                WriteStream &&other
        ) = delete;

        auto operator=(
                WriteStream &&
        ) -> WriteStream & = delete;

        constexpr auto current_position(

        ) const -> usize {
            return thiz.m_position;
        }

        constexpr auto size(

        ) -> usize {
            return thiz.m_data.size();
        }

        constexpr auto current_iterator(

        ) -> decltype(thiz.m_data.begin() + thiz.m_position) {
            return thiz.m_data.begin() + thiz.m_position;
        }

        constexpr auto begin(

        ) -> decltype(thiz.m_data.begin()) {
            return thiz.m_data.begin();
        }

        constexpr auto end(

        ) -> decltype(thiz.m_data.end()) {
            return thiz.m_data.end();
        }

        auto view(

        ) -> Uint8List & {
            return thiz.m_data;
        }

        constexpr auto set_position(
            const usize &index
        ) -> void {
            assert_conditional(index <= thiz.m_data.capacity(), "Index must be smaller than capacity", "set_position");
            thiz.m_position = index;
        }

        auto allocate_full(
            const usize &new_size
        ) -> void {
            if (new_size > thiz.m_data.capacity()) {
                thiz.m_data.reallocate(new_size);
            }
        }

        auto u8(const u8 &value) -> void {
            thiz.write<uint8_t>(value);
        }

        auto i8(const i8 &value) -> void {
            thiz.write<int8_t >(value);
        }

        auto u16(const u16 &value) -> void {
            thiz.write<uint16_t>(value);
        }

        auto i16(const i16 &value) -> void {
            thiz.write<int16_t>(value);
        }

        auto u24(const u32 &value) -> void {
            for (auto i : Range(3)) {
                thiz.u8(static_cast<uint8_t>((value >> (i * 8)) & 0xff));
            }
        }

        auto i24(const i32 &value) -> void {
            for (auto i : Range(3)) {
                thiz.i8(static_cast<int8_t>((value >> (i * 8)) & 0xff));
            }
        }

        auto u32(const u32 &value) -> void {
            thiz.write<uint32_t>(value);
        }

        auto i32(const i32 &value) -> void {
            thiz.write<int32_t>(value);
        }

        auto u64(const u64 &value) -> void {
            thiz.write<uint64_t>(value);
        }

        auto i64(const i64 &value) -> void {
            thiz.write<int64_t>(value);
        }
        
        auto v32(const uint32_t& value) -> void {
            auto temporary = value;
            while (temporary >= 128_ui) {
                thiz.u8(static_cast<uint8_t>(temporary) & 127_byte | 128_byte);
                temporary >>= 7_ui;
            }
            thiz.u8(static_cast<uint8_t>(temporary));
        }

        auto v64(const uint64_t& value) -> void {
            auto temporary = value;
            while (temporary >= 128_ul) {
                thiz.u8(static_cast<uint8_t>(temporary) & 127_byte | 128_byte);
                temporary >>= 7_size;
            }
            thiz.u8(static_cast<uint8_t>(temporary));
        }

        auto z32(const int32_t &value) -> void {
            thiz.v32(static_cast<uint32_t>(value << 1_size ^ value >> 31_size));
        }

        auto z64(const int64_t &value) -> void {
            thiz.v64(static_cast<uint64_t>(value << 1_size ^ value >> 63_size));
        }

        auto bytes(Uint8List & value) -> void {
            thiz.write<Uint8List>(value);
        }

        auto string(String &value) -> void {
            auto temporary = value.size() + thiz.m_position;
            thiz.allocate_full(temporary);
            thiz.m_data.size(thiz.m_data.size() + value.size());
            std::memcpy(&thiz.m_data[thiz.m_position], value.begin(), value.size());
            thiz.set_position(temporary);
        }

        template<typename T>
        auto string_of(String &value) -> void {
            thiz.write<T>(static_cast<T>(value.size()));
            thiz.string(value);
        }

        auto string_null_terminator(
            String &value
        ) -> void {
            thiz.string(value);
            thiz.u8(0_byte);
        }

        auto raw(
            uint8_t* data,
            const usize &size
        ) -> void {
            auto temporary = size + thiz.m_position;
            thiz.allocate_full(temporary);
            thiz.m_data.size(thiz.m_data.size() + size);
            std::memcpy(&thiz.m_data[thiz.m_position], data, size);
            thiz.set_position(temporary);
        }

        auto null(const usize &size) {
            auto value = Uint8List{size};
            thiz.bytes(value);
        }

        template<typename T>
        requires std::is_arithmetic_v<T>
        auto write(
            const T &value
        ) -> void {
            constexpr auto sz = sizeof(T);
            auto temporary = thiz.m_position + sz;
            thiz.allocate_full(temporary);
            thiz.m_data.size(thiz.m_data.size() + sz);
            std::memcpy(&thiz.m_data[thiz.m_position], &value, sz);
            thiz.set_position(temporary);
        }

        template<typename T> requires (std::is_same_v<T, Uint8Array> or std::is_same_v<T, Uint8List>) && requires(T a) {
            { a.size() } -> std::convertible_to<usize>;
            { a.begin() } -> std::convertible_to<uint8_t*>;
        }
        auto write(
            T &value
        ) -> void {
            auto temporary = value.size() + thiz.m_position;
            thiz.allocate_full(temporary);
            thiz.m_data.size(thiz.m_data.size() + value.size());
            std::memcpy(&thiz.m_data[thiz.m_position], value.begin(), value.size());
            thiz.set_position(temporary);
        }
    };

}