#pragma once

#include "kernel/utility/encryption/base64/common.hpp"

namespace Sen::Kernel::Encryption::Base64 {

    struct Decode : Common {

        constexpr Decode (
        ) = default;

        constexpr ~Decode(
        ) = default;

    protected:

        template <auto... Index> requires (std::is_same_v<type_of<Index>, usize> && ...)
        static constexpr auto unroll_position (
            std::array<uint8_t, 4>& a4,
            std::index_sequence<Index...>
        ) -> void {
            ((a4[Index] = position(a4[Index])), ...);
        }

        template <auto... Index> requires (std::is_same_v<type_of<Index>, usize> && ...)
        static constexpr auto unroll_sequences (
            Pointer<u8>& ripe,
            const std::array<uint8_t, 3>& a3,
            std::index_sequence<Index...>
        ) -> void {
            ((*ripe++ = a3[Index]), ...);
        }

    public:

        static auto process(
            UCharacterArray& source,
            Uint8Array& destination
        ) -> void {
            auto a3 = std::array<u8, 3_size>{};
            auto a4 = std::array<u8, 4_size>{};
            auto raw = source.begin();
            destination.allocate(original_size(source));
            auto ripe = destination.begin();
            auto index = 0_size;
            while (raw != source.end()) {
                if (*raw == '=') {
                    break;
                }
                a4[index++] = *(raw++);
                if (index == 4) {
                    unroll_position(a4, std::make_index_sequence<array_size_v<decltype(a4)>>{});
                    a4_to_a3(a3.data(), a4.data());
                    unroll_sequences(ripe, a3, std::make_index_sequence<array_size_v<decltype(a3)>>{});
                    index = 0;
                }
            }
            if (index != 0) {
                std::memset(a4.data() + index, '\0', std::distance(a4.begin() + index, a4.end()));
                unroll_position(a4, std::make_index_sequence<array_size_v<decltype(a4)>>{});
                a4_to_a3(a3.data(), a4.data());
                for (const auto j : Range{index - 1}) {
                    *ripe++ = a3[j];
                }
            }
        }
    };

}