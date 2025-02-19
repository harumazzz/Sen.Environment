#pragma once

#include "kernel/utility/container/array/byte_array.hpp"
#include "kernel/utility/container/string/basic_string.hpp"
#include "kernel/utility/encryption/rijndael/common.hpp"

namespace Sen::Kernel::Encryption::Rijndael {

    template <auto mode> requires std::is_same_v<type_of<mode>, Mode>
    struct Encrypt : Common {

    protected:

        using Common = Common;

        inline static auto constexpr Mode = static_cast<int>(mode);

    public:

        inline static auto process(
            const Uint8Array& source,
            const StringView& key,
            const StringView& iv,
            Uint8Array& destination
        ) -> void
        {
            assert_conditional(is_valid_block_size(key.size() * 8_size), fmt::format("{}", Language::get("rijndael.key_size_is_not_valid")), "process");
            assert_conditional(is_valid_block_size(iv.size() * 8_size), fmt::format("{}", Language::get("rijndael.iv_size_is_not_valid")), "process");
            assert_conditional(is_padded_size(key.size(), 32_size), fmt::format("{}", Language::get("rijndael.key_is_not_padded")), "process");
            assert_conditional(source.size() != 0_size, fmt::format("{}", Language::get("rijndael.encrypt.plain_is_empty")), "process");
            auto rijndael = Subprojects::Rijndael::CRijndael{};
            rijndael.MakeKey(key.begin(), iv.begin(), static_cast<int>(key.size()), static_cast<int>(iv.size()));
            rijndael.Encrypt(reinterpret_cast<char const *>(source.begin()), reinterpret_cast<char*>(destination.begin()), source.size(), Mode);
        }

    };
}