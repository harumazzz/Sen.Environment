#pragma once

#include "kernel/support/popcap/new_type_object_notation/common.hpp"
#include "kernel/support/popcap/new_type_object_notation/model.hpp"

namespace Sen::Kernel::Support::PopCap::NewTypeObjectNotation {

	struct Encode : Common {

	private:
	    /*
        static auto exchange_number_string(WriteMemoryStream &stream, Optional<String> const &value) -> void
        {
            if (value.has_value())
            {
                const auto& string = *value;
                const auto data = StringParser::to_uint32(string, "Failed to convert res");
                exchange_number_fixed<u32>(stream, data);
            }
            else
            {
                exchange_number_fixed<u32>(stream, k_empty);
            }
        }

	    static auto exchange_composite_subgroup(WriteMemoryStream &stream, CompositeSubgroup const &value) -> void
        {
            exchange_number_string(stream, value.res);
            exchange_string_block<u32>(stream, value.id);
        }

        static auto exchange_composite_group(WriteMemoryStream &stream, CompositeGroup const &value) -> void
        {
            exchange_null_block(stream, 4_size);
            exchange_list_size<u32>(stream, value.subgroups);
            exchange_null_block(stream, 4_size);
            exchange_number_fixed<u8>(stream, k_default_version);
            exchange_boolean(stream, false);
            exchange_string_block<u32>(stream, value.id);
            exchange_list<void>(&exchange_composite_subgroup, stream, value.subgroups);
        }

	    static auto constexpr compute_atlas_null_block_count (
	    ) -> usize {
    	    return sizeof(u32) * 6_size;
	    }

	    static auto exchange_atlas_info(WriteMemoryStream &stream, AtlasInfo const &value) -> void
        {
            exchange_enumeration<u8>(stream, value.type);
            exchange_number_fixed<u32>(stream, value.slot);
            exchange_number_fixed<u32>(stream, value.width);
            exchange_number_fixed<u32>(stream, value.height);
            exchange_null_block(stream, compute_atlas_null_block_count());
            exchange_number_fixed<u32>(stream, k_default_cols); //cols;
            exchange_number_fixed<u32>(stream, k_default_rows); //rows;
            exchange_boolean(stream, value.atlas);
            exchange_boolean(stream, value.runtime);
            exchange_null_block(stream, 2_size);
            exchange_string_block<u32>(stream, value.id);
            exchange_string_block<u32>(stream, value.path);
        }

	    static auto constexpr compute_type_null_block_count (
	    ) -> usize {
            return sizeof(u32) * 2_size;
        }

	    static auto exchange_type_info(WriteMemoryStream &stream, TypeInfo const &value) -> void
        {
            exchange_enumeration<u8>(stream, value.type);
        	exchange_number_fixed<u32>(stream, value.slot);
            exchange_null_block(stream, compute_type_null_block_count());
            const auto x = value.type == ResourceTypeEnumeration::pop_anim ? k_default_x_atlas : value.x.has_value() ? *value.x : k_default_x_texture;
            const auto y = value.type == ResourceTypeEnumeration::pop_anim ? k_default_y_atlas : value.y.has_value() ? *value.y : k_default_y_texture;
            exchange_number_fixed<i32>(stream, x);
            exchange_number_fixed<i32>(stream, y);
            exchange_number_fixed<i32>(stream, value.ax);
            exchange_number_fixed<i32>(stream, value.ay);
            exchange_number_fixed<i32>(stream, value.aw);
            exchange_number_fixed<i32>(stream, value.ah);
            const auto cols = value.cols.has_value() ? *value.cols : k_default_cols;
            const auto rows = value.rows.has_value() ? *value.rows : k_default_rows;
            exchange_number_fixed<i32>(stream, cols);
            exchange_number_fixed<i32>(stream, rows);
            exchange_null_block(stream, k_null_block_count);
            if (value.type == ResourceTypeEnumeration::pop_anim)
            {
                exchange_boolean(stream, value.forceOriginalVectorSymbolSize);
            }
            else
            {
                exchange_null_block(stream, 1_size);
            }
            exchange_boolean(stream, value.parent.has_value());
            exchange_string_block<u32>(stream, value.id);
            exchange_string_block<u32>(stream, value.path);
            if (value.parent.has_value())
            {
                exchange_string_block<u32>(stream, value.parent);
            }
        }

	    static auto exchange_simple_info(WriteMemoryStream &stream, SimpleInfo const& value) -> void
        {
            if (std::holds_alternative<AtlasInfo>(value))
            {
                exchange_atlas_info(stream, std::get<AtlasInfo>(value));
            }
            else if (std::holds_alternative<TypeInfo>(value))
            {
                exchange_type_info(stream, std::get<TypeInfo>(value));
            }
            else
            {
                assert_conditional(false, "Invalid simple info type", "exchange_simple_info");
            }
        }

	    static auto exchange_simple_group(WriteMemoryStream &stream, SimpleResource const &value) -> void
        {
            exchange_number_string(stream, value.res);
            exchange_null_block(stream, 4_size);
            exchange_list_size<u32>(stream, value.resources);
            exchange_number_fixed<u8>(stream, k_default_version);
            exchange_boolean(stream, value.parent.has_value());
            exchange_string_block<u32>(stream, value.id);
            if (value.parent.has_value())
            {
                exchange_string_block<u32>(stream, value.parent);
            }
            exchange_list<void>(&exchange_simple_info, stream, value.resources);
        }

	    static auto exchange_group(WriteMemoryStream &stream, GroupInfo const &value) -> void
	    {
            if (std::holds_alternative<SimpleResource>(value))
            {
                exchange_enumeration<u8>(stream, GroupType::simple);
                exchange_simple_group(stream, std::get<SimpleResource>(value));
            }
            else if (std::holds_alternative<CompositeGroup>(value))
            {
                exchange_enumeration<u8>(stream, GroupType::composite);
                exchange_composite_group(stream, std::get<CompositeGroup>(value));
            }
            else
            {
                assert_conditional(false, "Invalid group type", "exchange_group"); //TODO:
            }
	    }

        */
	public:
        static auto process_whole(WriteMemoryStream &stream, ResourcesModel const& model) -> void
        {
            /*
            stream.allocate_full(6400000);
            exchange_number_fixed<u32>(stream, model.slot_count);
            exchange_list<u32>(&exchange_group, stream, model.groups);
            */
        }

	    static auto process_fs(StringView const &source, StringView const & destination) -> void
	    {
	        auto stream = WriteMemoryStream{};
            auto model = ResourcesModel{};
            FileSystem::read_json<ResourcesModel>(source, model);
            process_whole(stream, model);
            FileSystem::write_file(destination, stream.view());
	    }
	};
}