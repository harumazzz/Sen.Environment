#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/resource_stream_group/model.hpp"

namespace Sen::Kernel::Support::PopCap::ResourceStreamGroup
{

    struct Common
    {
        static constexpr auto k_magic_identifier = 0x72736770_ui;

        static constexpr auto version_list = {3_ui, 4_ui}; // unsupported rsg version 1.

        static constexpr auto compression_list = {0_ui, 1_ui, 2_ui, 3_ui};

        static constexpr auto contain_compression(u32 const& value) -> bool
        {
            return std::ranges::find(compression_list, value) != compression_list.end();
        }

        static constexpr auto contain_version(u32 const& version) -> bool
        {
            return std::ranges::find(version_list, version) != version_list.end();
        }


        static auto exchange_packet_resource(
            Uint32ReadFileStream &stream,
            Optional<ItemResource> &value,
            const std::string_view& path
        ) -> void {
            if (static_cast<bool>(stream.numeric())) {
                value.emplace(std::in_place_type<TextureResource>);
                auto &texture = std::get<TextureResource>(value.operator*());
                texture.path = String{path.data(), path.size()};
                exchange_numeric(stream, texture._mapping.offset);
                exchange_numeric(stream, texture._mapping.size);
                exchange_numeric(stream, texture._mapping.index);
                exchange_null_block(stream, 2_size); // skip
                exchange_numeric(stream, texture.dimension.width);
                exchange_numeric(stream, texture.dimension.height);
            } else {
                value.emplace(std::in_place_type<GeneralResource>);
                auto &general = std::get<GeneralResource>(value.operator*());
                general.path = String{path.data(), path.size()};
                exchange_numeric(stream, general._mapping.offset);
                exchange_numeric(stream, general._mapping.size);
            }
        }

        static auto get_packet_information(Uint32ReadFileStream &stream, PacketInformation &model) -> void {
            assert_conditional(stream.numeric() == k_magic_identifier, fmt::format("{}", Language::get("popcap.rsg.unpack.invalid_rsg_magic")), "get_packet_information");
            exchange_numeric(stream, model.version);
            assert_conditional(contain_version(model.version), fmt::format("{}", Language::get("popcap.rsg.invalid_version")), "get_packet_information");
            exchange_null_block(stream, 2_size);
            exchange_numeric(stream, model.compression);
            stream.set_position(0x48_size);
            compiled_map_decode(&exchange_packet_resource, stream, model.resource);
        }


        template <typename Exchanger> requires is_exchanger<Exchanger>
        static auto compiled_map_decode(
            Exchanger const& exchanger,
            Uint32ReadFileStream& stream,
            List<ItemResource>& value_list
        ) -> void
        {
            const auto start_position = stream.current_position() - 0x48_size;
            const auto resource_information_section_size = stream.numeric();
            const auto resource_information_section_offset = stream.numeric();
            const auto resource_information_end = start_position + resource_information_section_offset + resource_information_section_size;
            stream.set_position(start_position + resource_information_section_offset);
            auto parent_string = HashMap<usize, std::string>{};
            while (stream.current_position() < resource_information_end)
            {
                auto string_stream = std::ostringstream{};
                if (const auto position = (stream.current_position() - resource_information_section_offset - start_position) / k_block_size; parent_string.contains(position))
                {
                    string_stream << parent_string[position];
                    parent_string.erase(position);
                }
                while (true)
                {
                    const auto composite_value = stream.numeric();
                    const auto child_string_offset = (composite_value & 0xffffff00) >> 8;
                    const auto current_character = composite_value & 0xff;
                    if (child_string_offset != k_none_size)
                    {
                        parent_string[child_string_offset] = string_stream.str();
                    }
                    if (current_character == k_none_size)
                    {
                        break;
                    }
                    if (current_character == 0x5C)
                    {
                        string_stream << static_cast<char>(0x2F);
                    }
                    else
                    {
                        string_stream << static_cast<char>(current_character);
                    }
                }
                if (string_stream.view().empty())
                {
                    break;
                }
                auto make_resource = [&]() -> Optional<ItemResource>
                {
                    auto resource = Optional<ItemResource>{};
                    exchanger(stream, resource, string_stream.str());
                    return resource;
                };
                value_list.append(make_resource().operator*());
            }
        }
        /*
        static auto get_common_size(
            StringView const& range_1,
            StringView const& range_2) -> size_t
        {

            const auto maximum_common_size = range_1.size() >= range_2.size() ? range_1.size() : range_2.size();
            for (const auto index : Range{maximum_common_size})
            {
                const auto char_a = index < range_1.size() ? range_1[index] : '\0';
                if (const auto char_b = index < range_2.size() ? range_2[index] : '\0'; char_a != char_b)
                {
                    return index;
                }
            }
            return maximum_common_size;
        }

        template <typename Exchanger> requires is_exchanger<Exchanger>
        static auto compiled_map_encode(Exchanger const& exchanger, WriteMemoryStream& stream, List<ItemResource>& value_list) -> void
        {
            const auto resource_information_section_offset = stream.current_position();
            //  debug(fmt::format("resource_information_section_offset: {}", resource_information_section_offset));
            auto string_list = List<String>{value_list.size()};
            auto work_option = std::vector<WorkOption>{value_list.size()};
            for (auto const& resource : value_list)
            {
                if (std::holds_alternative<TextureResource>(resource))
                {
                    auto path = std::get<TextureResource>(resource).path;
                    StringHelper::make_uppercase_string(path);
                    string_list.append(path);
                }
                else if (std::holds_alternative<GeneralResource>(resource))
                {
                    auto path = std::get<GeneralResource>(resource).path;
                    StringHelper::make_uppercase_string(path);
                    string_list.append(path);
                }
            }
            std::sort(string_list.begin(), string_list.end(), [](auto const& lhs, auto const& rhs) -> bool
            {
                return lhs.view() < rhs.view();
            });
            if (!string_list.empty())
            {
                work_option[k_begin_index] = WorkOption{
                    .inherit_length = k_none_size,
                    .parent_offset = k_begin_index,
                    .has = true
                };
                for (const auto index : Range{string_list.size()})
                {
                    auto& element_key = string_list[index];
                    //debug(fmt::format("index: {}, key: {}", index, element_key.view()));
                    auto current_string_has_child = std::vector<bool>(element_key.length() + 1_size);
                    for (const auto index_1 : Range{index + 1_size, string_list.size(), 1_size})
                    {
                        if (!work_option[index_1].has)
                        {
                            const auto common_size = get_common_size(element_key, string_list[index_1]);
                            if (!current_string_has_child[common_size] && common_size >= work_option[index].inherit_length)
                            {
                                current_string_has_child[common_size] = true;
                                work_option[index_1] = WorkOption{
                                    .inherit_length = common_size,
                                    .parent_offset = (stream.current_position() / k_block_size + (common_size - work_option[index].inherit_length)),
                                    .has = true
                                };
                            }
                        }
                    }
                    auto character_index = k_none_size;
                   // debug(fmt::format("index: {}, has: {}, char_index: {}", index, work_option[index].has, character_index));
                    if (work_option[index].has)
                    {
                        character_index += work_option[index].inherit_length;
                        auto current_position = stream.current_position();
                        auto parent_position = work_option[index].parent_offset * k_block_size;
                       // auto p = stream.ru32(parent_position);
                       // debug(fmt::format("index: {}, p: {}, parent_position: {}", index, p, parent_position));
                      //  const auto composite_value = p | ((current_position - resource_information_section_offset) / k_block_size) << 8;
                        stream.set_position(parent_position);
                      //  stream.u32(composite_value);
                        stream.set_position(current_position);
                        work_option[index] = WorkOption{};
                    }
                    while (character_index < element_key.size())
                    {
                        if (const auto current_character = element_key[character_index]; current_character == 0x2F)
                        {
                            stream.u8(0x5C_u8);
                        }
                        else
                        {
                            stream.u8(static_cast<u8>(element_key[character_index]));
                        }
                        exchange_null_block(stream, 3_size);
                        ++character_index;
                    }
                    exchange_null_block(stream, k_block_size);
                    exchanger(stream, value_list[index]);
                }
            }

        }
        */
    };
}
