#pragma once

#include "kernel/utility/utility.hpp"

namespace Sen::Kernel::Support::PopCap::NewTypeObjectNotation {


	using namespace Sen::Kernel; 


	struct Encode {
		
		constexpr explicit Encode(
		) = default;

		constexpr ~Encode(

		) = default;

		inline static auto is_not_null_object (
			nlohmann::ordered_json& json,
			const std::string& property
		) -> bool 
		{
			return json.find(property) != json.end();
		}

		inline static auto is_null_object (
			nlohmann::ordered_json& json,
			const std::string& property
		) -> bool 
		{
			return json.find(property) == json.end();
		}

		inline static auto validate_resource (
			nlohmann::ordered_json& resource
		) -> void {
			assert_conditional(is_not_null_object(resource, "slot_count"),fmt::format("{}", Kernel::Language::get("popcap.newton.encode.slot_count_cannot_be_null")),"validate_resource");
		}

		inline static auto write_slot_and_group_count(
			DataStreamView& stream,
			nlohmann::ordered_json& resource
		) -> void {
			stream.writeInt32(resource["slot_count"].get<int>());
			stream.writeInt32(static_cast<int>(resource["groups"].size()));
		}

		inline static auto process_group(
			DataStreamView& stream,
			nlohmann::ordered_json& group
		) -> void {
			write_group_type(stream, group);
			write_group_info(stream, group);
			process_group_type(stream, group);
		}

		inline static auto process_group_type (
			DataStreamView& stream,
			nlohmann::ordered_json& group
		) -> void {
			switch (hash_string(group["type"].get<std::string>())) {
				case hash_string("composite"_sv):
					process_composite_group(stream, group);
					break;
				case hash_string("simple"_sv):
					process_simple_group(stream, group);
					break;
			}
		}

		inline static auto write_group_type (
			DataStreamView& stream,
			nlohmann::ordered_json& group
		) -> void {
			switch (hash_string(group["type"].get<std::string>())) {
				case hash_string("composite"_sv):
					stream.writeUint8(0x01);
					break;
				case hash_string("simple"_sv):
					stream.writeUint8(0x02);
					break;
				default:
					assert_conditional(false,fmt::format("{} {} {} {}", Kernel::Language::get("popcap.newton.encode.unknown_type"), group["type"].get<std::string>(), Kernel::Language::get("popcap.newton.encode.at_group_id"), group["id"].get<std::string>()), "write_group_type");
			}
		}

		inline static auto write_group_info (
			DataStreamView& stream,
			nlohmann::ordered_json& group
		) -> void {
			auto subgroups_count = is_null_object(group, "subgroups") ? 0x00 : group["subgroups"].size();
			auto resources_count = is_null_object(group, "resources") ? 0x00 : group["resources"].size();
			stream.writeInt32(is_null_object(group, "res") ? 0x00 : Converter::to_int32(group["res"].get<std::string>(),String::format(fmt::format("{}", Language::get("popcap.newton.invalid_res_type")),group["res"].get<std::string>())));
			stream.writeInt32(subgroups_count);
			stream.writeInt32(resources_count);
			stream.writeUint8(0x01);
			if (is_not_null_object(group, "parent")) {
				stream.writeUint8(0x01);
			} else {
				stream.writeUint8(0x00);
			}
			stream.writeUint32(group["id"].get<std::string>().size());
			stream.writeString(group["id"].get<std::string>());
			if (is_not_null_object(group, "parent")) {
				stream.writeUint32(group["parent"].get<std::string>().size());
				stream.writeString(group["parent"].get<std::string>());
			}
		}

		inline static auto process_composite_group (
			DataStreamView& stream,
			nlohmann::ordered_json& group
		) -> void {
			assert_conditional(is_null_object(group, "resources"), fmt::format("{}", Kernel::Language::get("popcap.newton.encode.resource_must_be_null_with_composite")), "process_composite_group");
			for (auto& subgroup : group["subgroups"]) {
				stream.writeInt32(is_null_object(subgroup, "res") ? 0x00 : Converter::to_int32(
					subgroup["res"].get<std::string>(),
					String::format(
						fmt::format("{}", Language::get("popcap.newton.invalid_res_type")),
						subgroup["res"].get<std::string>()
					)
				));
				stream.writeUint32(subgroup["id"].get<std::string>().size());
				stream.writeString(subgroup["id"].get<std::string>());
			}
		}

		inline static auto process_simple_group(
			DataStreamView& stream,
			nlohmann::ordered_json& group
		) -> void {
			assert_conditional(is_null_object(group, "subgroups"),fmt::format("{}", Kernel::Language::get("popcap.newton.encode.subgroup_must_be_null_with_simple")),"process_simple_group");
			for (auto& resource_x : group["resources"]) {
				write_resource_type(stream, resource_x);
				writeResourceDetails(stream, resource_x);
			}
		}

		inline static auto write_resource_type(
			DataStreamView& stream,
			nlohmann::ordered_json& resource
		) -> void {
			switch (hash_string(resource["type"].get<std::string>())) {
				case hash_string("Image"_sv):
					stream.writeUint8(0x01);
					break;
				case hash_string("PopAnim"_sv):
					stream.writeUint8(0x02);
					break;
				case hash_string("SoundBank"_sv):
					stream.writeUint8(0x03);
					break;
				case hash_string("File"_sv):
					stream.writeUint8(0x04);
					break;
				case hash_string("PrimeFont"_sv):
					stream.writeUint8(0x05);
					break;
				case hash_string("RenderEffect"_sv):
					stream.writeUint8(0x06);
					break;
				case hash_string("DecodedSoundBank"_sv):
					stream.writeUint8(0x07);
					break;
				default:
					assert_conditional(false,fmt::format("{} {} {}", Kernel::Language::get("popcap.newton.encode.invalid_type"), Kernel::Language::get("popcap.newton.encode.at_group_id"), resource["id"].get<std::string>()), "write_resource_type");
			}
		}

		inline static auto writeResourceDetails(
			DataStreamView& stream,
			nlohmann::ordered_json& resource
		) -> void {
			stream.writeInt32(resource["slot"]);
			stream.writeInt32(is_null_object(resource, "width") ? 0x00 : resource["width"].get<int>());
			stream.writeInt32(is_null_object(resource, "height") ? 0x00 : resource["height"].get<int>());
			auto is_sprite = is_not_null_object(resource, "aw") && resource["aw"].get<int>() != 0 &&
							is_not_null_object(resource, "ah") && resource["ah"].get<int>() != 0;
			stream.writeInt32(is_null_object(resource, "x") ? (is_sprite ? 0x00 : 0x7FFFFFFF) : resource["x"].get<int>());
			stream.writeInt32(is_null_object(resource, "y") ? (is_sprite ? 0x00 : 0x7FFFFFFF) : resource["y"].get<int>());
			stream.writeInt32(is_null_object(resource, "ax") ? 0x00 : resource["ax"].get<int>());
			stream.writeInt32(is_null_object(resource, "ay") ? 0x00 : resource["ay"].get<int>());
			stream.writeInt32(is_null_object(resource, "aw") ? 0x00 : resource["aw"].get<int>());
			stream.writeInt32(is_null_object(resource, "ah") ? 0x00 : resource["ah"].get<int>());
			stream.writeInt32(is_null_object(resource, "cols") ? 0x01 : resource["cols"].get<int>());
			stream.writeInt32(is_null_object(resource, "rows") ? 0x01 : resource["rows"].get<int>());
			stream.writeUint8(is_not_null_object(resource, "atlas") && resource["atlas"].get<bool>() ? 0x01 : 0x00);
			stream.writeUint8(0x01);
			stream.writeUint8(0x01);
			auto has_parent = is_not_null_object(resource, "parent");
			stream.writeUint8(has_parent ? 0x01 : 0x00);
			stream.writeUint32(resource["id"].get<std::string>().size());
			stream.writeString(resource["id"].get<std::string>());
			stream.writeUint32(resource["path"].get<std::string>().size());
			stream.writeString(resource["path"].get<std::string>());
			if (has_parent) {
				stream.writeUint32(resource["parent"].get<std::string>().size());
				stream.writeString(resource["parent"].get<std::string>());
			}
		}

		inline static auto process(
			DataStreamView& stream,
			nlohmann::ordered_json& resource
		) -> void {
			validate_resource(resource);
			write_slot_and_group_count(stream, resource);
			for (auto& m_data : resource["groups"]) {
				process_group(stream, m_data);
			}
		}

		inline static auto process_fs(
			std::string_view source,
			std::string_view destination
		) -> void
		{
			auto resource = FileSystem::read_json(source);
			auto stream = DataStreamView{};
			process(stream, resource.operator*());
			stream.out_file(destination);
			return;
		}

	};
}