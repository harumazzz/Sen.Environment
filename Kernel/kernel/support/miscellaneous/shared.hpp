#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/resource_stream_group/definition.hpp"

namespace Sen::Kernel::Support::Miscellaneous::Shared
{
    using namespace Sen::Kernel::Support::PopCap::ResourceStreamGroup;

    inline static constexpr auto k_block_size = 4_size;

    inline static constexpr auto k_string_null = ""_sv;

    inline static constexpr auto k_padding_unit_size = 0x1000_size;

    inline static constexpr auto k_none_size = 0_size;

    inline static constexpr auto k_begin_index = 0_size;

    inline auto sub_bytes(
        List<uint8_t> const &data,
        size_t const &begin,
        size_t const &size) -> List<uint8_t>
    {
        assert(begin + size <= data.size());
        auto bytes = List<std::uint8_t>{};
        bytes.assign(data.begin() + begin, data.begin() + begin + size);
        return bytes;
    }

    inline auto write_image(
        std::string const &destination,
        Image<int> const &data) -> void
    {
        FileSystem::create_directory(Path::getParents(destination));
        ImageIO::write_png(destination, data);
        return;
    }

    inline static auto dump_json(
        nlohmann::ordered_json const &content) -> std::string
    {
        return content.dump();
    }

    inline static auto write_xml(
        std::string const &destination,
        tinyxml2::XMLDocument &content) -> void
    {
        FileSystem::create_directory(Path::getParents(destination));
        FileSystem::write_xml(destination, &content);
    }

    inline auto write_text(
        std::string const &destination,
        std::string const &content) -> void
    {
        FileSystem::create_directory(Path::getParents(destination));
        FileSystem::write_file(destination, content);
        return;
    }

    inline auto toupper_case(
        std::string &str) -> void
    {
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);
        return;
    }

    inline auto toupper_back(
        std::string const &str) -> std::string
    {
        auto upper_str = std::string{str.data(), str.size()};
        std::transform(upper_str.begin(), upper_str.end(), upper_str.begin(), ::toupper);
        return upper_str;
    }

    inline auto tolower_case(
        std::string &str) -> void
    {
        std::transform(str.begin(), str.end(), str.begin(), ::tolower);
        return;
    }

    inline auto tolower_back(
        std::string const &str) -> std::string
    {
        auto lower_str = std::string{str.data(), str.size()};
        std::transform(lower_str.begin(), lower_str.end(), lower_str.begin(), ::tolower);
        return lower_str;
    }

    inline auto compare_string(
        std::string_view a,
        std::string_view b) -> bool
    {
        if (a.size() != b.size())
        {
            return false;
        }
        for (const auto &i : Range(a.size()))
        {
            if (tolower(a[i]) != tolower(b[i]))
            {
                return false;
            }
        }
        return true;
    }

    // trim from start (in place)
    inline static auto trim_left(std::string &s) -> void
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
                                        { return !std::isspace(ch); }));
        return;
    }

    // trim from end (in place)
    inline static auto trim_right(std::string &s) -> void
    {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch)
                             { return !std::isspace(ch); })
                    .base(),
                s.end());
        return;
    }

    inline static auto trim_string(
        std::string &s) -> void
    {
        trim_left(s);
        trim_right(s);
        return;
    }

    inline static auto trim_back(
        std::string const &value) -> std::string
    {
        auto str_trim = value;
        trim_string(str_trim);
        return str_trim;
    }

    inline auto get_string(
        std::string_view value) -> std::string
    {
        return std::string{value};
    }

    inline auto case_insensitive_compare(const std::string &a, const std::string &b) -> bool
    {
        return tolower_back(a) < tolower_back(b);
    };

    template <typename K, typename V>
    inline auto search_element_in_map(
        std::map<K, V> const &data,
        V const &value) -> std::map<K, V>::const_iterator
    {
        auto it = data.begin();
        for (; it != data.end(); ++it)
        {
            if (it->second == value)
            {
                return it;
            }
        }
        return it;
    }

    template <typename K, typename V>
    inline auto search_element_in_map(
        std::map<K, V> const &data,
        K const &key) -> std::map<K, V>::const_iterator
    {
        auto it = data.begin();
        for (; it != data.end(); ++it)
        {
            if (it->first == key)
            {
                return it;
            }
        }
        return it;
    }

    template <typename T>
    inline auto async_process_list(
        List<std::future<T>> &data) -> void
    {
        for (auto &element : data)
        {
            element.get();
        }
        return;
    }

    inline auto compute_utf8_character_extra_size(
        char const &character) -> size_t
    {
        auto extra_size = size_t{};
        if (character < 0b1'0000000)
        {
            extra_size = 0;
        }
        else if (character < 0b11'000000)
        {
            assert_conditional(false, fmt::format("{}", Language::get("utf8_charset_invalid")), "compute_utf8_character_extra_size");
        }
        else if (character < 0b111'00000)
        {
            extra_size = 1;
        }
        else if (character < 0b1111'0000)
        {
            extra_size = 2;
        }
        else if (character < 0b11111'000)
        {
            extra_size = 3;
        }
        else
        {
            assert_conditional(false, fmt::format("{}", Language::get("utf8_charset_invalid")), "compute_utf8_character_extra_size");
        }
        return extra_size;
    }
    
    template <typename EnumrationType>
        requires std::is_enum<EnumrationType>::value
    inline static auto magic_enum_cast(
        std::string_view data) -> EnumrationType
    {
        auto value = magic_enum::enum_cast<EnumrationType>(data, magic_enum::case_insensitive);
        assert_conditional(value.has_value(), format(fmt::format("{}", Language::get("miscellaneous.shared.failed_cast_enum")), data, magic_enum::enum_type_name<EnumrationType>()), "magic_enum_cast");
        return value.value();
    }

    namespace CompiledMapData
    {
        inline auto get_common_size(
            std::string const &range_1,
            std::string const &range_2) -> size_t
        {
            auto maximum_common_size = range_1.size() >= range_2.size() ? range_1.size() : range_2.size();
            for (auto index : Range(maximum_common_size))
            {
                if (range_1[index] != range_2[index])
                {
                    return index;
                }
            }
            return maximum_common_size;
        }

        inline auto compute_ripe_size(
            std::map<std::string, Common::ResourceInformation> const &value) -> size_t
        {
            struct WorkOption
            {
                size_t inherit_length;
                bool has;
            };
            auto string_list = List<std::string>{};
            for (auto &element : value)
            {
                string_list.emplace_back(element.first);
            }
            auto work_option = List<WorkOption>(string_list.size());
            if (!string_list.empty())
            {
                work_option[0].inherit_length = k_none_size;
                work_option[0].has = true;
            }
            auto block_count = k_none_size;
            for (auto index : Range(string_list.size()))
            {
                auto &element_key = string_list[index];
                auto a_string_has_child = List<bool>(element_key.size() + 1);
                for (auto index_1 : Range(static_cast<size_t>(index + 1), string_list.size(), size_t{1}))
                {
                    if (!work_option[index_1].has)
                    {
                        auto common_size = get_common_size(element_key, string_list[index_1]);
                        if (!a_string_has_child.at(common_size) && common_size >= work_option[index].inherit_length)
                        {
                            a_string_has_child[common_size] = true;
                            work_option[index_1] = WorkOption{
                                .inherit_length = common_size,
                                .has = true};
                        }
                    }
                }
                auto character_index = k_none_size;
                if (work_option[index].has)
                {
                    character_index += work_option[index].inherit_length;
                    work_option[index].inherit_length = k_none_size;
                    work_option[index].has = false;
                }
                block_count += element_key.size() - character_index + size_t{1};
                auto additional_size = value.at(element_key).read_texture_additional ? Common::information_resource_texture_additional_section_size : Common::information_resource_general_additional_section_size;
                block_count += (additional_size / k_block_size);
            }
            return block_count * k_block_size;
        }

        
    }
};