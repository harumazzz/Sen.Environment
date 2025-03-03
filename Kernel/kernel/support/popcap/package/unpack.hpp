#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/package/model.hpp"
#include "kernel/support/popcap/package/common.hpp"

namespace Sen::Kernel::Support::PopCap::Package
{
    struct Unpack : Common
    {
    private:
        /*
        template <auto compress, typename ReadStream> requires is_readable_stream<ReadStream> && std::is_same_v<type_of<compress>, bool>
        static auto process_package(ReadStream &stream, PackageInfo &model, StringView const &resource_directory) -> void
        {
            assert_conditional(stream.u32() == k_magic_identifier, fmt::format("{}", Language::get("popcap.pak.unpack.invalid_pak_header")), "process_package");
            assert_conditional(stream.u32() == k_version, fmt::format("{}", Language::get("popcap.package.invalid_version")) , "process_package");
            auto resource_information_list = List<ResourceInformation>{};
            while (true)
            {
                const auto flag = static_cast<ResourceInformationListStateFlag>(stream.u8());
                if (flag == ResourceInformationListStateFlag::done)
                {
                    break;
                }
                if (flag == ResourceInformationListStateFlag::next)
                {
                    auto make_resource = [&]() -> ResourceInformation {
                        auto resource_information = ResourceInformation{};
                        exchange_string_block<u8>(stream, resource_information.path);
                        exchange_number_fixed<u32>(stream, resource_information.size);
                        if constexpr (compress)
                        {
                            exchange_number_fixed<u32>(stream, resource_information.size_original);
                        }
                        exchange_null_block(stream, sizeof(u64));
                        return resource_information;
                    };
                    resource_information_list.append(make_resource());
                }
                assert_conditional(false, fmt::format("{}", Language::get("popcap.package.unpack_failed")), "process_package");
            }
            model.resource.resize(resource_information_list.size());
            for (const auto index : Range{resource_information_list.size()})
            {
                const auto & resource_information_structure = resource_information_list[index];
                model.resource[index].path = resource_information_structure.path;
                auto resource_data = stream.bytes(resource_information_structure.size);
                const auto file_destination = Path::join(resource_directory, resource_information_structure.path);
                if (FileSystem::exist(Path::dirname(file_destination)))
                {
                    FileSystem::create_directory(file_destination);
                }
                if constexpr (compress)
                {
                    auto uncompressed_data = Uint8Array{resource_information_structure.size_original};
                    Compression::Zlib::Uncompress::process(resource_data, uncompressed_data);
                    FileSystem::write_file(file_destination, uncompressed_data);
                }
                else
                {
                    FileSystem::write_file(file_destination, resource_data);
                }
            }
        }
        */
    public:
        /*
        template <typename ReadStream> requires is_readable_stream<ReadStream>
        static auto process_whole(ReadStream &stream, PackageInfo &model, StringView const &destination, bool const& compressed) -> void
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
            bool const& compressed) -> void
        {
            auto stream = ReadFileStream{source};
            auto model = PackageInfo{};
            //process_whole(stream, model, destination, compressed);
            FileSystem::write_json(source, model);
        }
    };
}
