#pragma once

#include "kernel/support/popcap/resource_group/common.hpp"

namespace Sen::Kernel::Support::PopCap::ResourceGroup { 

	template <auto use_string_for_style>
	struct Convert : public Common {

		private:

			static_assert(sizeof(use_string_for_style) == sizeof(bool));
			static_assert(use_string_for_style == true || use_string_for_style == false);

			inline static auto gather_atlas_elements(
				const nlohmann::ordered_json& subgroup
			) -> List<const nlohmann::ordered_json*> {
				auto atlas = List<const nlohmann::ordered_json*>{};
				for (const auto& element : subgroup["resources"]) {
					if (element.contains("atlas") && element["atlas"].get<bool>()) {
						atlas.emplace_back(&element);
					}
				}
				return atlas;
			}

			inline static auto process_children_for_parent(
				const nlohmann::ordered_json& subgroup,
				const nlohmann::ordered_json& parent
			) -> nlohmann::ordered_json {
				auto children_data = nlohmann::ordered_json{};
				auto children_in_parent = List<const nlohmann::ordered_json*>{};
				for (const auto& element : subgroup["resources"]) {
					if (element.contains("parent") && element["parent"].get<std::string>() == parent["id"].get<std::string>()) {
						children_in_parent.emplace_back(&element);
					}
				}
				for (const auto* element : children_in_parent) {
					auto child = nlohmann::ordered_json{};
					if constexpr (use_string_for_style) {
						child = {
							{"type", (*element)["type"].get<std::string>()},
							{"path", String::replaceAll((*element)["path"].get<std::string>(), Common::WindowStyle, Common::PosixStyle)},
							{"default", {
								{"ax", (*element)["ax"].get<int>()},
								{"ay", (*element)["ay"].get<int>()},
								{"aw", (*element)["aw"].get<int>()},
								{"ah", (*element)["ah"].get<int>()}
							}}
						};
					} else {
						child = {
							{"type", (*element)["type"].get<std::string>()},
							{"path", String::join((*element)["path"].get<List<std::string>>(), Common::PosixStyle)},
							{"default", {
								{"ax", (*element)["ax"].get<int>()},
								{"ay", (*element)["ay"].get<int>()},
								{"aw", (*element)["aw"].get<int>()},
								{"ah", (*element)["ah"].get<int>()}
							}}
						};
					}
					if (element->contains("x") && (*element)["x"] != Common::DefaultCoordinateOffset) {
						child["default"]["x"] = (*element)["x"].get<int>();
					} else {
						child["default"]["x"] = Common::DefaultCoordinateOffset;
					}
					if (element->contains("y") && (*element)["y"] != Common::DefaultCoordinateOffset) {
						child["default"]["y"] = (*element)["y"].get<int>();
					} else {
						child["default"]["y"] = Common::DefaultCoordinateOffset;
					}
					if (element->contains("rows") && (*element)["rows"] != Common::DefaultLayoutOffset) {
						child["default"]["rows"] = (*element)["rows"].get<int>();
					}
					if (element->contains("cols") && (*element)["cols"] != Common::DefaultLayoutOffset) {
						child["default"]["cols"] = (*element)["cols"].get<int>();
					}
					children_data[(*element)["id"].get<std::string>()] = std::move(child);
				}
				return children_data;
			}

			inline static auto convert_atlas(
				const nlohmann::ordered_json& subgroup
			) -> nlohmann::ordered_json {
				auto result = nlohmann::ordered_json{{"type", subgroup["res"]}};
				auto atlas = gather_atlas_elements(subgroup);
				for (const auto* parent : atlas) {
					auto atlas_data = nlohmann::ordered_json{
						{"type", (*parent)["type"].get<std::string>()},
						{"path", use_string_for_style
									? String::replaceAll((*parent)["path"].get<std::string>(), Common::WindowStyle, Common::PosixStyle)
									: String::join((*parent)["path"].get<List<std::string>>(), Common::PosixStyle)},
						{"dimension", {
							{"width", (*parent)["width"].get<int>()},
							{"height", (*parent)["height"].get<int>()}
						}}
					};
					atlas_data["data"] = process_children_for_parent(subgroup, *parent);
					result["packet"][(*parent)["id"].get<std::string>()] = std::move(atlas_data);
				}

				return result;
			}

			inline static auto process_resource(
				const nlohmann::ordered_json& element
			) -> nlohmann::ordered_json {
				if constexpr (use_string_for_style) {
					return {
						{"type", element["type"].get<std::string>()},
						{"path", String::replaceAll(element["path"].get<std::string>(), Common::WindowStyle, Common::PosixStyle)}
					};
				} else {
					return {
						{"type", element["type"].get<std::string>()},
						{"path", String::join(element["path"].get<List<std::string>>(), Common::PosixStyle)}
					};
				}
			}

			inline static auto convert_common(
				const nlohmann::ordered_json& subgroup
			) -> nlohmann::ordered_json {
				auto result = nlohmann::ordered_json{{"type", nullptr}};
				if (subgroup.contains("loc")) {
					result["loc"] = subgroup["loc"];
				}
				result["packet"] = {{"type", "File"}};
				auto& data = result["packet"]["data"];
				for (const auto& element : subgroup["resources"]) {
					data[element["id"].get<std::string>()] = process_resource(element);
				}

				return result;
			}

			inline static auto first_where(
				const nlohmann::ordered_json& resource_group, 
				const std::string& id
			) -> const nlohmann::ordered_json& {
				for (const auto& element : resource_group["groups"]) {
					if (element["id"] == id) {
						return element;
					}
				}
				assert_conditional(false, String::format(fmt::format("{}", Language::get("popcap.resource_group.convert.cannot_find_id")), id), "first_where");
			}

		public:

			constexpr Convert(

			) = default;

			constexpr ~Convert(

			) = default;

			inline static auto process_whole(
				const nlohmann::ordered_json& resource_group
			) -> nlohmann::ordered_json 
			{
				assert_conditional(resource_group.find("groups") != resource_group.end(), fmt::format("{}", Language::get("popcap.resource_group.group_cannot_be_null_in_resource_group")), "process_whole");
				auto result = nlohmann::ordered_json{{"expand_path", use_string_for_style ? Common::String : Common::Array}};
				for (const auto& element : resource_group["groups"]) {
					if (element.find("subgroups") != element.end()) {
						auto subgroup = nlohmann::ordered_json{{"is_composite", true}};
						for (const auto& k : element["subgroups"]) {
							if (k.find("res") != k.end() && k["res"].get<std::string>() != "0") {
								subgroup["subgroup"][k["id"].get<std::string>()] = convert_atlas(first_where(resource_group, k["id"]));
							} else {
								subgroup["subgroup"][k["id"].get<std::string>()] = convert_common(first_where(resource_group, k["id"]));
							}
						}
						result["groups"][element["id"].get<std::string>()] = std::move(subgroup);
					}
					if (element.find("parent") == element.end() && element.find("resources") != element.end()) {
						auto subgroup = nlohmann::ordered_json{{"is_composite", false}};
						subgroup["subgroup"][element["id"].get<std::string>()] = convert_common(element);
						result["groups"][element["id"].get<std::string>()] = std::move(subgroup);
					}
				}

				return result;
			}

			inline static auto process_fs(
				std::string_view source, 
				std::string_view destination
			) -> void
			{
				auto resource_group = FileSystem::read_json(source);
				FileSystem::write_json(destination, process_whole(resource_group));
				return;
			}
	};
}