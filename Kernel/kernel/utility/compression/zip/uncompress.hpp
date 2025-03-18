#pragma once

#include "kernel/utility/compression/zip/common.hpp"

namespace Sen::Kernel::Compression::Zip {

    struct Uncompress : Common {

    protected:

        using Common = Common;

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

        auto operator=(
            const Uncompress& other
        ) -> Uncompress& = delete;

        auto operator=(
            Uncompress&& other
        ) -> Uncompress& = delete;

        static auto make_value (
            Subprojects::zip::zip_t* zip,
            const usize& size
        ) -> Uint8Array {
            auto value = Uint8Array{size};
            Subprojects::zip::zip_entry_extract(zip, static_cast<size_t (*)(void *, uint64_t, const void *, size_t)>(on_extract), &value);
            return value;
        }

        static auto process (
            Uint8Array& source,
            HashMap<String, Uint8Array>& destination
        ) -> void {
            auto zip = Subprojects::zip::zip_stream_open(reinterpret_cast<char*>(source.data()), source.size(), 0, 'r');
            assert_conditional(zip != nullptr, "Failed to open zip file", "process");
            auto count = static_cast<usize>(Subprojects::zip::zip_entries_total(zip));
            destination.reserve(count);
            for (auto index : Range{count}) {
                auto entry = Subprojects::zip::zip_entry_openbyindex(zip, index);
                auto name = Subprojects::zip::zip_entry_name(zip);
                auto size = Subprojects::zip::zip_entry_size(zip);
                destination.emplace(String{name, static_cast<usize>(size)}, make_value(zip, size));
                Subprojects::zip::zip_entry_close(zip);
            }
            Subprojects::zip::zip_close(zip);
        }

        static auto process_fs (
            const String& source,
            const String& destination
        ) -> void {
            auto raw = Uint8Array{};
            auto ripe = HashMap<String, Uint8Array>{};
            FileSystem::read_file(source, raw);
            process(raw, ripe);
            for (auto& [name, data] : ripe) {
                FileSystem::write_file(Path::join(destination, name), data);
            }
        }

    };

}

