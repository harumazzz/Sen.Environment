#pragma once

#include "kernel/utility/compression/lzma/common.hpp"
#include "kernel/utility/container/list/byte_list.hpp"
#include "kernel/utility/stream/read_memory_stream.hpp"
#include "kernel/utility/stream/write_memory_stream.hpp"

namespace Sen::Kernel::Compression::Lzma {

    struct Uncompress : Common {

        protected:

            using Common = Common;

        public:

            constexpr Uncompress (
            ) = default;

            constexpr ~Uncompress (
            ) = default;

            Uncompress (
                Uncompress&& other
            ) = delete;

            Uncompress (
                const Uncompress& other
            ) = delete;

            auto operator =(
                const Uncompress& other
            ) -> Uncompress& = delete;

            auto operator =(
                Uncompress&& other
            ) -> Uncompress& = delete;

            static auto process_whole (
                ReadMemoryStream& source,
                WriteMemoryStream& destination
            ) -> void {
                auto destination_size = destination.capacity();
                auto prop_size = static_cast<usize>(Subprojects::lzma::$LZMA_PROPS_SIZE);
                auto props = source.bytes(prop_size);
                auto destination_length = static_cast<usize>(source.u64());
                destination.allocate_full(destination_length);
                auto source_length = source.size() - source.current_position();
                auto state = Subprojects::lzma::LzmaUncompress(
                    destination.begin(),
                    &destination_length,
                    source.begin(),
                    &source_length,
                    props.begin(),
                    prop_size
                );
                assert_conditional(state == Subprojects::lzma::$SZ_OK, fmt::format("{}", Kernel::Language::get("lzma.uncompress.failed")), "uncompress");
                destination.resize(destination_size);
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
