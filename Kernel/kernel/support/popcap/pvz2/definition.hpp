#pragma once

#include "kernel/definition/utility.hpp"

namespace Sen::Kernel::Support::PopCap::PvZ2
{
    inline static auto constexpr data_magic = "CNES"_sv;

    enum DataType
    {
        File,            // 0
        Image,           // 1 - ptx
        PopAnim,         // 2 - pam
        Data,            // 3 - rton
        SoundBank,       // 4 - bank
        DecodedSoundBank, // 5 - bank
        PrimeFont,       // 6 - font
        RenderEffect     // 7 effect
    };

    enum class CompressionFlag : uint8_t
    {
        NO_COMPRESSION,
        DEFAULT_COMPRESSION,
        ATLAS_COMPRESSION,
        BEST_COMPRESSION,
    };

    struct DataPTXinfo
    {
    public:
        int x = 0;
        int y = 0;
        explicit DataPTXinfo(

            ) = default;

        explicit DataPTXinfo(
            int x, int y) : x(x), y(y)
        {
        }
    };

    inline auto static to_json(
        nlohmann::ordered_json &nlohmann_json_j,
        const DataPTXinfo &nlohmann_json_t) -> void
    {
        nlohmann_json_j["x"] = nlohmann_json_t.x;
        nlohmann_json_j["y"] = nlohmann_json_t.y;
        return;
    }

    inline auto static from_json(
        const nlohmann::ordered_json &nlohmann_json_j,
        DataPTXinfo &nlohmann_json_t) -> void
    {
        nlohmann_json_j.at("x").get_to(nlohmann_json_t.x);
        nlohmann_json_j.at("y").get_to(nlohmann_json_t.y);
        return;
    };

    struct DataResInfo
    {
    public:
        DataType type;
        // std::string name;
        std::string path;
        DataPTXinfo ptx_default_info;
        explicit DataResInfo(

            ) = default;

        explicit DataResInfo(
            DataType type, const std::string &path) : type(type), path(path)
        {
        }
    };

    inline auto static type_to_string(
        DataType type) -> std::string
    {
        switch (type)
        {
        case Image:
        {
            return "Image";
        }
        case PopAnim:
        {
            return "PopAnim";
        }
        case Data:
        {
            return "Data";
        }
        case SoundBank:
        {
            return "SoundBank";
        }
        case DecodedSoundBank:
        {
            return "DecodedSoundBank";
        }
        case PrimeFont:
        {
            return "PrimeFont";
        }
        case RenderEffect:
        {
            return "RenderEffect";
        }
        default:
            return "File";
        }
    }

    inline auto static get_type(
        const std::string &type) -> DataType
    {
        if (type == "Image")
        {
            return Image;
        }
        else if (type == "PopAnim")
        {
            return PopAnim;
        }
        else if (type == "Data")
        {
            return Data;
        }
        else if (type == "SoundBank")
        {
            return SoundBank;
        }
        else if (type == "DecodedSoundBank")
        {
            return DecodedSoundBank;
        }
        else if (type == "PrimeFont")
        {
            return PrimeFont;
        }
        else if (type == "RenderEffect")
        {
            return RenderEffect;
        }
        else if (type == "File")
        {
            return File;
        }
        else
        {
            throw Exception("invaild_data_type"); // TODO add localization.
        }
    }

    inline auto static to_json(
        nlohmann::ordered_json &nlohmann_json_j,
        const DataResInfo &nlohmann_json_t) -> void
    {
        nlohmann_json_j["type"] = type_to_string(nlohmann_json_t.type);
        // nlohmann_json_j["name"] = nlohmann_json_t.name;
        nlohmann_json_j["path"] = nlohmann_json_t.path;
        if (nlohmann_json_t.type == Image)
        {
            nlohmann_json_j["default"] = nlohmann_json_t.ptx_default_info;
        }
        return;
    }

    inline auto static from_json(
        const nlohmann::ordered_json &nlohmann_json_j,
        DataResInfo &nlohmann_json_t) -> void
    {
        const auto type = nlohmann_json_j.at("type").get<std::string>();
        nlohmann_json_t.type = get_type(type);
        // nlohmann_json_j.at("name").get_to(nlohmann_json_t.name);
        nlohmann_json_j.at("path").get_to(nlohmann_json_t.path);
        if (nlohmann_json_t.type == Image)
        {
            nlohmann_json_j.at("default").get_to(nlohmann_json_t.ptx_default_info);
        }
        return;
    };

    struct DataInfo
    {
    public:
        DataType type;
        CompressionFlag compression_flags;
        uint32_t format;
        std::map<std::string, DataResInfo> data;

    };

    inline auto static to_json(
        nlohmann::ordered_json &nlohmann_json_j,
        const DataInfo &nlohmann_json_t) -> void
    {
        nlohmann_json_j["type"] = type_to_string(nlohmann_json_t.type);
        nlohmann_json_j["compression_flags"] = nlohmann_json_t.compression_flags;
        if (nlohmann_json_t.type == Image)
        {
            nlohmann_json_j["format"] = nlohmann_json_t.format;
        }
        nlohmann_json_j["data"] = nlohmann_json_t.data;
        return;
    }

    inline auto static from_json(
        const nlohmann::ordered_json &nlohmann_json_j,
        DataInfo &nlohmann_json_t) -> void
    {
        const auto type = nlohmann_json_j.at("type").get<std::string>();
        nlohmann_json_t.type = get_type(type);
        nlohmann_json_j.at("compression_flags").get_to(nlohmann_json_t.compression_flags);
        if (nlohmann_json_t.type == Image)
        {
            nlohmann_json_j.at("format").get_to(nlohmann_json_t.format);
        }
        nlohmann_json_j.at("data").get_to(nlohmann_json_t.data);
        return;
    };

    struct Info
    {
    public:
        bool is_composite;
        std::map<std::string, DataInfo> groups;
    };

    inline auto static to_json(
        nlohmann::ordered_json &nlohmann_json_j,
        const Info &nlohmann_json_t) -> void
    {
        nlohmann_json_j["is_composite"] = nlohmann_json_t.is_composite;
        nlohmann_json_j["groups"] = nlohmann_json_t.groups;
        return;
    }

    inline auto static from_json(
        const nlohmann::ordered_json &nlohmann_json_j,
        Info &nlohmann_json_t) -> void
    {
        nlohmann_json_j.at("is_composite").get_to(nlohmann_json_t.is_composite);
        nlohmann_json_j.at("groups").get_to(nlohmann_json_t.groups);
        return;
    };



   

    template <typename T>
        requires std::is_integral<T>::value
    struct RSB_HeadInfo
    {
        std::string_view magic = "1bsr"_sv;
        T version;
        T file_offset;
        T file_list_length;
        T file_list_begin;
        T rsg_list_length;
        T rsg_list_begin;
        T rsg_number;
        T rsg_info_begin;
        T rsg_info_each_length;
        T composite_number;
        T composite_info_begin;
        T composite_info_each_length;
        T composite_list_length;
        T composite_list_begin;
        T autopool_number;
        T autopool_info_begin;
        T autopool_info_each_length;
        T ptx_number;
        T ptx_info_begin;
        T ptx_info_each_length;
        T part1_begin;
        T part2_begin;
        T part3_begin;

        RSB_HeadInfo(

            ) = default;

        ~RSB_HeadInfo(

            ) = default;

        RSB_HeadInfo(
            RSB_HeadInfo &&that) = delete;

        auto operator=(
            RSB_HeadInfo &&that) -> RSB_HeadInfo & = delete;
    };

    template <typename T>
        requires std::is_integral<T>::value
    struct PTXInfo
    {
    public:
        T id;
        T width;
        T height;
        explicit PTXInfo(

            ) = default;

        explicit PTXInfo(
            T id, T width, T height) : id(id), width(width), height(height)
        {
        }

        ~PTXInfo(

            ) = default;
    };

    template <typename T>
        requires std::is_integral<T>::value
    struct ResInfo
    {
        std::string path;
        bool is_atlas;
        std::vector<uint8_t> data;
        PTXInfo<T> ptx_info;

        explicit ResInfo(

            ) = default;

        explicit ResInfo(
            const std::string path, bool is_atlas) : path(path), is_atlas(is_atlas)
        {
        }

        ~ResInfo(

            ) = default;
    };

    template <typename T>
        requires std::is_integral<T>::value
    struct PacketInfo
    {
        T flags;
        std::vector<ResInfo<T>> res;
    };

    template <typename T>
        requires std::is_integral<T>::value
    struct SubgroupInfo
    {
        T pos;
        T size;
        T format;
    };

    template <typename T>
        requires std::is_integral<T>::value
    struct GroupInfo
    {
        std::map<std::string, SubgroupInfo<T>> subgroup;
    };

}