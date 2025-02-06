#pragma once

#include "kernel/utility/encryption/fnv/common.hpp"

namespace Sen::Kernel::Encryption::FNV {

    struct Hash {

        template <auto bit_count, auto mode> requires std::is_same_v<type_of<bit_count>, BitCount> && std::is_same_v<type_of<mode>, Mode>
        static auto process_whole (
            Uint8Array& data,
            typename Parameter<bit_count>::Value& value
        ) -> void
        {
            using Param = Parameter<bit_count>;
            if constexpr (mode == Mode::FNV0) {
                value = 0;
            } else {
                value = Param::offset;
            }
            for (auto& byte : data) {
                if constexpr (mode == Mode::FNV1a) {
                    value ^= byte;
                    value *= Param::prime;
                } else {
                    value *= Param::prime;
                    value ^= byte;
                }
            }
        }

        template <auto bit_count, auto mode> requires std::is_same_v<type_of<bit_count>, BitCount> && std::is_same_v<type_of<mode>, Mode>
        static auto process(
            Uint8Array& source,
            Uint8Array& destination
        ) -> void {
            if constexpr (bit_count == BitCount::B32) {
                destination.allocate(sizeof(u32));
                auto hash_value = u32{};
                process_whole_integer<BitCount::B32>(source, hash_value, mode);
                std::memcpy(destination.begin(), &hash_value, sizeof(hash_value));
            }
            else {
                destination.allocate(sizeof(u64));
                auto hash_value = u64{};
                process_whole_integer<BitCount::B64>(source, hash_value, mode);
                std::memcpy(destination.begin(), &hash_value, sizeof(hash_value));
            }
        }

    };


}
