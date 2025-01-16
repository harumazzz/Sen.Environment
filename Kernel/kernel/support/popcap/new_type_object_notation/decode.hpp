#pragma once

#include "kernel/utility/utility.hpp"

namespace Sen::Kernel::Support::PopCap::NewTypeObjectNotation {

	using namespace Sen::Kernel;

	struct Decode {

		constexpr Decode(

		) = default;

		constexpr ~Decode(

		) = default;

		inline static auto read_group_type(
			uint8_t group_type, 
			int index,
        	std::string& group_type_str
		) -> void {
			switch (static_cast<int>(group_type)) {
				case 0x01: 
					group_type_str = "composite";
					break;
				case 0x02: 
					group_type_str = "simple";
					break;
				default:
					assert_conditional(false, fmt::format("{} {}. {}: group[\"type\"] == 1 || group[\"type\"] == 2, {} {}", Language::get("popcap.newton.invalid_group_type"), index, Language::get("conditional"), Language::get("but_received"), group_type), "read_group_type");
			}
		}

		inline static auto read_resource_type(
			uint8_t resource_type, 
			const std::string& group_id,
			std::string& result
		) -> void
		{
			switch (static_cast<int>(resource_type)) {
				case 0x01: 
					result = "Image"; 
					break;
				case 0x02: 
					result = "PopAnim"; 
					break;
				case 0x03: 
					result = "SoundBank"; 
					break;
				case 0x04: 
					result = "File"; 
					break;
				case 0x05: 
					result = "PrimeFont"; 
					break;
				case 0x06: 
					result = "RenderEffect"; 
					break;
				case 0x07: 
					result = "DecodedSoundBank"; 
					break;
				default:
					assert_conditional(false, fmt::format("{} {}, {}", Language::get("popcap.newton.invalid_resource_type"), group_id, Language::get("popcap.newton.expected_from_to"), resource_type), "read_resource_type");
			}
		}

		inline static auto process(
			DataStreamView& stream, 
        	nlohmann::ordered_json& result
		) -> void{
			result = nlohmann::ordered_json{
				{"version", 1},
				{"content_version", 1},
				{"slot_count", stream.readUint32()}
			};
			auto group_size = stream.readUint32();
			auto groups = nlohmann::ordered_json::array_t{};
			groups.reserve(group_size);
			for (auto i : Range(group_size)) {
				auto group = nlohmann::ordered_json{};
				auto group_type = stream.readUint8();
				auto group_s = std::string{};
				read_group_type(group_type, i, group_s);
				group["type"] = std::move(group_s);
				auto res = stream.readUint32();
				if (res != 0x00) {
					group["res"] = fmt::format("{}", res);
				}
				auto subgroups_count = stream.readUint32();
				auto resources_count = stream.readInt32();
				auto version = stream.readUint8();
				assert_conditional(version == 0x01, fmt::format("{} {} {} {}", Kernel::Language::get("popcap.newton.decode.unknown_version"), version, Kernel::Language::get("popcap.newton.decode.at_index"), i), "process");
				auto group_has_parent = stream.readBoolean();
				group["id"] = stream.readString(static_cast<size_t>(stream.readUint32()));
				if (group_has_parent) {
					group["parent"] = stream.readString(static_cast<size_t>(stream.readUint32()));
				}
				if (group_type == 0x01) {
					process_composite_group(stream, group, subgroups_count);
				} else if (group_type == 0x02) {
					process_simple_group(stream, group, resources_count);
				}
				groups.emplace_back(std::move(group));
			}
			result["groups"] = std::move(groups);
		}

		inline static auto process_composite_group(
			DataStreamView& stream, 
			nlohmann::ordered_json& group, 
			uint32_t subgroups_count
		) -> void {
			assert_conditional(stream.readInt32() == 0x00, fmt::format("{}, id: {}", Kernel::Language::get("popcap.newton.decode.resource_must_be_null_with_composite"), group["id"].get<std::string>()), "process_composite_group");
			auto subgroups = nlohmann::ordered_json::array_t{};
			subgroups.reserve(subgroups_count);
			for (auto index : Range(subgroups_count)) {
				auto subgroup = nlohmann::ordered_json{};
				auto sub_res = stream.readUint32();
				if (sub_res != 0x00) {
					subgroup["res"] = fmt::format("{}", sub_res);
				}
				subgroup["id"] = stream.readString(static_cast<size_t>(stream.readUint32()));
				subgroups.emplace_back(std::move(subgroup));
			}
			group["subgroups"] = std::move(subgroups);
		}

		inline static auto process_simple_group(
			DataStreamView& stream, 
			nlohmann::ordered_json& group, 
			int32_t resources_count
		) -> void {
			assert_conditional(stream.readUint32() == 0x00, fmt::format("{}, id: {}", Kernel::Language::get("popcap.newton.decode.subgroup_must_be_null_with_simple"), group["id"].get<std::string>()), "process_simple_group");
			auto resources = nlohmann::ordered_json::array_t{};
			resources.reserve(resources_count);
			for (auto index : Range<int>(resources_count)) {
				auto resource = nlohmann::ordered_json{};
				auto result = std::string{};
				read_resource_type(stream.readUint8(), group["id"].get<std::string>(), result);
				resource["type"] = std::move(result);
				populate_resource_details(stream, resource);
				resources.emplace_back(std::move(resource));
			}
			group["resources"] = std::move(resources);
		}

		inline static auto populate_resource_details(
			DataStreamView& stream, 
			nlohmann::ordered_json& resource
		) -> void {
			resource["slot"] = stream.readUint32();
			auto width = stream.readInt32();
			auto height = stream.readInt32();
			auto ax = stream.readInt32();
			auto ay = stream.readInt32();
			auto aw = stream.readInt32();
			auto ah = stream.readInt32();
			auto atlas = stream.readBoolean();
			if (atlas) {
				resource["width"] = width;
				resource["height"] = height;
				resource["atlas"] = true;
				resource["runtime"] = true;
			}
			if (aw != 0x00 && ah != 0x00) {
				resource["ax"] = ax;
				resource["ay"] = ay;
				resource["aw"] = aw;
				resource["ah"] = ah;
			}
			resource["id"] = stream.readString(static_cast<size_t>(stream.readUint32()));
			resource["path"] = stream.readString(static_cast<size_t>(stream.readUint32()));
			if (stream.readBoolean()) {
				resource["parent"] = stream.readString(static_cast<size_t>(stream.readUint32()));
			}
		}

		inline static auto process_fs(
			std::string_view source, 
			std::string_view destination
		) -> void {
			auto stream_view = DataStreamView{source};
			auto result = nlohmann::ordered_json{};
			process(stream_view, result);
			FileSystem::write_json(destination, result);
		}
	};
}