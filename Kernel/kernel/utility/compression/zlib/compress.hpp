#pragma once

#include "kernel/utility/compression/zlib/common.hpp"
#include "kernel/utility/stream/read_memory_stream.hpp"
#include "kernel/utility/stream/write_memory_stream.hpp"

namespace Sen::Kernel::Compression::Zlib {


    struct Compress : Common {

        template <auto level = 9, auto type, auto windows_bits, auto memory_level, auto strategy> requires is_valid_compress<type, level, windows_bits, memory_level, strategy>
        static auto process_whole (
            Uint8Array& source,
            Uint8List& destination
        ) -> void {
            #pragma clang diagnostic push
            #pragma clang diagnostic ignored "-Wold-style-cast"
            auto actual_window_bits = windows_bits;
            if constexpr (type == Type::deflate) {
                actual_window_bits = -actual_window_bits;
            } else if constexpr (type == Type::zlib) {
              actual_window_bits = actual_window_bits;
            } else {
                actual_window_bits += 16_size;
            }
            const auto allocate_size = Subprojects::zlib::compressBound(source.size());
            destination.allocate(allocate_size);
            auto z_stream = Subprojects::zlib::z_stream{
                .next_in = reinterpret_cast<Subprojects::zlib::Bytef*>(source.begin()),
                .avail_in = static_cast<Subprojects::zlib::uInt>(source.size()),
                .total_in = 0,
                .next_out = reinterpret_cast<Subprojects::zlib::Bytef*>(destination.begin()),
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
                static_cast<int>(level),
                Subprojects::zlib::$Z_DEFLATED,
                actual_window_bits,
                static_cast<int>(memory_level),
                static_cast<int>(strategy),
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
            destination.resize(z_stream.total_out);
            #pragma clang diagnostic pop
        }

        template <auto level = 9, auto type = Type::zlib, auto windows_bits = Subprojects::zlib::$MAX_WBITS, auto memory_level = Subprojects::zlib::$MAX_MEM_LEVEL, auto strategy = Strategy::default_mode> requires is_valid_compress<type, level, windows_bits, memory_level, strategy>
        static auto process (
            Uint8Array& source,
            Uint8Array& destination
        ) -> void {
            auto buffer = Uint8List{};
            process_whole<level, type, windows_bits, memory_level, strategy>(source, buffer);
            destination.assign(buffer);
        }

    };

}
