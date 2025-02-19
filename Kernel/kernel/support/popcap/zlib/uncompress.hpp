#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/zlib/common.hpp"

namespace Sen::Kernel::Support::PopCap::Zlib
{
    struct Uncompress : Common
    {
        template <auto use_variant> requires std::is_same_v<type_of<use_variant>, bool>
        static auto process_whole(ReadMemoryStream &stream, Uint8Array &value) -> void
        {
            assert_conditional(stream.u32() == k_magic_identifier, fmt::format("{}: 0x{:X}", Language::get("popcap.zlib.uncompress.mismatch_zlib_magic"), k_magic_identifier), "process_whole");
            const auto uncompress_size = stream.u32();
            value.allocate(static_cast<usize>(uncompress_size));
            if constexpr (use_variant) {
                stream += sizeof(u32) * 4_size;
            }
            auto byte = stream.bytes(stream.size() - stream.current_position());
            Compression::Zlib::Uncompress::process(byte, value);
        }

        static auto process_fs(
            StringView const &source,
            StringView const &destination,
            bool const &use_variant
        ) -> void
        {
            auto stream = ReadMemoryStream{source};
            auto value = Uint8Array{};
            if (use_variant) {
                process_whole<true>(stream, value);
            } else {
                process_whole<false>(stream, value);
            }
            FileSystem::write_file(destination, value);
        }
    };
}
