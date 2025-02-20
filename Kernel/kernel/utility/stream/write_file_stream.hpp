#pragma once

#include "kernel/utility/stream/writeable_stream.hpp"


namespace Sen::Kernel
{
    class WriteFileStream : public WritableStream<WriteFileStream>
    {
    private:
        using Base = WritableStream;

        using File = FileSystem::FileHandler;

    protected:
        File m_file{};

    public:
        explicit WriteFileStream(
            const StringView& source
        ) : Base{}, m_file{FileSystem::open_write(source)}
        {
        }

        WriteFileStream(
            const WriteFileStream& other
        ) = delete;

        auto operator=(
            const WriteFileStream& other
        ) -> WriteFileStream& = delete;

        WriteFileStream(
            WriteFileStream&& other
        ) = delete;

        auto operator=(
            WriteFileStream&&
        ) -> WriteFileStream& = delete;

        /// DETAIL : On the File Stream, the size of the file is the current position it is pointing to

        constexpr auto size(

        ) const -> usize
        {
            return thiz.m_position;
        }

        auto view(

        ) -> File&
        {
            return thiz.m_file;
        }

        auto raw(
            const uint8_t* data,
            const usize& size
        ) -> void
        {
            thiz.m_position = size + thiz.m_position;
            thiz.m_file.write(data, size);
        }

        auto allocate_size(const usize& size) -> void
        {
            auto temporary = size + thiz.m_position;
            if (thiz.size() < temporary)
            {
                auto null = Uint8Array{temporary - thiz.size()};
                thiz.write_impl(null);
            }
        }

         auto set_position(
            const usize& index
        ) -> void
        {
            assert_conditional(index <= thiz.m_file.size(), "Index must be smaller than capacity", "set_position");
            thiz.m_position = index;
        }

        template <typename T> requires std::is_arithmetic_v<T>
        auto write_impl(
            const T& value
        ) -> void
        {
            constexpr auto sz = sizeof(T);
            thiz.m_position = sz + thiz.m_position;
            auto count = std::fwrite(&value, sz, 1, thiz.m_file.data());
            assert_conditional(count == 1, fmt::format("{}", "Missing bytes when writing the file"), "write_impl");
        }

        template <typename... Args> requires (sizeof...(Args) > 1 && (is_numeric_v<Args> && ...))
        auto write_impl(
            const Args&... args
        ) -> void
        {
            auto temporary = Uint8Array{total_sizeof<Args...>()};
            {
                auto offset = 0_size;
                (forward_bytes(std::forward<Args>(args), temporary, offset), ...);
            }
            thiz.write_impl(temporary);
        }

        template <typename T> requires (std::is_same_v<T, Uint8Array> or std::is_same_v<T, Uint8List>) && requires(T a)
        {
            { a.size() } -> std::convertible_to<usize>;
            { a.begin() } -> std::convertible_to<uint8_t*>;
        }
        auto write_impl(
            T& value
        ) -> void
        {
            return thiz.raw(value.begin(), value.size());
        }
    };
}
