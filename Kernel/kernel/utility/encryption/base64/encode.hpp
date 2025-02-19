#pragma once

#include "kernel/utility/encryption/base64/common.hpp"

namespace Sen::Kernel::Encryption::Base64 {

    struct Encode : Common {

    public:

        constexpr Encode (
        ) = default;

        constexpr ~Encode(
        ) = default;

    protected:

        template <auto... Index> requires (std::is_same_v<type_of<Index>, usize> && ...)
        static constexpr auto unroll_sequences (
            UCharacterArray& destination,
            usize& position,
            const std::array<uint8_t, 4_size>& a4,
            std::index_sequence<Index...>
        ) -> void {
            ((destination[position++] = characters[a4[Index]]), ...);
        }

    public:

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
                    unroll_sequences (destination, position, a4, std::make_index_sequence<array_size_v<decltype(a4)>>{});
                    index = 0;
                }
            }
            if (index != 0) {
                std::memset(a3.data() + index, '\0', std::distance(a3.begin() + index, a3.end()));
                a3_to_a4(a4.data(), a3.data());
                for (const auto j : Range{index + 1}) {
                    destination[position++] = characters[a4[j]];
                }
                while (index++ < 3) {
                    destination[position++] = trailing_type;
                }
            }
        }

    };


}