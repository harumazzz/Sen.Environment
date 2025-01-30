#pragma once

#include "kernel/utility/encryption/base64/common.hpp"

namespace Sen::Kernel::Encryption::Base64 {

    struct Encode : Common {

        constexpr Encode (
        ) = default;

        constexpr ~Encode(
        ) = default;

        static auto process(
            Uint8Array& source,
            UCharacterArray& destination
        ) -> void {
            auto size = source.size();
            destination.allocate(compute_size(size));
            auto position = 0_size;
            auto a3 = std::array<u8, 3_size>{};
            auto a4 = std::array<u8, 4_size>{};
            auto source_data = source.data();
            auto index = 0_size;
            while (size--) {
                a3[index++] = *(source_data++);
                if (index == 3) {
                    a3_to_a4(a4.data(), a3.data()); 
                    for (auto j : Range{4}) {
                        destination[position++] = characters[a4[j]];
                    }
                    index = 0;
                }
            }
            if (index != 0) {
                std::fill(a3.begin() + index, a3.end(), '\0');
                a3_to_a4(a4.data(), a3.data());
                for (auto j : Range{index + 1}) {
                    destination[position++] = characters[a4[j]];
                }
                while (index++ < 3) {
                    destination[position++] = trailing_type;
                }
            }
        }

    };


}