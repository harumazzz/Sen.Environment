#pragma once

#include "kernel/utility/library.hpp"
#include "kernel/utility/macro.hpp"
#include "kernel/utility/container/array/byte_array.hpp"

namespace Sen::Kernel::Encryption::Base64 {

    struct Common {

        inline static auto constexpr trailing_type = '='_uc;

        inline static constexpr auto characters = std::array<char, 64>{
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
            'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
            'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
            'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/',
        };

        template <typename T>
        static auto a3_to_a4(
            Pointer<T> a4, 
            Pointer<T> a3
        ) -> void
        {
            a4[0] = (a3[0] & 0xfc) >> 2;
            a4[1] = ((a3[0] & 0x03) << 4) + ((a3[1] & 0xf0) >> 4);
            a4[2] = ((a3[1] & 0x0f) << 2) + ((a3[2] & 0xc0) >> 6);
            a4[3] = (a3[2] & 0x3f);
        }

        template <typename T>
        static auto a4_to_a3(
            Pointer<T> a3,
            Pointer<T> a4
        )-> void
        {
            a3[0] = (a4[0] << 2) + ((a4[1] & 0x30) >> 4);
            a3[1] = ((a4[1] & 0xf) << 4) + ((a4[2] & 0x3c) >> 2);
            a3[2] = ((a4[2] & 0x3) << 6) + a4[3];
        }

    };

    template <typename T>
    auto position(
        T c
    ) -> u8 {
        if(c >= 'A' && c <= 'Z') return c - 'A';
        if(c >= 'a' && c <= 'z') return c - 71;
        if(c >= '0' && c <= '9') return c + 4;
        if(c == '+') return 62;
        if(c == '/') return 63;
        return 255;
    }

    auto constexpr compute_size (
        const usize& size
    ) -> usize {
        return (size + 2_size - ((size + 2_size) % 3_size)) / 3_size * 4_size;
    }

    inline auto original_size (
        UCharacterArray& source
    ) -> usize {
        auto has_match = 0;
        auto end = source.end();
        while (*(--end) == '=') {
            ++has_match;
        }
        return ((6_size * source.size()) / 8_size) - has_match;
    }


}
