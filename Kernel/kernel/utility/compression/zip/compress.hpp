#pragma once

#include "kernel/utility/compression/zip/common.hpp"

namespace Sen::Kernel::Compression::Zip {

    struct Compress : Common {

    protected:

        using Common = Common;

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

        auto operator=(
            const Compress& other
        ) -> Compress& = delete;

        auto operator=(
            Compress&& other
        ) -> Compress& = delete;

        static auto process (
            HashMap<String, Uint8Array>& source,
            Uint8Array& destination
        ) -> void {
            auto zip = Subprojects::zip::zip_stream_open(nullptr, 0, 0, 'w');
            assert_conditional(zip != nullptr, "Failed to create ZIP archive", "process");
            for (auto& [name, data] : source) {
                Subprojects::zip::zip_entry_open(zip, name.begin());
                Subprojects::zip::zip_entry_write(zip, data.data(), data.size());
                Subprojects::zip::zip_entry_close(zip);
            }
            auto size = static_cast<usize>(Subprojects::zip::zip_stream_copy(zip, nullptr, nullptr));
            auto dest = new u8[size];
            Subprojects::zip::zip_stream_copy(zip, reinterpret_cast<void **>(&dest), &size);
            destination.assign(as_lvalue(Uint8Array{dest, size}));
            Subprojects::zip::zip_close(zip);
        }


        static auto process_fs (
            const String& source,
            const String& destination
        ) -> void {
            auto raw = Uint8Array{};
            auto ripe = HashMap<String, Uint8Array>{};
            auto data = CList<String>{};
            FileSystem::read_recursive_directory_file(source, data);
            ripe.reserve(data.size());
            for (auto & file : data) {
                auto view = Uint8Array{};
                FileSystem::read_file(file, view);
                ripe.try_emplace(file, std::move(view));
            }
            process(ripe, raw);
            FileSystem::write_file(destination, raw);
        }


    };

}

