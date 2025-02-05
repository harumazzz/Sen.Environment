#pragma once

#include "kernel/utility/compression/zlib/common.hpp"
#include "kernel/utility/stream/read_stream.hpp"
#include "kernel/utility/stream/write_stream.hpp"

namespace Sen::Kernel::Compression::Zlib {

    template <auto type, auto windows_bits> requires std::is_same_v<type_of<type>, Type> && is_numeric_v<type_of<windows_bits>> && is_between_v<windows_bits, 8_size, Subprojects::zlib::$MAX_WBITS>
    struct Uncompress : Common {

    protected:

        using Common = Common;

        inline static constexpr auto WindowsBits = windows_bits;

        inline static constexpr auto Format = type;

    public:

        constexpr Uncompress (
        ) = default;

        constexpr ~Uncompress (
        ) = default;

        Uncompress (
            const Uncompress& other
        ) = delete;

        Uncompress (
            Uncompress&& other
        ) = delete;

        auto operator = (
            const Uncompress& other
        ) -> Uncompress& = delete;

        auto operator = (
            Uncompress&& other
        ) -> Uncompress& = delete;

        static auto process_whole (
            ReadStream& source,
            WriteStream& destination
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
            state = Subprojects::zlib::inflateInit2_(
                &z_stream,
                actual_window_bits,
                Subprojects::zlib::$ZLIB_VERSION,
                static_cast<int>(sizeof(z_stream))
            );
            assert_conditional(state == Subprojects::zlib::$Z_OK, "Failed to initialize zlib", "process_whole");
            state = Subprojects::zlib::inflate(
                &z_stream,
                Subprojects::zlib::$Z_NO_FLUSH
            );
            assert_conditional(state == Subprojects::zlib::$Z_OK, "Failed to uncompress zlib", "process_whole");
            state = Subprojects::zlib::inflate(
                &z_stream,
                Subprojects::zlib::$Z_FINISH
            );
            assert_conditional(state == Subprojects::zlib::$Z_STREAM_END, "Failed to uncompress zlib", "process_whole");
            state = Subprojects::zlib::inflateEnd(
                &z_stream
            );
            assert_conditional(state == Subprojects::zlib::$Z_OK, "Failed to uncompress zlib", "process_whole");
            assert_conditional(z_stream.avail_in == 0, "Failed to uncompress zlib", "process_whole");
            destination.size(static_cast<usize>(z_stream.total_out));
            #pragma clang diagnostic pop
        }

        static auto process (
            Uint8Array& source,
            Uint8List& destination
        ) -> void {
            auto raw = ReadStream{source};
            auto ripe = WriteStream{destination};
            process_whole(raw, ripe);
            raw.release_stream(source);
            ripe.release_stream(destination);
        }
    };

}
