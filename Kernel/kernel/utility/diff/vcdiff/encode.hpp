#pragma once

#include "kernel/utility/container/array/byte_array.hpp"
#include "kernel/utility/container/list/byte_list.hpp"
#include "kernel/utility/diff/vcdiff/common.hpp"
#include "kernel/utility/stream/write_memory_stream.hpp"

namespace Sen::Kernel::Diff::VCDiff {

    template <auto interleaved> requires std::is_same_v<type_of<interleaved>, bool>
    struct Encode : Common {

    protected:

        using Common = Common;

        inline static auto constexpr Interleaved = interleaved;

    public:

        static auto process_whole (
            Uint8Array& before,
            Uint8Array& after,
            WriteMemoryStream& patch
        ) -> void {
            auto _patch = std::string{};
            auto hashed_dictionary = Subprojects::open_vcdiff::HashedDictionary{reinterpret_cast<const char*>(before.data()), before.size()};
            auto encoder = Subprojects::open_vcdiff::VCDiffStreamingEncoder{&hashed_dictionary, !Interleaved ? (Subprojects::open_vcdiff::VCDiffFormatExtensionFlagValues::VCD_STANDARD_FORMAT) : (Subprojects::open_vcdiff::VCDiffFormatExtensionFlagValues::VCD_FORMAT_INTERLEAVED), true};
            hashed_dictionary.Init();
            encoder.StartEncoding(&_patch);
            encoder.EncodeChunk(reinterpret_cast<const char*>(after.data()), after.size(), &_patch);
            encoder.FinishEncoding(&_patch);
            patch.raw(reinterpret_cast<uint8_t *>(_patch.data()), _patch.size());
        }

        static auto process (
            Uint8Array& before,
            Uint8Array& after,
            Uint8List& patch
        ) -> void {
            auto stream = WriteMemoryStream{patch};
            process_whole(before, after, stream);
            stream.release_stream(patch);
        }

    };
}
