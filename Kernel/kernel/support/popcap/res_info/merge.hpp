#pragma once

#include "kernel/support/popcap/res_info/common.hpp"

namespace Sen::Kernel::Support::PopCap::ResInfo {

	struct Merge : public Common {

		inline static auto process(
			std::string_view source,
			std::string_view destination
		) -> void {
			auto info = FileSystem::read_json(fmt::format("{}/{}", source, "data.json"));
			auto res_info = nlohmann::ordered_json{
				{"expand_path", info["information"]["expand_path"]}
			};
			for (auto& [group_name, group_value] : info["groups"].items()) {
				auto subgroups = nlohmann::ordered_json{
					{"is_composite", group_value["is_composite"]}
				};
				for (auto& subgroup_name : group_value["subgroups"]) {
					auto subgroup_string = subgroup_name.get<std::string>();
					subgroups["subgroup"][subgroup_string] = FileSystem::read_json(Path::normalize(fmt::format("{}/groups/{}.json", source, subgroup_string)));
				}
				res_info["groups"][group_name] = subgroups;
			}
			FileSystem::write_json(destination, res_info);
		}

		inline static auto process_fs(
			std::string_view source,
			std::string_view destination
		) -> void {
			return Merge::process(source, destination);
		}
	};
}