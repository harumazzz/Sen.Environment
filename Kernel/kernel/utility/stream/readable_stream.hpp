#pragma once

#include "kernel/utility/stream/base_stream.hpp"

namespace Sen::Kernel {

    template <typename Derived>
    struct ReadableStream {

    protected:

        usize m_position{};

    public:

        constexpr explicit ReadableStream(
        ) : m_position{0_size} {

        }

        virtual ~ReadableStream(
        ) = default;

        ReadableStream(
            const ReadableStream&
        ) = delete;

        auto operator=(
            const ReadableStream&
        ) -> ReadableStream& = delete;

        ReadableStream(
            ReadableStream&&
        ) = delete;

        auto operator=(
            ReadableStream&&
        ) -> ReadableStream& = delete;

        auto u8() -> uint8_t {
            return thiz.template read<uint8_t>();
        }

        auto i8() -> int8_t {
            return thiz.template read<int8_t>();
        }

        auto u16() -> uint16_t {
            return thiz.template read<uint16_t>();
        }

        auto i16() -> int16_t {
            return thiz.template read<int16_t>();
        }

        auto u24() -> uint32_t {
            return thiz.template read<uint32_t>(3_size);
        }

        auto i24() -> int32_t {
            return thiz.template read<int32_t>(3_size);
        }

        auto u32() -> uint32_t {
            return thiz.template read<uint32_t>();
        }

        auto i32() -> int32_t {
            return thiz.template read<int32_t>();
        }

        auto u64() -> uint64_t {
            return thiz.template read<uint64_t>();
        }

        auto i64() -> int64_t {
            return thiz.template read<int64_t>();
        }

        auto f32() -> float {
            return thiz.template read<float>();
        }

        auto f64() -> double {
            return thiz.template read<double>();
        }

        auto v32() -> uint32_t {
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

        auto v64() -> uint64_t {
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

        auto z32() -> int32_t {
            auto value = thiz.v32();
            return static_cast<int32_t>(value >> 1_size) ^ -static_cast<int32_t>(value & 1_ui);
        }

        auto z64() -> int64_t {
            auto value = thiz.v64();
            return static_cast<int64_t>(value >> 1_size) ^ -static_cast<int64_t>(value & 1_ul);
        }

        auto boolean(

        ) -> bool {
            return thiz.u8() == 1_byte;
        }

        virtual auto string(
            const usize& from,
            const usize& to
        ) -> String {
            throw UnsupportedOperationException{"Not implemented", std::source_location::current(), "string"};
        }

        virtual auto string(const usize& size) -> String {
            return thiz.string(m_position, m_position + size);
        }

        template<typename T> requires is_numeric_v<T>
        auto string_of() -> String {
            return string(static_cast<usize>(static_cast<Derived*>(this)->template read_impl<T>()));
        }

        auto string_v32() -> String {
            return string(static_cast<usize>(v32()));
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

        auto bytes(const usize& size) -> Uint8Array {
            return read(m_position, m_position + size);
        }

        auto sub(
            const usize&offset, 
            const usize& size
        ) -> Uint8Array {
            auto current_position = m_position;
            auto value = read(offset, offset + size);
            thiz.m_position = current_position;
            return value;
        }

        template<typename T> requires is_numeric_v<T>
        auto read() -> T {
            return static_cast<Derived*>(this)->template read_impl<T>();
        }

        template<typename T> requires is_numeric_v<T>
        auto read(const usize& size) -> T {
            return static_cast<Derived*>(this)->template read_impl<T>(size);
        }

        virtual auto read(const usize& from, const usize& to) -> Uint8Array {
            throw UnsupportedOperationException{"Not implemented", std::source_location::current(), "read"};
        }

        constexpr auto current_position() const -> usize {
            return thiz.m_position;
        }

    };

}