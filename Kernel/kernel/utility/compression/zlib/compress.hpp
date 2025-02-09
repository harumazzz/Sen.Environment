#pragma once

#include "kernel/utility/compression/zlib/common.hpp"
#include "kernel/utility/stream/read_memory_stream.hpp"
#include "kernel/utility/stream/write_memory_stream.hpp"

namespace Sen::Kernel::Compression::Zlib {

    template <auto type, auto level, auto windows_bits, auto memory_level, auto strategy> requires std::is_same_v<type_of<type>, Type> && is_numeric_v<type_of<level>> && is_numeric_v<type_of<windows_bits>> && is_numeric_v<type_of<memory_level>> &&
            is_between_v<level, 0_size, Subprojects::zlib::$Z_BEST_COMPRESSION> && is_between_v<windows_bits, 8_size, Subprojects::zlib::$MAX_WBITS> && is_between_v<memory_level, 1_size, Subprojects::zlib::$MAX_MEM_LEVEL>
    && (!(windows_bits == 8_size && type == Type::zlib)) && std::is_same_v<type_of<strategy>, Strategy>
    struct Compress : Common {

    protected:

        using Common = Common;

        inline static constexpr auto Format = type;

        inline static constexpr auto Level = level;

        inline static constexpr auto WindowsBits = static_cast<int>(windows_bits);

        inline static constexpr auto MemoryLevel = memory_level;

        inline static constexpr auto Strategy = strategy;

    public:

        constexpr Compress (
        ) = default;

        constexpr ~Compress (
        ) = default;

        Compress (
            const Compress& other
        ) = delete;

        Compress (
            Compress&& other
        ) = delete;

        auto operator = (
            const Compress& other
        ) -> Compress& = delete;

        auto operator = (
            Compress&& other
        ) -> Compress& = delete;

        static auto process_whole (
            ReadMemoryStream& source,
            WriteMemoryStream& destination
        ) -> void {
            #pragma clang diagnostic push
            #pragma clang diagnostic ignored "-Wold-style-cast"
            auto actual_window_bits = WindowsBits;
            if constexpr (Format == Type::deflate) {
                actual_window_bits = -actual_window_bits;
            } else if constexpr (Format == Type::zlib) {
                actual_window_bits = actual_window_bits;
            } else {
                actual_window_bits += 16_size;
            }
            auto z_stream = Subprojects::zlib::z_stream{
                .next_in = reinterpret_cast<Subprojects::zlib::Bytef*>(source.begin()),
                .avail_in = static_cast<Subprojects::zlib::uInt>(source.size()),
                .total_in = 0,
                .next_out = reinterpret_cast<Subprojects::zlib::Bytef*>(destination.current_iterator()),
                .avail_out = static_cast<Subprojects::zlib::uInt>(destination.capacity()),
                .total_out = 0,
                .msg = nullptr,
                .state = nullptr,
                .zalloc = nullptr,
                .zfree = nullptr,
                .opaque = nullptr,
                .data_type = 0,
                .adler = 0,
                .reserved = 0,
            };
            auto state = int{};
            state = Subprojects::zlib::deflateInit2_(
                &z_stream,
                static_cast<int>(Level),
                Subprojects::zlib::$Z_DEFLATED,
                actual_window_bits,
                static_cast<int>(MemoryLevel),
                static_cast<int>(Strategy),
                Subprojects::zlib::$ZLIB_VERSION,
                static_cast<int>(sizeof(z_stream))
            );
            assert_conditional(state == Subprojects::zlib::$Z_OK, "Failed to initialize zlib", "process_whole");
            state = Subprojects::zlib::deflate(
                &z_stream,
                Subprojects::zlib::$Z_NO_FLUSH
            );
            assert_conditional(state == Subprojects::zlib::$Z_OK, "Failed to compress zlib", "process_whole");
            state = Subprojects::zlib::deflate(
                &z_stream,
                Subprojects::zlib::$Z_FINISH
            );
            assert_conditional(state == Subprojects::zlib::$Z_STREAM_END, "Failed to compress zlib", "process_whole");
            state = Subprojects::zlib::deflateEnd(
                &z_stream
            );
            assert_conditional(state == Subprojects::zlib::$Z_OK, "Failed to compress zlib", "process_whole");
            assert_conditional(z_stream.avail_in == 0, "Failed to compress zlib", "process_whole");
            destination.size(static_cast<usize>(z_stream.total_out));
            #pragma clang diagnostic pop
        }

        static auto estimate_size(
            const usize& raw_size
        ) -> usize
        {
            auto wrap_size = 0_size;
            if constexpr (Format == Type::deflate) {
                wrap_size = 0_size;
            }
            else if constexpr (Format == Type::zlib) {
                wrap_size = 2_size + 4_size + 4_size;
            }
            else {
                wrap_size = 10_size + 8_size;
            }
            if (WindowsBits == 15_size && MemoryLevel == 8_size) {
                return wrap_size + (raw_size + (raw_size >> 12_size) + (raw_size >> 14_size) + (raw_size >> 25_size) + 13_size - 6_size);
            }
            return wrap_size + (raw_size + ((raw_size + 7_size) >> 3_size) + ((raw_size + 63_size) >> 6_size) + 5_size);
        }

        static auto process (
            Uint8Array& source,
            Uint8List& destination
        ) -> void {
            auto raw = ReadMemoryStream{source};
            auto ripe = WriteMemoryStream{destination};
            process_whole(raw, ripe);
            raw.release_stream(source);
            ripe.release_stream(destination);
        }

    };

}
