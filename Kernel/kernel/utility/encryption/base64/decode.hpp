#pragma once

#include "kernel/utility/encryption/base64/common.hpp"

namespace Sen::Kernel::Encryption::Base64 {

    struct Decode : Common {

        constexpr Decode (
        ) = default;

        constexpr ~Decode(
        ) = default;

        static auto process(
            UCharacterArray& source,
            Uint8Array& destination
        ) -> void {
            auto a3 = std::array<u8, 3_size>{};
            auto a4 = std::array<u8, 4_size>{};
            auto raw = source.begin();
            destination.allocate(original_size(source));
            auto ripe = destination.begin();
            auto i = 0_size;
            while (raw != source.end()) {
                if (*raw == '=') {
                    break;
                }
                a4[i++] = *(raw++);
                if (i == 4) {
                    for (auto& c : a4) {
                        c = position(c);
                    }
                    a4_to_a3(a3.data(), a4.data());
                    for (auto& byte : a3) {
                        *ripe++ = byte;
                    }
                    i = 0;
                }
            }
            if (i != 0) {
                std::fill(a4.begin() + i, a4.end(), '\0');
                for (auto j : Range{4}) {
                    a4[j] = position(a4[j]);
                }
                a4_to_a3(a3.data(), a4.data());
                for (auto j : Range{i - 1}) {
                    *ripe++ = a3[j];
                }
            }
        }
    };

}