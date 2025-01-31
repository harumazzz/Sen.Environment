#pragma once

#include "kernel/support/popcap/resource_group/common.hpp"

namespace Sen::Kernel::Support::PopCap::ResourceGroup {

	struct Merge : public Common {

		constexpr Merge(

		) = default;

		constexpr ~Merge(

		) = default;

		inline static auto create_resources_json(

		) -> nlohmann::ordered_json {
			return nlohmann::ordered_json{
				{"version", 1},
				{"content_version", 1},
				{"slot_count", 0}
			};
		}

		inline static auto process_composite_group(
			const std::string& parent,
			const nlohmann::ordered_json& parent_value
		) -> nlohmann::ordered_json {
			auto composite_object = nlohmann::ordered_json{
				{"id", parent},
				{"type", "composite"},
				{"subgroups", nlohmann::ordered_json::array()}
			};
			for (const auto& [subgroup, subgroup_value] : parent_value["subgroups"].items()) {
				auto resource_for_subgroup = nlohmann::ordered_json{{"id", subgroup}};
				if (!subgroup_value["type"].is_null()) {
					resource_for_subgroup["res"] = subgroup_value["type"];
				}
				if (subgroup_value.find("loc") != subgroup_value.end()) {
					resource_for_subgroup["loc"] = subgroup_value["loc"];
				}
				composite_object["subgroups"].emplace_back(std::move(resource_for_subgroup));
			}
			return composite_object;
		}

		inline static auto process_subgroup_resources(
			std::string_view source,
			const std::string& parent,
			const nlohmann::ordered_json& parent_value
		) -> nlohmann::ordered_json::array_t {
			nlohmann::ordered_json::array_t resources;
			for (const auto& [subgroup, subgroup_value] : parent_value["subgroups"].items()) {
				auto resource_json_path = fmt::format("{}/subgroup/{}.json", source, subgroup);
				auto resource_content = FileSystem::read_json(resource_json_path);
				assert_conditional(resource_content.find("resources") != resource_content.end(), format(fmt::format("{}", Language::get("popcap.resource_group.property_cannot_be_null")), std::string{"groups"}), "process_subgroup_resources");
				resources.emplace_back(std::move(resource_content));
			}
			return resources;
		}

		inline static auto collect_groups(
			std::string_view source,
			const nlohmann::ordered_json& content
		) -> nlohmann::ordered_json::array_t {
			nlohmann::ordered_json::array_t groups;
			groups.reserve(content.size());

			for (const auto& [parent, parent_value] : content.items()) {
				if (parent_value["is_composite"].get<bool>()) {
					groups.emplace_back(process_composite_group(parent, parent_value));
				}
				auto subgroup_resources = process_subgroup_resources(source, parent, parent_value);
				groups.insert(groups.end(), std::make_move_iterator(subgroup_resources.begin()), std::make_move_iterator(subgroup_resources.end()));
			}

			return groups;
		}

		inline static auto write_resources_json(
			std::string_view destination,
			nlohmann::ordered_json& resources_json
		) -> void {
			rewrite_slot_count(resources_json);  
			FileSystem::write_json(destination, resources_json);
		}

		inline static auto process(
			std::string_view source,
			std::string_view destination,
			const nlohmann::ordered_json& content
		) -> void {
			auto resources_json = create_resources_json();
			resources_json["groups"] = collect_groups(source, content);
			write_resources_json(destination, resources_json);
		}

		inline static auto process_fs(
			std::string_view source,
			std::string_view destination
		) -> void {
			auto content = FileSystem::read_json(fmt::format("{}/{}", source, "data.json"));
			process(source, destination, content);
		}

	};

}