#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/resource_group/convert.hpp"
#include "kernel/support/popcap/resource_group/common.hpp"

namespace Sen::Kernel::Support::PopCap::ResInfo {

	using namespace ResourceGroup;

    struct Convert : public ResourceGroup::Common {

        struct SubInformation {

            std::string_view id;

            std::string_view parent;

            explicit SubInformation(

            ) = delete;

            explicit SubInformation(
                std::string_view id,
                std::string_view parent
            ) : id(id), parent(parent)
            {

            }

            constexpr ~SubInformation(

            ) = default;
        };

        inline static auto constexpr Composite = std::string_view{ "composite" };

        inline static auto constexpr Simple = std::string_view{ "simple" };

        inline static auto generate_composite(
            std::string_view id,
            const nlohmann::ordered_json& composite
        ) -> nlohmann::ordered_json
        {
            auto result = nlohmann::ordered_json{
                { "type", Convert::Composite },
                { "id", id },
                { "subgroups", nlohmann::ordered_json::array() }
            };
            for (auto& [element, value] : composite["subgroup"].items()) {
                auto subgroup = nlohmann::ordered_json{
                    {"id", element}
                };
                if (!composite["subgroup"][element]["type"].is_null()) {
                    subgroup["res"] = composite["subgroup"][element]["type"].get<std::string_view>();
                }
                if (composite["subgroup"][element].find("loc") != composite["subgroup"][element].end()) {
                    subgroup["loc"] = composite["subgroup"][element]["loc"];
                }
                result["subgroups"].emplace_back(subgroup);
            }
            return result;
        }

        template <auto use_string_for_style>
        inline static auto generate_common(
            const SubInformation& extra_information,
            const nlohmann::ordered_json& resource_information
        ) -> nlohmann::ordered_json
        {
            static_assert(use_string_for_style == true || use_string_for_style == false);
            static_assert(sizeof(use_string_for_style) == sizeof(bool));
            auto result = nlohmann::ordered_json{
                {"type", Simple},
                {"id", extra_information.id}
            };
            if (resource_information.find("loc") != resource_information.end()) {
                result["loc"] = resource_information["loc"];
            }
            result["resources"] = nlohmann::ordered_json::array();
            if (!extra_information.parent.empty()) {
                result["parent"] = extra_information.parent;
            }
            for (auto& [key, value] : resource_information["packet"]["data"].items()) {
                auto resource = nlohmann::ordered_json{
                    {"type", value["type"].get<std::string>()},
                    {"slot", 0},
                    {"id", key}
                };
                if constexpr (use_string_for_style) {
                    resource["path"] = String::replaceAll(value["path"].get<std::string>(), ResourceGroup::Common::PosixStyle, ResourceGroup::Common::WindowStyle);
                }
                else {
                    resource["path"] = String::split(value["path"].get<std::string>(), ResourceGroup::Common::PosixStyle);
                }
                result["resources"].emplace_back(resource);
            }
            return result;
        }

        template <auto use_string_for_style>
        inline static auto generate_image(
            const SubInformation& extra_information,
            const nlohmann::ordered_json& resource_information
        ) -> nlohmann::ordered_json
        {
            static_assert(use_string_for_style == true || use_string_for_style == false);
            static_assert(sizeof(use_string_for_style) == sizeof(bool));
            auto result = nlohmann::ordered_json{
                {"type", Simple},
                {"id", extra_information.id},
                {"res", resource_information["type"].get<std::string>()},
                {"parent", extra_information.parent},
                {"resources", nlohmann::ordered_json::array()}
            };
            for (auto& [key, value] : resource_information["packet"].items()) {
                auto resource = nlohmann::ordered_json{
                    {"type", value["type"].get<std::string>()},
                    {"slot", 0},
                    {"id", key},
                    {"atlas", true},
                    {"runtime", true},
                    {"width", value["dimension"]["width"].get<int>()},
                    {"height", value["dimension"]["height"].get<int>()}
                };
                if constexpr (use_string_for_style) {
                    resource["path"] = String::replaceAll(value["path"].get<std::string>(), ResourceGroup::Common::PosixStyle, ResourceGroup::Common::WindowStyle);
                }
                else {
                    resource["path"] = String::split(value["path"].get<std::string>(), Common::PosixStyle);
                }
                result["resources"].emplace_back(resource);
                for (auto& [sub, sub_value] : value["data"].items()) {
                    auto sub_resource = nlohmann::ordered_json{
                        {"type", sub_value["type"].get<std::string>()},
                        {"slot", 0},
                        {"id", sub},
                        {"parent", key}
                    };
                    if constexpr (use_string_for_style) {
                        sub_resource["path"] = String::replaceAll(sub_value["path"].get<std::string>(), Common::PosixStyle, ResourceGroup::Common::WindowStyle);
                    }
                    else {
                        sub_resource["path"] = String::split(sub_value["path"].get<std::string>(), Common::PosixStyle);
                    }
                    if (sub_value["default"].find("x") != sub_value["default"].end() && sub_value["default"]["x"].get<int>() != ResourceGroup::Common::DefaultCoordinateOffset) {
                        sub_resource["x"] = sub_value["default"]["x"].get<int>();
                    }
                    if (sub_value["default"].find("y") != sub_value["default"].end() && sub_value["default"]["y"].get<int>() != ResourceGroup::Common::DefaultCoordinateOffset) {
                        sub_resource["y"] = sub_value["default"]["y"].get<int>();
                    }
                    if (sub_value["default"].find("rows") != sub_value["default"].end() && sub_value["default"]["rows"].get<int>() != ResourceGroup::Common::DefaultLayoutOffset) {
                        sub_resource["rows"] = sub_value["default"]["rows"].get<int>();
                    }
                    if (sub_value["default"].find("cols") != sub_value["default"].end() && sub_value["default"]["cols"].get<int>() != ResourceGroup::Common::DefaultLayoutOffset) {
                        sub_resource["cols"] = sub_value["default"]["cols"].get<int>();
                    }
                    sub_resource["ax"] = sub_value["default"]["ax"].get<int>();
                    sub_resource["ay"] = sub_value["default"]["ay"].get<int>();
                    sub_resource["aw"] = sub_value["default"]["aw"].get<int>();
                    sub_resource["ah"] = sub_value["default"]["ah"].get<int>();
                    result["resources"].emplace_back(sub_resource);
                }
            }
            return result;
        }

        inline static auto process(
            const nlohmann::ordered_json& res_info,
            nlohmann::ordered_json& result
        ) -> void
        {
            assert_conditional(res_info.find("expand_path") != res_info.end(), fmt::format("{}", Kernel::Language::get("popcap.res_info.convert.expand_path_is_null_in_res_info")), "process");
            assert_conditional(res_info.find("groups") != res_info.end(), fmt::format("{}", Kernel::Language::get("popcap.res_info.convert.groups_is_null_in_res_info")), "process");
            bool use_string_for_style = hash_string(res_info["expand_path"].get<std::string>()) == hash_string("string"_sv);
            for (auto& [composite_name, group] : res_info["groups"].items()) {
                if (group["is_composite"].get<bool>()) {
                    result["groups"].emplace_back(generate_composite(composite_name, group));
                    for (auto& [subgroup_name, subgroup_value] : group["subgroup"].items()) {
                        if (!subgroup_value["type"].is_null()) {
                            if (use_string_for_style) {
                                result["groups"].emplace_back(generate_image<true>(SubInformation(subgroup_name, composite_name), subgroup_value));
                            }
                            else {
                                result["groups"].emplace_back(generate_image<false>(SubInformation(subgroup_name, composite_name), subgroup_value));
                            }
                        }
                        else {
                            if (use_string_for_style) {
                                result["groups"].emplace_back(generate_common<true>(SubInformation(subgroup_name, composite_name), subgroup_value));
                            }
                            else {
                                result["groups"].emplace_back(generate_common<false>(SubInformation(subgroup_name, composite_name), subgroup_value));
                            }
                        }
                    }
                }
                else {
                    for (auto& [subgroup_name, subgroup_value] : group["subgroup"].items()) {
                        if (use_string_for_style) {
                            result["groups"].emplace_back(generate_common<true>(SubInformation{ subgroup_name, Convert::emptyString }, subgroup_value));
                        }
                        else {
                            result["groups"].emplace_back(generate_common<false>(SubInformation{ subgroup_name, Convert::emptyString }, subgroup_value));
                        }
                    }
                }
            }
            Convert::rewrite_slot_count(result);
            return;
        }

        inline static auto process_whole(
            const nlohmann::ordered_json& res_info,
            nlohmann::ordered_json& destination
        ) -> void
        {
            return process(res_info, destination);
        }

        inline static auto process_fs(
            std::string_view source,
            std::string_view destination
        ) -> void
        {
            auto result = nlohmann::ordered_json{
                {"version", 1},
                {"content_version", 1},
                {"slot_count", 0},
                {"groups", nlohmann::ordered_json::array()}
            };
            Convert::process_whole(FileSystem::read_json(source), result);
            FileSystem::write_json(destination, result);
            return;
        }
    };

};