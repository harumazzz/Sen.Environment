#pragma once

#include "kernel/definition/utility.hpp"
#include "kernel/support/miscellaneous/custom/stream_compressed_group/definition.hpp"

namespace Sen::Kernel::Support::Miscellaneous::Custom::ResourceStreamBundle
{
    using TextureInformationVersion = Sen::Kernel::Support::PopCap::ResourceStreamBundle::Common::TextureInformationVersion;

    using TextureFormatCategory = Sen::Kernel::Support::Miscellaneous::Custom::StreamCompressedGroup::TextureFormatCategory;

    struct PackagesSetting
    {
        int rton_count;
        int json_count;
        std::string key;
        std::string iv;
    };

    inline auto to_json(
        nlohmann::ordered_json &nlohmann_json_j,
        const PackagesSetting &nlohmann_json_t) -> void
    {
        nlohmann_json_j["rton_count"] = nlohmann_json_t.rton_count;
        nlohmann_json_j["json_count"] = nlohmann_json_t.json_count;
        nlohmann_json_j["key"] = nlohmann_json_t.key;
        nlohmann_json_j["iv"] = nlohmann_json_t.iv;
        return;
    }

    inline auto from_json(
        const nlohmann::ordered_json &nlohmann_json_j,
        PackagesSetting &nlohmann_json_t) -> void
    {
        nlohmann_json_j.at("rton_count").get_to(nlohmann_json_t.rton_count);
        nlohmann_json_j.at("json_count").get_to(nlohmann_json_t.json_count);
        nlohmann_json_j.at("key").get_to(nlohmann_json_t.key);
        nlohmann_json_j.at("iv").get_to(nlohmann_json_t.iv);
        return;
    }

    struct Setting
    {
        TextureFormatCategory texture_format_category;
        bool only_high_resolution;
        bool unpack_packages{true};
        PackagesSetting packages_setting;
    };

    inline auto to_json(
        nlohmann::ordered_json &nlohmann_json_j,
        const Setting &nlohmann_json_t) -> void
    {
        nlohmann_json_j["texture_format_category"] = nlohmann_json_t.texture_format_category;
        nlohmann_json_j["only_high_resolution"] = nlohmann_json_t.only_high_resolution;
        nlohmann_json_j["unpack_packages"] = nlohmann_json_t.unpack_packages;
        nlohmann_json_j["packages_setting"] = nlohmann_json_t.packages_setting;
        return;
    }

    inline auto from_json(
        const nlohmann::ordered_json &nlohmann_json_j,
        Setting &nlohmann_json_t) -> void
    {
        nlohmann_json_j.at("texture_format_category").get_to(nlohmann_json_t.texture_format_category);
        nlohmann_json_j.at("only_high_resolution").get_to(nlohmann_json_t.only_high_resolution);
        nlohmann_json_j.at("unpack_packages").get_to(nlohmann_json_t.unpack_packages);
        nlohmann_json_j.at("packages_setting").get_to(nlohmann_json_t.packages_setting);
        return;
    }

    // -------------------------------------------------------------------------

    enum ExpandPath
    {
        String, 
        Array
    };

    struct ManifestGroupInfo 
    {
      //  ExpandPath expand_path;
        uint32_t compression;
        bool allow_new_type_resource;
        std::string resource_additional_name;
    };

    inline auto to_json(
        nlohmann::ordered_json &nlohmann_json_j,
        const ManifestGroupInfo &nlohmann_json_t) -> void
    {
        //nlohmann_json_j["expand_path"] = nlohmann_json_t.expand_path == String ? "string" : "array";
        nlohmann_json_j["compression"] = nlohmann_json_t.compression;
        nlohmann_json_j["allow_new_type_resource"] = nlohmann_json_t.allow_new_type_resource;
        if (nlohmann_json_t.resource_additional_name.empty())
        {
            nlohmann_json_j["resource_additional_name"] = nullptr;
        }
        else
        {
            nlohmann_json_j["resource_additional_name"] = nlohmann_json_t.resource_additional_name;
        }
        return;
    }

    inline auto from_json(
        const nlohmann::ordered_json &nlohmann_json_j,
        ManifestGroupInfo &nlohmann_json_t) -> void
    {
       // auto expand_path_string = nlohmann_json_j["expand_path"].get<std::string>();
       // assert_conditional(!(expand_path_string != "string" && expand_path_string != "array"), String::format(fmt::format("{}", Language::get("pvz2.rsb.modding.invalid_expand_path")), expand_path_string), "from_json");
       // nlohmann_json_t.expand_path = nlohmann_json_t.allow_new_type_resource ? ExpandPath::String : ExpandPath::Array;
        nlohmann_json_j.at("compression").get_to(nlohmann_json_t.compression);
        nlohmann_json_j.at("allow_new_type_resource").get_to(nlohmann_json_t.allow_new_type_resource);
        if (nlohmann_json_j["resource_additional_name"] != nullptr)
        {
            nlohmann_json_j.at("resource_additional_name").get_to(nlohmann_json_t.resource_additional_name);
        }
        return;
    }

    struct PackagesInfo {
        bool is_contain_packages;
        uint32_t compression;
        bool chinese;
        bool encode;
    };  

    inline auto to_json(
        nlohmann::ordered_json &nlohmann_json_j,
        const PackagesInfo &nlohmann_json_t) -> void
    {
        nlohmann_json_j["compression"] = nlohmann_json_t.compression;
        nlohmann_json_j["chinese"] = nlohmann_json_t.chinese;
        nlohmann_json_j["encode"] = nlohmann_json_t.encode;
        return;
    }

    inline auto from_json(
        const nlohmann::ordered_json &nlohmann_json_j,
        PackagesInfo &nlohmann_json_t) -> void
    {
        nlohmann_json_j.at("compression").get_to(nlohmann_json_t.compression);
        nlohmann_json_j.at("chinese").get_to(nlohmann_json_t.chinese);
        nlohmann_json_j.at("encode").get_to(nlohmann_json_t.encode);
        return;
    }

    struct InformationStructure {
        uint32_t version;
        TextureInformationVersion texture_information_version;
        // TextureFormatCategory texture_format_category;
        PackagesInfo packages_info;
        ManifestGroupInfo manifest_info;
        List<std::string> packet;
    };

    inline auto to_json(
        nlohmann::ordered_json &nlohmann_json_j,
        const InformationStructure &nlohmann_json_t) -> void
    {
        nlohmann_json_j["version"] = nlohmann_json_t.version;
        nlohmann_json_j["texture_information_version"] = nlohmann_json_t.texture_information_version;
        if (nlohmann_json_t.packages_info.is_contain_packages) {
            nlohmann_json_j["packages_info"] = nlohmann_json_t.packages_info;
        }
        else {
            nlohmann_json_j["packages_info"] = nullptr;
        }
        nlohmann_json_j["manifest_info"] = nlohmann_json_t.manifest_info;
        nlohmann_json_j["packet"] = nlohmann_json_t.packet;
        return;
    }

    inline auto from_json(
        const nlohmann::ordered_json &nlohmann_json_j,
        InformationStructure &nlohmann_json_t) -> void
    {
        nlohmann_json_j.at("version").get_to(nlohmann_json_t.version);
        nlohmann_json_j.at("texture_information_version").get_to(nlohmann_json_t.texture_information_version);
        if (nlohmann_json_j.at("packages_info") != nullptr) {
            nlohmann_json_t.packages_info.is_contain_packages = true;
            nlohmann_json_j.at("packages_info").get_to(nlohmann_json_t.packages_info);
        }
        nlohmann_json_j.at("manifest_info").get_to(nlohmann_json_t.manifest_info);
        nlohmann_json_j.at("packet").get_to(nlohmann_json_t.packet);
        return;
    }
}