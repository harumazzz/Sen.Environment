#pragma once

#include "kernel/utility/compression/bzip2/common.hpp"
#include "kernel/utility/container/array/byte_array.hpp"
#include "kernel/utility/stream/read_memory_stream.hpp"
#include "kernel/utility/stream/write_memory_stream.hpp"
#include "kernel/utility/trait/trait.hpp"

namespace Sen::Kernel::Compression::bzip2 {

    /// Usually not small : false

    template <auto small> requires std::is_same_v<type_of<small>, bool>
    struct Uncompress : Common {

    protected:

        using Common = Common;

        inline static constexpr auto Small = small;

    public:

        constexpr explicit Uncompress(
        ) = default;

        constexpr ~Uncompress(
        ) = default;

        Uncompress(
            const Uncompress& other
        ) = delete;

        Uncompress(
            Uncompress&& other
        ) = delete;

        auto operator =(
            Uncompress&& other
        ) -> Uncompress& = delete;

        auto operator =(
            const Uncompress& other
        ) -> Uncompress& = delete;

        static auto process_whole (
            ReadMemoryStream& source,
            WriteMemoryStream& destination
        ) -> void {
            auto bz_stream = Subprojects::bzip2::bz_stream{
                .next_in = reinterpret_cast<char *>(source.begin()),
                .avail_in = static_cast<unsigned int>(source.size()),
                .total_in_lo32 = 0,
                .total_in_hi32 = 0,
                .next_out = reinterpret_cast<char *>(destination.current_iterator()),
                .avail_out = static_cast<unsigned int>(destination.capacity()),
                .total_out_lo32 = 0,
                .total_out_hi32 = 0,
                .state = nullptr,
                .bzalloc = nullptr,
                .bzfree = nullptr,
                .opaque = nullptr,
            };
            auto state = int{};
            state = Subprojects::bzip2::BZ2_bzDecompressInit(
                &bz_stream,
                0,
                static_cast<int>(small)
            );
            assert_conditional(state == Subprojects::bzip2::$BZ_OK, "Failed to initialize bzip2", "process_whole");
            state = Subprojects::bzip2::BZ2_bzDecompress(
                &bz_stream
            );
            assert_conditional(state == Subprojects::bzip2::$BZ_STREAM_END, "Failed to decompress", "process_whole");
            state = Subprojects::bzip2::BZ2_bzDecompressEnd(
                &bz_stream
            );
            assert_conditional(state == Subprojects::bzip2::$BZ_OK, "Failed to decompress", "process_whole");
            destination.resize((static_cast<std::uint64_t>(bz_stream.total_in_hi32) << 32) + static_cast<std::uint64_t>(bz_stream.total_in_lo32));
        }

        static auto process (
            Uint8Array& source,
            Uint8List& destination
        ) -> void {
            destination.allocate(source.size() * 4_size);
            auto raw = ReadMemoryStream{source};
            auto ripe = WriteMemoryStream{destination};
            process_whole(raw, ripe);
            raw.release_stream(source);
            ripe.release_stream(destination);
        }

    };

}
