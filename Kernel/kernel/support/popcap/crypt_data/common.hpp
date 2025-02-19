#pragma once

namespace Sen::Kernel::Support::PopCap::CryptData {

    struct Common
    {
        static constexpr auto k_magic_identifier = std::array<char, 11> { 0x43, 0x52, 0x59, 0x50, 0x54, 0x5F, 0x52, 0x45, 0x53, 0x0A, 0x00 };
    };
}
