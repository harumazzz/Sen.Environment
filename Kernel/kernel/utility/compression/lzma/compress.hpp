#pragma once

#include "kernel/utility/compression/lzma/common.hpp"
#include "kernel/utility/stream/read_memory_stream.hpp"
#include "kernel/utility/stream/write_memory_stream.hpp"

namespace Sen::Kernel::Compression::Lzma {

    template <auto level> requires std::is_same_v<type_of<level>, Level>
    struct Compress : Common {

    protected:

        using Common = Common;

        inline static auto constexpr Level = level;

    public:

        constexpr Compress (
        ) = default;

        constexpr ~Compress (
        ) = default;

        Compress (
            Compress&& other
        ) = delete;

        Compress (
            const Compress& other
        ) = delete;

        auto operator =(
            const Compress& other
        ) -> Compress& = delete;

        auto operator =(
            Compress&& other
        ) -> Compress& = delete;

        static auto process_whole (
            ReadMemoryStream& source,
            WriteMemoryStream& destination
        ) -> void {
            auto destination_size = destination.capacity();
            auto prop_size = static_cast<size_t>(Subprojects::lzma::$LZMA_PROPS_SIZE);
            auto props = std::array<u8, 5>{0x5D, 0x00, 0x00, 0x00, 0x04};
            for (auto prop : props) {
                destination.u8(prop);
            }
            destination.u64(static_cast<u64>(source.size()));
            auto state = Subprojects::lzma::LzmaCompress(
                destination.begin(),
                &destination_size,
                source.begin(),
                source.size(),
                &props.front(),
                &prop_size,
                static_cast<int>(Level),
                0,
                -1,
                -1,
                -1,
                -1,
                -1
            );
            assert_conditional(state == Subprojects::lzma::$SZ_OK, fmt::format("{}", Kernel::Language::get("lzma.compress.failed")), "compress");
            destination.resize(destination_size);
        }

        static auto process (
            Uint8Array& source,
            Uint8List& destination
        ) -> void {
            destination.allocate(compute_destination_size(source.size()));
            auto raw = ReadMemoryStream{source};
            auto ripe = WriteMemoryStream{destination};
            process_whole(raw, ripe);
            raw.release_stream(source);
            ripe.release_stream(destination);
        }


    };

}
