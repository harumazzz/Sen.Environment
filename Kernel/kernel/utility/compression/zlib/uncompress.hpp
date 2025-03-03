#pragma once

#include "kernel/utility/compression/zlib/common.hpp"
#include "kernel/utility/stream/read_memory_stream.hpp"
#include "kernel/utility/stream/write_memory_stream.hpp"

namespace Sen::Kernel::Compression::Zlib {

    struct Uncompress : Common {

        template <auto format, auto windows_bits, typename Source, typename Destination>
        requires is_valid_uncompress<format, windows_bits> && FileSystem::Detail::is_buffer_container_v<Source> && FileSystem::Detail::is_buffer_container_v<Destination>
        static auto process_whole (
            const Source& source,
            Destination& destination
        ) -> void {
            #pragma clang diagnostic push
            #pragma clang diagnostic ignored "-Wold-style-cast"
            auto actual_window_bits = windows_bits;
            if constexpr (format == Type::deflate) {
                actual_window_bits = -actual_window_bits;
            } else if constexpr (format == Type::zlib) {
                actual_window_bits = actual_window_bits;
            } else {
                actual_window_bits += 16_size;
            }
            auto avail_out = k_none_size;
            if constexpr (is_array_v<Destination>) {
                avail_out = destination.size();
            }
            else {
                destination.allocate(source.size() * 8_size);
                avail_out = destination.capacity();
            }
            const auto source_size = source.size() * source.sizeof_value();
            debug(source_size);
            debug(avail_out);
            auto z_stream = Subprojects::zlib::z_stream{
                .next_in = reinterpret_cast<Subprojects::zlib::Bytef*>(source.begin()),
                .avail_in = static_cast<Subprojects::zlib::uInt>(source_size),
                .total_in = 0,
                .next_out = reinterpret_cast<Subprojects::zlib::Bytef*>(destination.begin()),
                .avail_out = static_cast<Subprojects::zlib::uInt>(avail_out),
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
            assert_conditional(state == Subprojects::zlib::$Z_STREAM_END, "Failed to uncompress zlib", "process_whole");
            state = Subprojects::zlib::inflateEnd(
                &z_stream
            );
            assert_conditional(state == Subprojects::zlib::$Z_OK, "Failed to uncompress zlib state", "process_whole");
            if constexpr (is_list_v<Destination>) {
                destination.resize(static_cast<usize>(z_stream.total_out / destination.sizeof_value()));
            }
            #pragma clang diagnostic pop
        }

        template < auto type = Type::zlib, auto windows_bits = Subprojects::zlib::$MAX_WBITS, typename Source, typename Destination>
        requires is_valid_uncompress<type, windows_bits> && FileSystem::Detail::is_buffer_container_v<Source> && FileSystem::Detail::is_buffer_container_v<Destination>
        static auto process (
            Source& source,
            Destination& destination
        ) -> void {
            process_whole<type, windows_bits>(source, destination);
        }

    };

}
