#pragma once

#include "kernel/support/popcap/new_type_object_notation/common.hpp"
#include "kernel/support/popcap/new_type_object_notation/model.hpp"

namespace Sen::Kernel::Support::PopCap::NewTypeObjectNotation {

	struct Decode : Common {

	private:
		/*
        static auto exchange_number_string(ReadMemoryStream &stream, Optional<String> &value) -> void
        {
            if (const auto data = stream.u32(); data != 0_ui)
            {
                value = StringParser::from_uint32(data, "Failed to convert res");
            }
        }

	    static auto exchange_composite_subgroup(ReadMemoryStream &stream, CompositeSubgroup &value) -> void
        {
            exchange_number_string(stream, value.res);
            exchange_string_block<u32>(stream, value.id);
        }

        static auto exchange_composite_group(ReadMemoryStream &stream, CompositeGroup &value) -> void
        {
            exchange_null_block(stream, 4_size);
            exchange_list_size<u32>(stream, value.subgroups);
            const auto resource_count = stream.u32();
            const auto version = stream.u8();
            assert_conditional(version == k_default_version, fmt::format("{} {} {} {}", Kernel::Language::get("popcap.newton.decode.unknown_version"), version, Kernel::Language::get("popcap.newton.decode.at_index"), stream.current_position()), "exchange_composite_group");
            const auto group_has_parent = stream.boolean();
            assert_conditional(!group_has_parent, "Composite cannot contain parent", "exchange_composite_group"); //TODO:
            exchange_string_block<u32>(stream, value.id);
            assert_conditional(resource_count == k_empty, fmt::format("{}, id: {}", Kernel::Language::get("popcap.newton.decode.resource_must_be_null_with_composite"), value.id.view()), "exchange_composite_group");
            exchange_list<void>(&exchange_composite_subgroup, stream, value.subgroups);
        }

		static auto constexpr compute_atlas_null_block_count(
		) -> usize{
	        return sizeof(u32) * 8_size;
        }

	    static auto exchange_atlas_info(ReadMemoryStream &stream, AtlasInfo& value) -> void
        {
            exchange_enumeration<u8>(stream, value.type);
            exchange_number_fixed<u32>(stream, value.slot);
            exchange_number_fixed<u32>(stream, value.width);
            exchange_number_fixed<u32>(stream, value.height);
            exchange_null_block(stream, compute_atlas_null_block_count());
            exchange_boolean(stream, value.atlas);
            exchange_boolean(stream, value.runtime);
            exchange_null_block(stream, k_null_block_count);
            exchange_string_block<u32>(stream, value.id);
            exchange_string_block<u32>(stream, value.path);
        }

		static auto constexpr compute_type_null_block_count(
		) -> usize{
        	return sizeof(u32) * 2_size;
        }

	    static auto exchange_type_info(ReadMemoryStream &stream, TypeInfo &value) -> void
        {
            exchange_enumeration<u8>(stream, value.type);
        	exchange_number_fixed<u32>(stream, value.slot);
            exchange_null_block(stream, compute_type_null_block_count());
            exchange_number_fixed<i32>(stream, value.x);
            exchange_number_fixed<i32>(stream, value.y);
            exchange_number_fixed<i32>(stream, value.ax);
            exchange_number_fixed<i32>(stream, value.ay);
            exchange_number_fixed<i32>(stream, value.aw);
            exchange_number_fixed<i32>(stream, value.ah);
            exchange_number_fixed<i32>(stream, value.cols);
            exchange_number_fixed<i32>(stream, value.rows);
	        compare_value_to_reset(value.x, k_default_x_texture);
	        compare_value_to_reset(value.x, k_default_x_atlas);
	        compare_value_to_reset(value.y, k_default_y_texture);
	        compare_value_to_reset(value.y, k_default_y_atlas);
	        compare_value_to_reset(value.ax, k_default_ax);
	        compare_value_to_reset(value.ay, k_default_ay);
	        compare_value_to_reset(value.aw, k_default_aw);
	        compare_value_to_reset(value.ah, k_default_ah);
	        compare_value_to_reset(value.cols, k_default_cols);
	        compare_value_to_reset(value.rows, k_default_rows);
            exchange_null_block(stream, k_null_block_count);
            if (value.type == ResourceTypeEnumeration::pop_anim) {
                exchange_boolean(stream, value.forceOriginalVectorSymbolSize);
            }
            else {
                exchange_null_block(stream, 1_size);
            }
            const auto has_parent = stream.boolean();
            exchange_string_block<u32>(stream, value.id);
            exchange_string_block<u32>(stream, value.path);
            if (has_parent)
            {
                exchange_string_block<u32>(stream, value.parent);
            }
        }

		static auto constexpr compute_size (
		) -> usize {
	        return sizeof(u32) * 11_size + 1_size;
        }

	    static auto exchange_simple_info(ReadMemoryStream &stream, SimpleInfo& value) -> void
        {
            const auto current_pos = stream.current_position();
            stream += compute_size();
            const auto atlas = stream.boolean();
            stream.set_position(current_pos);
            if (atlas)
            {
                value.emplace<AtlasInfo>();
                auto& atlas_info = std::get<AtlasInfo>(value);
                exchange_atlas_info(stream, atlas_info);
            }
            else
            {
                value.emplace<TypeInfo>();
                auto& type_info = std::get<TypeInfo>(value);
                exchange_type_info(stream, type_info);
            }
        }

	    static auto exchange_simple_group(ReadMemoryStream &stream, SimpleResource &value) -> void
        {
            exchange_number_string(stream, value.res);
            const auto subgroup_count = stream.u32();
            exchange_list_size<u32>(stream, value.resources);
            const auto version = stream.u8();
            assert_conditional(version == 0x01, fmt::format("{} {} {} {}", Kernel::Language::get("popcap.newton.decode.unknown_version"), version, Kernel::Language::get("popcap.newton.decode.at_index"), stream.current_position()), "exchange_simple_group");
            const auto group_has_parent = stream.boolean();
            exchange_string_block<u32>(stream, value.id);
            if (group_has_parent)
            {
                exchange_string_block<u32>(stream, value.parent);
            }
            assert_conditional(subgroup_count == 0_ui, fmt::format("{}, id: {}", Kernel::Language::get("popcap.newton.decode.subgroup_must_be_null_with_simple"), value.id.view()), "exchange_simple_group");
            exchange_list<void>(&exchange_simple_info, stream, value.resources);
        }
	    
	    static auto exchange_group(ReadMemoryStream &stream, GroupInfo &value) -> void
	    {
	        auto group_type = GroupType{};
            exchange_enumeration<u8>(stream, group_type);
	        switch (group_type)
	        {
	        case GroupType::composite:
	            {
	                auto composite = CompositeGroup{};
	                composite.type = GroupType::composite;
	                exchange_composite_group(stream, composite);
	                value.emplace<CompositeGroup>(composite);
	                break;
	            }
	        case GroupType::simple:
	            {
	                auto simple = SimpleResource{};
	                simple.type = GroupType::simple;
	                exchange_simple_group(stream, simple);
	                value.emplace<SimpleResource>(simple);
	                break;
	            }
	        default:
	            {
	                assert_conditional(false, "Invalid group type", "exchange_group");  //TODO:
	            }
	        }
	    }
		*/

	public:
        static auto process_whole(ReadMemoryStream &stream, ResourcesModel& model) -> void
        {
        	/*
            model.version = k_default_version;
            model.content_version = k_default_version;
            exchange_number_fixed<u32>(stream, model.slot_count);
            exchange_list<u32>(&exchange_group, stream, model.groups);
            */
        }

	    static auto process_fs(StringView const &source, StringView const & destination) -> void
	    {
	        auto stream = ReadMemoryStream{source};
            auto model = ResourcesModel{};
            process_whole(stream, model);
            FileSystem::write_json(destination, model);
	    }
	};
}