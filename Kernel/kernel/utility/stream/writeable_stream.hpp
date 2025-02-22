#pragma once

#include "kernel/utility/stream/base_stream.hpp"

namespace Sen::Kernel {

    template <typename Derived>
    struct WritableStream {

    protected:

        usize m_position{};

    public:

        constexpr explicit WritableStream(
        ) : m_position{0_size} {

        }

        virtual ~WritableStream(
        ) = default;

        WritableStream(
            WritableStream&& other
        ) = delete;

        auto operator=(
            WritableStream&& other
        ) -> WritableStream& = delete;

        WritableStream(
            const WritableStream& other
        ) = delete;

        auto operator=(
            const WritableStream& other
        ) -> WritableStream& = delete;

        constexpr auto current_position() const -> usize {
            return m_position;
        }

        template <typename T>
        auto write(const T& value) -> void {
            static_cast<Derived*>(this)->template write_impl<T>(value);
        }

        auto u8(const u8& value) -> void {
            thiz.template write<uint8_t>(value);
        }

        auto i8(const i8& value) -> void {
            thiz.template write<int8_t>(value);
        }

        auto u16(const u16& value) -> void {
            thiz.template write<uint16_t>(value);
        }

        auto i16(const i16& value) -> void {
            thiz.template write<int16_t>(value);
        }

        auto u32(const u32& value) -> void {
            thiz.template write<uint32_t>(value);
        }

        auto i32(const i32& value) -> void {
            thiz.template write<int32_t>(value);
        }

        auto u64(const u64& value) -> void {
            thiz.template write<uint64_t>(value);
        }

        auto i64(const i64& value) -> void {
            thiz.template write<int64_t>(value);
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

        auto bytes(Uint8Array & value) -> void {
            static_cast<Derived*>(this)-> raw(value.data(), value.size());
        }

        auto string_v32(const String &value) -> void {
            thiz.v32(value.size());
            thiz.string(value);
        }

        template<typename T>
        auto string_of(const String &value) -> void {
            thiz.template write<T>(static_cast<T>(value.size()));
            thiz.string(value);

        }

        auto string_null_terminator(
            const String &value
        ) -> void {
            thiz.string(value);
            thiz.u8(0_byte);
        }

        auto null(const usize &size) {
            static_cast<Derived*>(this)->allocate_size(size);
        }

        auto f32(const float& value) -> void {
            thiz.template write<float>(value);
        }

        auto f64(const double& value) -> void {
            thiz.template write<double>(value);
        }

        auto boolean(
            const bool data
        ) -> void {
            thiz.u8(data ? 0x01_u8 : 0x00_u8);
        }

        auto string(
            const std::string_view &source
        ) -> void {
            if (!source.empty())
            {
                static_cast<Derived*>(this)->raw(reinterpret_cast<const uint8_t *>(source.data()), source.size());
            }
        }

        auto string(
            const String &value
        ) -> void {
            if (!value.empty())
            {
                static_cast<Derived*>(this)->raw(reinterpret_cast<const uint8_t *>(value.begin()), value.size());
            }
        }

        auto string(
            const StringView &value
        ) -> void {
            if (!value.empty())
            {
                static_cast<Derived*>(this)->raw(reinterpret_cast<const uint8_t *>(value.begin()), value.size());
            }
        }

    };

}