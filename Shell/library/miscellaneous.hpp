#pragma once

#include "library/builtin.hpp"
#include "library/string.hpp"

namespace Sen::Shell {

    inline auto parse_arguments (
        int argc,
        #if WINDOWS
        wchar_t* argv[]
        #else
        char* argv[]
        #endif
    ) -> std::vector<std::string> {
        const auto source = std::span{argv, static_cast<usize>(argc)};
        auto result = std::vector<std::string>{};
		result.reserve(source.size());
        for (auto& e : source) {
            #if WINDOWS
            auto source_element_8 = u16_to_u8(std::u16string_view{reinterpret_cast<const char16_t *>(e)});
            std::ranges::replace(source_element_8, '\\', '/');
            result.emplace_back(std::move(reinterpret_cast<std::string &>(source_element_8)));
            #else
            result.emplace_back(e);
            #endif
        }
        return result;
    }

}