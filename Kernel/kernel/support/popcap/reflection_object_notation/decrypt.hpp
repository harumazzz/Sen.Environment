#pragma once

#include "kernel/utility/utility.hpp"

namespace Sen::Kernel::Support::PopCap::ReflectionObjectNotation {

    struct Decrypt : Common {

    protected:

        using Common = Common;

    public:

        static auto process (
            const Uint8Array& source,
            Uint8Array& destination,
            const StringView& key,
            const StringView& iv
        ) -> void {
            return Encryption::Rijndael::Decrypt<Encryption::Rijndael::Mode::CBC>::process(source, key, iv, destination);
        }

        static auto process_fs (
            const StringView& source,
            const StringView& destination,
            const StringView& key,
            const StringView& iv
        ) -> void {
            auto source_view = Uint8Array{};
            FileSystem::read_file(source, source_view);
            auto destination_view = Uint8Array{source_view.size()};
            process(source_view, destination_view, key, iv);
            FileSystem::write_file(destination, destination_view);
        }

    };

}