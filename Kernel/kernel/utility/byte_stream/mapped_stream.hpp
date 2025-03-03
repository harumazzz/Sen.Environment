#pragma once

#include "kernel/utility/filesystem/utility.hpp"


namespace Sen::Kernel {
    using MappedFile = decodeless::resizable_file;

    template<is_integer IntegralType>
    using MappedMemory = CArray<IntegralType>;

    enum AccessMode : u8 {
        ReadOnly,
        ReadWrite,
        WriteOnly,
    };

    template<auto mode, is_numeric_v IntegralType, typename ByteType> requires is_between_v<mode, AccessMode::ReadOnly, AccessMode::WriteOnly> && std::is_same_v<ByteType, MappedMemory<IntegralType> > || std::is_same_v<ByteType, MappedFile>
    class MappedStream {

    private:
    
        using Size = usize;
        
    protected:
        ByteType m_data;

        Size m_position;

    public:
        template<auto access_mode = mode, typename T = ByteType> requires is_between_v<access_mode, AccessMode::ReadOnly, AccessMode::ReadWrite> && std::is_same_v<T, MappedFile>
        explicit MappedStream(StringView const &source) : m_data{std::filesystem::path(source.wstring()), FileSystem::size_file(source)}, m_position{0_size} {
            assert_conditional(thiz.m_data.size() != 0_size, "File is empty", "MappedStream");
            if (auto const block_size = thiz.m_data.size() % sizeof(IntegralType); block_size != 0_size) {
                thiz.m_data.resize(thiz.m_data.size() + block_size);
            }
        }

        template<auto access_mode = mode, typename T = ByteType> requires is_between_v<access_mode, AccessMode::ReadWrite, AccessMode::WriteOnly> && std::is_same_v<T, MappedFile>
        explicit MappedStream(StringView const &source, usize const &allocate_size) : m_data{std::filesystem::path(source.wstring()), allocate_size}, m_position{0_size} {
            assert_conditional(allocate_size != 0_size, "File is empty", "MappedStream");
            auto const block_size = allocate_size % sizeof(IntegralType);
            thiz.m_data.resize(allocate_size + block_size);
        }

        template<typename T = ByteType> requires std::is_same_v<T, MappedMemory<IntegralType> >
        explicit MappedStream(CArray<IntegralType> &source) : m_data{} {
            assert_conditional(thiz.m_data.size() != 0_size, "Array is empty", "MappedStream");
            m_position = 0_size;
            m_data.assign(source);
        }

        MappedStream(
            const MappedStream &other
        ) = delete;

        auto operator=(
            const MappedStream &other
        ) -> MappedStream & = delete;

        MappedStream(
            MappedStream &&other
        ) = delete;

        auto operator=(
            MappedStream &&
        ) -> MappedStream & = delete;

        ~MappedStream() = default;

        auto constexpr set_position(Size const &position) -> void {
            assert_conditional(position < thiz.m_data.size(), "Index must be smaller than data size", "set_position");
            thiz.m_position = position;
        }

        [[nodiscard]] auto constexpr current_position() const -> Size {
            return thiz.m_position;
        }

        [[nodiscard]] auto constexpr size() const -> Size {
            if constexpr (std::is_same_v<ByteType, MappedFile>) {
                return thiz.m_data.size() / sizeof(IntegralType);
            } else {
                return thiz.m_data.size();
            }
        }

        constexpr auto current_iterator(

        ) const -> IntegralType * {
            return thiz.m_data.data() + thiz.m_position;
        }

        constexpr auto begin() const -> IntegralType * {
            return thiz.m_data.data();
        }

        constexpr auto end() const -> IntegralType * {
            return thiz.m_data.data() + thiz.m_data.size();
        }


        auto operator [](Size const &index) -> IntegralType {
            static_assert(mode != AccessMode::WriteOnly, "Cannot access non-existent access mode operator[]");
            auto const current_position = thiz.m_position;
            thiz.set_position(index);
            auto const value = numeric();
            thiz.set_position(current_position);
            return value;
        }

        auto operator +=(
            const Size &size
        ) -> MappedStream & {
            assert_conditional(size <= thiz.m_data.size(), "Index must be smaller than data size", "operator_plus");
            thiz.m_position += size;
            return thiz;
        }

        auto operator -=(
            const Size &size
        ) -> MappedStream & {
            assert_conditional(size <= thiz.m_data.size(), "Index must be smaller than data size", "operator_minus");
            thiz.m_position -= size;
            return thiz;
        }

        // Read Section

        auto numeric() -> IntegralType {
            static_assert(mode != AccessMode::WriteOnly, "Cannot access non-existent access mode operator[]");
            assert_conditional(thiz.m_position + 1_size < thiz.size(), "Index must be smaller than data size", "numeric_original");
            auto value = IntegralType{};
            if constexpr (std::is_same_v<ByteType, MappedFile>) {
                std::memcpy(&value, reinterpret_cast<IntegralType *>(thiz.m_data.data()) + thiz.m_position, sizeof(IntegralType));
            } else {
                std::memcpy(&value, thiz.m_data.begin() + thiz.m_position, 1_size);
            }
            ++thiz.m_position;
            return value;
        }

        template<typename T> requires std::is_arithmetic_v<T>
        auto numeric() -> T {
            static_assert(mode != AccessMode::WriteOnly, "Cannot access non-existent access mode numeric_generic");
            assert_conditional(thiz.m_position + sizeof(T) < thiz.size(), "Index must be smaller than data size", "numeric_generic");
            auto value = T{};
            auto position = thiz.m_position;
            if constexpr (sizeof(T) < sizeof(IntegralType)) {
                position *= sizeof(IntegralType);
            }
            if constexpr (std::is_same_v<ByteType, MappedFile>) {
                std::memcpy(&value, reinterpret_cast<T *>(thiz.m_data.data()) + position, sizeof(T));
            } else {
                std::memcpy(&value, reinterpret_cast<T *>(thiz.m_data.begin()) + position, sizeof(T));
            }
            if (sizeof(T) > sizeof(IntegralType)) {
                thiz.m_position += sizeof(T) / sizeof(IntegralType);
            }
            return value;
        }

        template<typename T> requires std::is_arithmetic_v<T>
        auto numeric(Size const &offset) -> T {
            static_assert(mode != AccessMode::WriteOnly, "Cannot access non-existent access mode numeric_offset");
            assert_conditional(offset + sizeof(T) < thiz.size(), "Index must be smaller than data size", "numeric_offset");
            const auto current_position = thiz.current_position();
            thiz.set_position(offset);
            auto const value = numeric<T>();
            thiz.set_position(current_position);
            return value;
        }

        template<typename T> requires std::is_same_v<T, u32> || std::is_same_v<T, u64>
        auto numeric_variant() -> T {
            static_assert(mode != AccessMode::WriteOnly, "Cannot access non-existent access mode numeric_variant");
            static_assert(sizeof(T) < sizeof(IntegralType), "This generic must has size higher than IntegralType size");
            auto value = T{};
            for (const auto shift_count: Range<usize>{0, sizeof(T) * 8_size, 7}) {
                const auto byte = numeric<u8>();
                if ((byte & 128_byte) != 0_byte) {
                    value |= static_cast<T>(byte & 127) << shift_count;
                } else {
                    value |= static_cast<T>(byte) << shift_count;
                    break;
                }
            }
            return value;
        }

        template<typename T> requires std::is_same_v<T, i32> || std::is_same_v<T, i64>
        auto numeric_zigzag() -> T {
            static_assert(mode != AccessMode::WriteOnly, "Cannot access non-existent access mode numeric_zigzag");
            static_assert(sizeof(T) < sizeof(IntegralType), "This generic must has size higher than IntegralType size");
            if constexpr (std::is_same_v<T, i32>) {
                const auto value = numeric_variant<u32>();
                return static_cast<i32>(value >> 1_size) ^ -static_cast<i32>(value & 1_ui);
            }
            else {
                const auto value = numeric_variant<u64>();
                return static_cast<i64>(value >> 1_size) ^ -static_cast<i64>(value & 1_ul);
            }
        }

        auto subview(Size const &offset, Size const &count) -> CArrayView<IntegralType> {
            static_assert(mode != AccessMode::WriteOnly, "Cannot access non-existent access mode subview");
            assert_conditional(offset + count < thiz.size(), "Index must be smaller than data size", "subview");
            if constexpr (std::is_same_v<ByteType, MappedMemory<IntegralType> >) {
                auto view = thiz.m_data.view();
                return view.subview(offset, count);
            } else {
                auto view = CArrayView<IntegralType>{reinterpret_cast<IntegralType *>(thiz.m_data.data()) + offset, count};
                return view;
            }
        }

        auto bytes(Size const &size) -> CArray<IntegralType> {
            static_assert(mode != AccessMode::WriteOnly, "Cannot access non-existent access mode bytes");
            assert_conditional(thiz.m_position + size < thiz.size(), "Index must be smaller than data size", "bytes");
            auto value = CArray<IntegralType>{size};
            if constexpr (std::is_same_v<ByteType, MappedFile>) {
                std::memcpy(value.data(), reinterpret_cast<IntegralType *>(thiz.m_data.data()) + thiz.m_position, size);
            } else if constexpr (std::is_same_v<ByteType, CArray<IntegralType> >) {
                std::memcpy(value.data(), thiz.m_data.begin() + thiz.m_position, size);
            }
            thiz.m_position += size;
            return value;
        }

        template<typename T> requires std::is_arithmetic_v<T>
        auto bytes(Size const &offset, Size const &count) -> CArray<T> {
            static_assert(mode != AccessMode::WriteOnly, "Cannot access non-existent access mode bytes");
            assert_conditional(offset + count <= thiz.size() * sizeof(IntegralType) / sizeof(T), "Index must be smaller than data size", "bytes_generic");
            auto value = CArray<T>{count};
            if constexpr (std::is_same_v<ByteType, MappedFile>) {
                std::memcpy(value.data(), reinterpret_cast<T *>(thiz.m_data.data()) + offset, count);
            } else if constexpr (std::is_same_v<ByteType, CArray<IntegralType> >) {
                std::memcpy(value.data(), reinterpret_cast<T *>(thiz.m_data.begin()) + offset, count);
            }
            return value;
        }

        template <auto test_size = true>
        auto string(Size const &size) -> StringView {
            static_assert(mode != AccessMode::WriteOnly, "Cannot access non-existent access mode string");
            if constexpr (test_size) {
                assert_conditional(size % sizeof(IntegralType) == 0_size, "String must fill block", "string");
            }
            assert_conditional(thiz.m_position + size < thiz.size(), "Index must be smaller than data size", "string");
            auto c_value = bytes(size);
            auto value = StringView{reinterpret_cast<char *>(c_value.begin()), c_value.size()};
            return value;
        }

        template <auto block_size>
        auto string_block() -> StringView {
            static_assert(mode != AccessMode::WriteOnly, "Cannot access non-existent access mode string");
            static_assert(sizeof(block_size) == sizeof(usize), "Block size must be size_t");
            static_assert(block_size % sizeof(IntegralType) == 0_size, "String must fill block string_block");
            const auto current_position = thiz.m_position;
            auto value = String{};
            while (true) {
                const auto byte = numeric<i8>(current_position);
                if (byte == 0_i8) {
                    break;
                }
                value += byte;
                ++current_position;
            }
            assert_conditional(value.size() < block_size, "Block size must higher than string size", "string_block");
            return value;
        }


        auto string_null_terminated() -> StringView {
            static_assert(mode != AccessMode::WriteOnly, "Cannot access non-existent access mode string_null_terminated");
            auto current_position = thiz.current_position();
            auto value = String{};
            while (true) {
                const auto byte = numeric<i8>(current_position);
                if (byte == 0_i8) {
                    break;
                }
                value += byte;
                ++current_position;
            }
            assert_conditional(value.size() + 1_size % sizeof(IntegralType) == 0_size, "String must fill block", "string");
            thiz.set_position(current_position);
            return value;
        }

        template<typename T> requires std::is_arithmetic_v<T>
        auto string_numeric() -> StringView {
            static_assert(mode != AccessMode::WriteOnly, "Cannot access non-existent access mode string_numeric");
            const auto size = numeric<T>();
            assert_conditional(size + sizeof(T) % sizeof(IntegralType) == 0_size, "String must fill block", "string");
            if constexpr (sizeof(T) < sizeof(IntegralType)) {
                const auto current_position = thiz.current_position();
                auto c_value = thiz.template bytes<T>(current_position + sizeof(T), size);
                auto value = StringView{reinterpret_cast<char *>(c_value.begin()), c_value.size()};
                thiz.m_position += static_cast<usize>((sizeof(T) + size) / sizeof(T)); // May bug
                thiz.set_position(current_position);

                return value;
            }
            else {
                return thiz.template string<T>(size);
            }
        }

        auto block(Size const &size) -> void {
            static_assert(mode != AccessMode::WriteOnly, "Cannot access non-existent access mode block");
            assert_conditional(thiz.m_position + size < thiz.size(), "Index must be smaller than data size", "block");
            thiz.operator+=(size);
        }


        auto boolean() -> bool {
            static_assert(mode != AccessMode::WriteOnly, "Cannot access non-existent access mode boolean");
            return static_cast<bool>(numeric());
        }

        // Write Section

        auto numeric(IntegralType const &value) -> void {
            static_assert(mode != AccessMode::ReadOnly, "Cannot access non-existent access mode numeric");
            assert_conditional(thiz.m_position + 1_size < thiz.size(), "Index must be smaller than data size", "numeric_original");
            if constexpr (std::is_same_v<ByteType, MappedFile>) {
                std::memcpy(reinterpret_cast<IntegralType *>(thiz.m_data.data()) + thiz.m_position, &value, sizeof(IntegralType));
            } else {
                std::memcpy(thiz.m_data.begin() + thiz.m_position, &value, 1_size);
            }
            ++thiz.m_position;
        }

        template<typename T> requires std::is_arithmetic_v<T>
        auto numeric(T const &value) -> void {
            static_assert(mode != AccessMode::ReadOnly, "Cannot access non-existent access mode numeric_generic");
            assert_conditional(thiz.m_position + sizeof(T) < thiz.size(), "Index must be smaller than data size", "numeric_generic");
            auto position = thiz.m_position;
            if constexpr (sizeof(T) < sizeof(IntegralType)) {
                position *= sizeof(IntegralType);
            }
            if constexpr (std::is_same_v<ByteType, MappedFile>) {
                std::memcpy(reinterpret_cast<T *>(thiz.m_data.data()) + position, &value, sizeof(T));
            } else {
                std::memcpy(reinterpret_cast<T *>(thiz.m_data.begin()) + position, &value, sizeof(T));
            }
            if (sizeof(T) > sizeof(IntegralType)) {
                thiz.m_position += sizeof(T) / sizeof(IntegralType);
            }
        }

        template<typename T> requires std::is_arithmetic_v<T>
        auto numeric(T const &value, Size const &offset) -> void {
            static_assert(mode != AccessMode::ReadOnly, "Cannot access non-existent access mode numeric_offset");
            assert_conditional(offset + sizeof(T) < thiz.size(), "Index must be smaller than data size", "numeric_offset");
            const auto current_position = thiz.current_position();
            thiz.set_position(offset);
            numeric<T>(value);
            thiz.set_position(current_position);
        }

        template<typename T> requires std::is_same_v<T, u32> || std::is_same_v<T, u64>
        auto numeric_variant(T const &value) -> void {
            static_assert(mode != AccessMode::ReadOnly, "Cannot access non-existent access mode numeric_variant");
            static_assert(sizeof(T) < sizeof(IntegralType), "This generic must has size higher than IntegralType size");
            auto temporary = value;
            while (temporary >= static_cast<T>(128)) {
                thiz.u8(static_cast<u8>(temporary) & 127_byte | 128_byte);
                temporary >>= 7_size;
            }
            thiz.u8(static_cast<u8>(temporary));
        }

        template<typename T> requires std::is_same_v<T, i32> || std::is_same_v<T, i64>
        auto numeric_zigzag(T const &value) -> void {
            static_assert(mode != AccessMode::ReadOnly, "Cannot access non-existent access mode numeric_zigzag");
            static_assert(sizeof(T) < sizeof(IntegralType), "This generic must has size higher than IntegralType size");
            if constexpr (std::is_same_v<T, i32>) {
                thiz.template numeric_variant<u32>(value << 1_size ^ value >> 31_size);
            }
            else {
                thiz.template numeric_variant<u64>(value << 1_size ^ value >> 63_size);
            }
        }

        auto subview(CArrayView<IntegralType> &value, Size const &offset) -> void {
            static_assert(mode != AccessMode::ReadOnly, "Cannot access non-existent access mode subview");
            assert_conditional(offset + value.size() < thiz.size(), "Index must be smaller than data size", "subview");
            const auto current_position = thiz.current_position();
            thiz.set_position(offset);
            bytes(value);
            thiz.set_position(current_position);
        }

        auto bytes(CArrayView<IntegralType> &value) -> void {
            static_assert(mode != AccessMode::ReadOnly, "Cannot access non-existent access mode bytes");
            const auto size = value.size();
            assert_conditional(thiz.m_position + size < thiz.size(), "Index must be smaller than data size", "bytes");
            if constexpr (std::is_same_v<ByteType, MappedFile>) {
                std::memcpy(reinterpret_cast<IntegralType *>(thiz.m_data.data()) + thiz.m_position, value.data(), size);
            } else if constexpr (std::is_same_v<ByteType, CArray<IntegralType> >) {
                std::memcpy( thiz.m_data.begin() + thiz.m_position, value.data(), size);
            }
            thiz.m_position += size;
        }

        template<typename T> requires std::is_arithmetic_v<T>
        auto bytes(CArrayView<T> &value, Size const &offset) -> void {
            static_assert(mode != AccessMode::ReadOnly, "Cannot access non-existent access mode bytes");
            auto size = value.size();
            assert_conditional(offset + size < thiz.size(), "Index must be smaller than data size", "bytes_generic");
            if constexpr (std::is_same_v<ByteType, MappedFile>) {
                std::memcpy(reinterpret_cast<T *>(thiz.m_data.data()) + offset, value.data(), size);
            } else if constexpr (std::is_same_v<ByteType, CArray<IntegralType> >) {
                std::memcpy( reinterpret_cast<T *>(thiz.m_data.begin()) + offset, value.data(), size);
            }
        }

        template <auto test_size = true>
        auto string(StringView const &value) -> void {
            static_assert(mode != AccessMode::ReadOnly, "Cannot access non-existent access mode string");
            const auto size = value.size();
            if constexpr (test_size) {
                assert_conditional(size % sizeof(IntegralType) == 0_size, "String must fill block", "string");
            }
            assert_conditional(thiz.m_position + size < thiz.size(), "Index must be smaller than data size", "string");
            if constexpr (std::is_same_v<ByteType, MappedFile>) {
                std::memcpy(reinterpret_cast<char *>(thiz.m_data.data()) + thiz.m_position, value.begin(), size);
            } else if constexpr (std::is_same_v<ByteType, CArray<IntegralType> >) {
                std::memcpy( reinterpret_cast<char *>(thiz.m_data.begin()) + thiz.m_position, value.begin(), size);
            }
        }

        template <auto block_size>
        auto string_block(StringView const &value) -> void {
            static_assert(mode != AccessMode::ReadOnly, "Cannot access non-existent access mode string");
            static_assert(sizeof(block_size) == sizeof(usize), "Block size must be size_t");
            static_assert(block_size % sizeof(IntegralType) == 0_size, "String must fill block string_block");
            assert_conditional(value.size() < block_size, "Block size must higher than string size", "string_block");
            thiz.template string<false>(value);
            thiz.fill(value.size() - block_size);
        }

        auto string_null_terminated(StringView const &value) -> void {
            static_assert(mode != AccessMode::ReadOnly, "Cannot access non-existent access mode string_null_terminated");
            assert_conditional((value.size() + 1_size) % sizeof(IntegralType) == 0_size, "String must fill block", "string");
            thiz.template string<false>(value);
            thiz.template numeric<u8>(0_ui);
        }

        template<typename T> requires std::is_arithmetic_v<T>
        auto string_numeric(StringView const &value) -> void {
            static_assert(mode != AccessMode::ReadOnly, "Cannot access non-existent access mode string_numeric");
            assert_conditional((value.size() + sizeof(T)) % sizeof(IntegralType) == 0_size, "String must fill block", "string");
            thiz.template numeric<T>(value.size());
            thiz.template string<false>(value);
        }

        auto boolean(bool const &value) -> void {
            static_assert(mode != AccessMode::ReadOnly, "Cannot access non-existent access mode boolean");
            thiz.numeric(static_cast<IntegralType>(value));
        }

        auto fill(Size const &size, IntegralType const &value = IntegralType{0}) -> void {
            static_assert(mode != AccessMode::ReadOnly, "Cannot access non-existent access mode block");
            assert_conditional(thiz.m_position + size < thiz.size(), "Index must be smaller than data size", "fill");
            std::ranges::fill(thiz.m_data.data() + thiz.m_position, thiz.m_data.data() + thiz.m_position + size, value);
        }

        auto view() -> CArrayView<IntegralType> {
            auto data_view = CArrayView<IntegralType>{reinterpret_cast<IntegralType *>(thiz.m_data.data()), thiz.size()};
            return data_view;
        }

        auto release_stream (
            CArrayView<IntegralType>& that
        ) -> void {
            if constexpr (std::is_same_v<ByteType, CArray<IntegralType> >) {
                that.assign(thiz.m_data);
            }
            else {
                thiz.set_position(0_size);
                auto value = bytes(thiz.size());
                that.assign(value);
            }
        }

        auto write_view(StringView const &path, usize const &offset, usize const &count) -> void {
            const auto size = count * sizeof(IntegralType);
            auto file = MappedFile{std::filesystem::path{path.wstring()}, size};
            file.resize(size);
            if constexpr (std::is_same_v<ByteType, MappedFile>) {
                std::memcpy(static_cast<IntegralType *>(file.data()), reinterpret_cast<IntegralType *>(thiz.m_data.data()) + offset, count);
            } else if constexpr (std::is_same_v<ByteType, CArray<IntegralType> >) {
                std::memcpy(static_cast<IntegralType *>(file.data()), thiz.m_data.begin() + offset, count);
            }
        }
    };

    template<auto mode, is_numeric_v IntegralType, typename ByteType> requires std::is_same_v<ByteType, MappedMemory<IntegralType> > || std::is_same_v<ByteType, MappedFile>
    inline auto operator<<(
        std::ostream &os,
        const MappedStream<mode, IntegralType, ByteType> &stream
    ) -> std::ostream & {
        os << "MappedStream(SizeValue: " << sizeof(IntegralType) << ", Position: " << stream.current_position() << ", Size: " << stream.size() << ")";
        return os;
    }
}
