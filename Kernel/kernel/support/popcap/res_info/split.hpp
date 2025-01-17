#pragma once

#include "kernel/support/popcap/res_info/common.hpp"

namespace Sen::Kernel::Support::PopCap::ResInfo {

	struct Split : public Common {

		inline static auto generate_subgroup(
			const nlohmann::ordered_json& resource
		) -> nlohmann::ordered_json {
			return nlohmann::ordered_json{
				{"is_composite", resource["is_composite"]},
				{"subgroups", Object::keys(resource["subgroup"])}
			};
		}

		inline static auto convert_info(
			const nlohmann::ordered_json& resource
		) -> nlohmann::ordered_json {
			auto result = nlohmann::ordered_json{
				{"information", nlohmann::ordered_json{
					{"expand_path", resource["expand_path"]}
				}}
			};
			auto group = nlohmann::ordered_json{};
			auto group_key = Object::keys(resource["groups"]);
			for (auto i : Range<size_t>(group_key.size())) {
				group[group_key[i]] = Split::generate_subgroup(resource["groups"][group_key[i]]);
			}
			result["groups"] = group;
			return result;
		}

		inline static auto process(
			std::string_view source,
			std::string_view destination
		) -> void {
			auto res_info = FileSystem::read_json(source);
			auto info = Split::convert_info(res_info);
			auto group_directory = Path::normalize(fmt::format("{}/{}", destination, "groups"));
			FileSystem::create_directory(group_directory);
			FileSystem::write_json(Path::normalize(fmt::format("{}/{}", destination, "data.json")), info);
			auto keys = Object::keys(res_info["groups"]);
			for (auto i : Range<size_t>(keys.size())) {
				auto subgroup_key = Object::keys(res_info["groups"][keys.at(i)]["subgroup"]);
				for (auto j : Range<size_t>(subgroup_key.size())) {
					FileSystem::write_json(Path::normalize(fmt::format("{}/{}.json", group_directory, subgroup_key.at(j))),
						res_info["groups"][keys.at(i)]["subgroup"][subgroup_key.at(j)]);
				}
			}
		}

		inline static auto process_fs(
			std::string_view source,
			std::string_view destination
		) -> void {
			Split::process(source, destination);
		}
	};
}