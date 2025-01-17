#pragma once

#include "kernel/support/popcap/resource_group/common.hpp"

namespace Sen::Kernel::Support::PopCap::ResourceGroup {

	struct Split : public Common {
        
        constexpr Split(

        ) = default;

        constexpr ~Split(

        ) = default;

        inline static auto remove_slot(
            nlohmann::ordered_json& resource
        ) -> void {
            for (auto& e : resource["resources"]) {
                e.erase("slot");
            }
        }

        inline static auto write_subgroup(
            nlohmann::ordered_json& content, 
            nlohmann::ordered_json& resource
        ) -> void {
            const auto& resource_id = resource["id"].get<std::string>();
            content[resource_id]["is_composite"] = true;

            if (resource.contains("subgroups")) {
                for (auto& e : resource["subgroups"]) {
                    const auto& subgroup_id = e["id"].get<std::string>();
                    content[resource_id]["subgroups"][subgroup_id] = nlohmann::ordered_json{{"type", e["res"]}};
                    if (e.contains("loc")) {
                        content[resource_id]["subgroups"][subgroup_id]["loc"] = e["loc"];
                    }
                }
            }
        }

        inline static auto write_composite(
            nlohmann::ordered_json& content, 
            nlohmann::ordered_json& resource
        ) -> void {
            const auto& resource_id = resource["id"].get<std::string>();
            content[resource_id] = nlohmann::ordered_json{
                {"is_composite", false},
                {"subgroups", nlohmann::ordered_json{
                    {resource_id, nlohmann::ordered_json{{"type", nullptr}}}
                }}
            };
        }

        inline static auto process(
            nlohmann::ordered_json& resource, 
            std::string_view destination
        ) -> void {
            assert_conditional(!resource["groups"].is_null(), fmt::format("{}", Language::get("popcap.resource_group.split.groups_cannot_be_null")), "process");
            FileSystem::create_directory(fmt::format("{}/{}", destination, "subgroup"));
            auto content = nlohmann::ordered_json{};
            auto futures = List<std::future<void>>{};
            futures.reserve(resource["groups"].size());
            for (auto& c : resource["groups"]) {
                const auto& resource_id = c["id"].get<std::string>();
                auto has_resource = c.find("resources") != c.end();
                if (has_resource) {
                    remove_slot(c);
                }
                auto has_parent = c.find("parent") == c.end();
                if (has_resource && has_parent) {
                    futures.push_back(std::async(std::launch::async, [destination, resource_id, &c]() {
                        FileSystem::write_json(fmt::format("{}/subgroup/{}.json", destination, resource_id), c);
                    }));
                }
                auto has_subgroup = c.find("subgroups") != c.end();
                if (has_subgroup || (has_resource && !has_parent)) {
                    if (has_subgroup) {
                        write_subgroup(content, c);
                    } else {
                        write_composite(content, c);
                        futures.push_back(std::async(std::launch::async, [destination, resource_id, &c]() {
                            FileSystem::write_json(fmt::format("{}/subgroup/{}.json", destination, resource_id), c);
                        }));
                    }
                }
            }
            std::for_each(futures.begin(), futures.end(), [](auto& e){
                e.get();
            });
            FileSystem::write_json(fmt::format("{}/data.json", destination), content);
        }

        inline static auto process_fs(
            std::string_view source, 
            std::string_view destination
        ) -> void {
            auto resource = FileSystem::read_json(source);
            return process(resource, destination);
        }
    };


}