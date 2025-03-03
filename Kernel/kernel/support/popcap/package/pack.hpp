#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/package/model.hpp"

namespace Sen::Kernel::Support::PopCap::Package
{
    struct Pack : Common
    {
    private:
        /*
        template <auto compress, typename WriteStream> requires is_writable_stream<WriteStream> && std::is_same_v<type_of<compress>, bool>
        static auto process_package(WriteStream &stream, PackageInfo const &model, StringView const &resource_directory) -> void
        {
            exchange_number_fixed<u32>(stream, k_magic_identifier);
            exchange_number_fixed<u32>(stream, k_version);
            auto resource_data_section_view_stored_list = List<Uint8Array>{};
            resource_data_section_view_stored_list.resize(model.resource.size());
            for (const auto index : Range{model.resource.size()})
            {
                const auto &[file_source] = model.resource[index];
                exchange_enumeration<u8>(stream, ResourceInformationListStateFlag::next);
                exchange_string_block<u8>(stream, file_source);
                auto resource_data = Uint8Array{};
                FileSystem::read_file(Path::join(resource_directory, file_source), resource_data);
                exchange_number_fixed<u32>(stream, resource_data.size());
                if constexpr (compress)
                {
                    auto compressed_data = Uint8Array{};
                    Compression::Zlib::Compress::process(resource_data, compressed_data);
                    resource_data_section_view_stored_list[index].assign(compressed_data);
                    exchange_number_fixed<u32>(stream, compressed_data.size());
                }
                else
                {
                    resource_data_section_view_stored_list[index].assign(resource_data);
                }
                exchange_null_block(stream, sizeof(u64));
            }
            exchange_enumeration<u8>(stream, ResourceInformationListStateFlag::done);
            for (auto &resource_data : resource_data_section_view_stored_list)
            {
                stream.bytes(resource_data);
            }
        }
        */
    public:
        /*
        template <typename WriteStream> requires is_writable_stream<WriteStream>
        static auto process_whole(WriteStream &stream, PackageInfo const &model, StringView const &destination, bool const& compressed) -> void
        {
            const auto resource_directory = Path::join(destination, String{"resource"});
            if (compressed)
            {
                process_package<true>(stream, model, resource_directory);
            }
            else
            {
                process_package<false>(stream, model, resource_directory);
            }

        }
        */
        static auto process_fs(
            StringView const& source,
            StringView const& destination,
            bool const& compressed
        ) -> void
        {
            auto stream = WriteFileStream{source};
            auto model = PackageInfo{};
            FileSystem::read_json(source, model);
            //process_whole(stream, model, destination, compressed);
        }
    };
}