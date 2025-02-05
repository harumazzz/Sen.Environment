#pragma once

#include "kernel/utility/container/array/byte_array.hpp"
#include "kernel/utility/diff/vcdiff/common.hpp"
#include "kernel/utility/stream/write_stream.hpp"

namespace Sen::Kernel::Diff::VCDiff {

    struct Decode : Common {

    protected:

        using Common = Common;

    public:

        static auto process_whole (
            Uint8Array& before,
            WriteStream& after,
            Uint8Array& patch
        ) -> void {
            auto after_ = std::string{};
            auto decoder = Subprojects::open_vcdiff::VCDiffStreamingDecoder{};
            decoder.SetAllowVcdTarget(true);
            decoder.SetMaximumTargetFileSize(limit);
            decoder.SetMaximumTargetWindowSize(maximum_window_size);
            decoder.StartDecoding(reinterpret_cast<const char*>(before.begin()), before.size());
            decoder.DecodeChunk(reinterpret_cast<const char*>(patch.begin()), patch.size(), &after_);
            decoder.FinishDecoding();
            for (auto & element : after_) {
                after.u8(element);
            }
        }

        static auto process (
            Uint8Array& before,
            Uint8List& after,
            Uint8Array& patch
        ) -> void {
            auto stream = WriteStream{after};
            process_whole(before, stream, patch);
            stream.release_stream(after);
        }

    };
}
