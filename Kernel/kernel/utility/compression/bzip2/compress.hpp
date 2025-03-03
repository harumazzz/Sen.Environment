#pragma once

#include "kernel/utility/compression/bzip2/common.hpp"
#include "kernel/utility/container/array/byte_array.hpp"
#include "kernel/utility/stream/read_memory_stream.hpp"
#include "kernel/utility/stream/write_memory_stream.hpp"
#include "kernel/utility/trait/trait.hpp"

namespace Sen::Kernel::Compression::bzip2 {

    /// Usually use Block Size = 9 and Work Factor = 0

    template <auto block_size, auto work_factor> requires std::is_integral_v<type_of<block_size>> && std::is_integral_v<type_of<work_factor>> &&
        is_between_v<block_size, 1, 9> && is_between_v<work_factor, 0, 250>
    struct Compress : Common {

    protected:

        using Common = Common;

        inline static auto constexpr BlocKSize = block_size;

        inline static auto constexpr WorkFactor = work_factor;


    public:

        constexpr explicit Compress(
        ) = default;

        constexpr ~Compress(
        ) = default;

        Compress(
            const Compress& other
        ) = delete;

        Compress(
            Compress&& other
        ) = delete;

        auto operator =(
            Compress&& other
        ) -> Compress& = delete;

        auto operator =(
            const Compress& other
        ) -> Compress& = delete;

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
            state = Subprojects::bzip2::BZ2_bzCompressInit(
                &bz_stream,
                static_cast<int>(BlocKSize),
                0,
                static_cast<int>(WorkFactor)
            );
            assert_conditional(state == Subprojects::bzip2::$BZ_OK, "Failed to initialize bzip2", "process_whole");
            state = Subprojects::bzip2::BZ2_bzCompress(
                &bz_stream,
                Subprojects::bzip2::$BZ_FINISH
            );
            assert_conditional(state == Subprojects::bzip2::$BZ_STREAM_END, "Failed to finish bzip2", "process_whole");
            assert_conditional(bz_stream.avail_in == 0, "Failed to finish bzip2", "process_whole");
            destination.resize((static_cast<usize>(bz_stream.total_out_hi32) << 32) + static_cast<usize>(bz_stream.total_out_lo32));
        }

        static auto process (
            Uint8Array& source,
            Uint8List& destination
        ) -> void {
            destination.allocate(source.size() + EXTRA_ALLOCATION);
            auto raw = ReadMemoryStream{source};
            auto ripe = WriteMemoryStream{destination};
            process_whole(raw, ripe);
            raw.release_stream(source);
            ripe.release_stream(destination);
        }

    };

}
