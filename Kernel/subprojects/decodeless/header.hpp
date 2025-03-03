// Copyright (c) 2024 Pyarelal Knowles, MIT License

#pragma once

#include <algorithm>
#include <array>
#include <bit>
#include <bitset>
#include <cstdint>
#include "./offset_ptr.hpp"
#include "./offset_span.hpp"
#include <string_view>

namespace decodeless {

struct Version {
    static constexpr uint32_t InvalidValue = 0xffffffff;
    uint32_t                  major = InvalidValue;
    uint32_t                  minor = InvalidValue;
    uint32_t                  patch = InvalidValue;

    static bool binaryCompatible(const Version& supported, const Version& loaded) {
        return loaded.major != InvalidValue && supported.major == loaded.major &&
               supported.minor >= loaded.minor;
    }
};

struct GitHash : public std::array<char, 40> {
    constexpr GitHash()
        : std::array<char, 40>() {
        std::fill(this->begin(), this->end(), 0);
    }
    template <size_t N>
    constexpr GitHash(const char (&str)[N])
        : std::array<char, 40>() {
        static_assert(N - 1 <= 40, "GitHash must be at most 40 bytes");
        std::copy_n(str, N - 1, this->begin());
    }
};

struct Magic : public std::array<char, 16> {
    bool operator<(const Magic& other) const {
        return std::string_view(begin(), end()) < std::string_view(other.begin(), other.end());
    }
    constexpr Magic()
        : std::array<char, 16>() {
        std::fill(this->begin(), this->end(), 0);
    }
    template <size_t N>
    constexpr Magic(const char (&str)[N])
        : std::array<char, 16>() {
        static_assert(N - 1 <= 16, "Magic must be at most 16 bytes");
        std::copy_n(str, N - 1, this->begin());
    }
};

enum PlatformFlags {
    ePlatformX32,
    ePlatformX64,
    ePlatformEndianBig,
    ePlatformEndianLittle,
};

struct PlatformBits : std::bitset<64> {
    PlatformBits() {
        this->set(PlatformFlags::ePlatformX32, sizeof(size_t) == 4);
        this->set(PlatformFlags::ePlatformX64, sizeof(size_t) == 8);
        this->set(PlatformFlags::ePlatformEndianBig, std::endian::native == std::endian::big);
        this->set(PlatformFlags::ePlatformEndianLittle, std::endian::native == std::endian::little);
    }
};

// Inherit from Header to add custom headers to the RootHeader
// Must have a static Magic HeaderIdentifier, matching the SubHeader concept.
struct Header {
    Magic   identifier;
    Version version;
    GitHash gitHash;

    bool operator<(const Header& other) const { return identifier < other.identifier; }
};

template <typename T>
concept SubHeader =
    std::is_base_of_v<Header, T> && std::is_same_v<decltype(T::HeaderIdentifier), const Magic>;

template <typename T>
concept VersionedSubHeader =
    SubHeader<T> && std::is_same_v<decltype(T::VersionSupported), const Version>;

// Top level file header with a magic identifier and references to custom
// headers that can then point to real data. Sub headers are idenitified with
// their own magic strings and version numbers. The indirection allows extending
// existing data in a stable header with data in a new header.
struct RootHeader {
    using HeaderList = offset_span<offset_ptr<Header>>;

    RootHeader(Magic identifier)
        : identifier(identifier){};

    // Find and cast a specific header
    template <SubHeader HeaderType>
    inline HeaderType* find() const {
        constexpr Magic      headerIdentifier = HeaderType::HeaderIdentifier;
        HeaderList::iterator result;
        if (headers.size() < 16) {
            result = std::find_if(headers.begin(), headers.end(),
                                  [&headerIdentifier](const offset_ptr<Header>& ptr) {
                                      return ptr->identifier == headerIdentifier;
                                  });
        } else {
            result =
                std::lower_bound(headers.begin(), headers.end(), headerIdentifier, HeaderPtrComp());
            if (result != headers.end() && (*result)->identifier != headerIdentifier) {
                result = headers.end();
            }
        }
        return result == headers.end() ? nullptr : reinterpret_cast<HeaderType*>(result->get());
    }

    template <VersionedSubHeader HeaderType>
    inline HeaderType* findSupported() const {
        HeaderType*       result = find<HeaderType>();
        constexpr Version versionSupported = HeaderType::VersionSupported;
        // TODO: might be nice to throw or return std::expected to differentiate
        // header exists but not compatible vs not found.
        return result && Version::binaryCompatible(versionSupported, result->version) ? result
                                                                                      : nullptr;
    }

    bool magicValid() const { return decodelessMagic == DecodelessMagic; }
    bool binaryCompatible() const {
        return Version::binaryCompatible(VersionSupported, decodelessVersion) &&
               platformBits == PlatformBits();
    }

    static constexpr Magic DecodelessMagic{"DECODELESS->FILE"};

    // User's magic string for file contents, e.g. matching the app headers
    Magic identifier;

    // Identifies decodeless::RootHeader compatible files
    Magic decodelessMagic = DecodelessMagic;

    // Version of this top level header
    static constexpr Version VersionSupported{0, 1, 0};
    Version                  decodelessVersion = VersionSupported;

    // Platform flags that might indicate binary incompatibility if they differ
    PlatformBits platformBits;

    // Sorted contiguous array of sub-headers
    HeaderList headers;

    // Comparison functor to facilitate find() and sorting headers
    struct HeaderPtrComp {
        inline bool operator()(const offset_ptr<Header>& a, const offset_ptr<Header>& b) {
            return a->identifier < b->identifier;
        }
        inline bool operator()(const offset_ptr<Header>& a, const Magic& identifier) {
            return a->identifier < identifier;
        }
        inline bool operator()(const Magic& identifier, const offset_ptr<Header>& b) {
            return identifier < b->identifier;
        }
    };
};

} // namespace decodeless
