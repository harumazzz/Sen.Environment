#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/utility/library.hpp"
#include "kernel/utility/macro.hpp"
#include "kernel/utility/trait/trait.hpp"

namespace Sen::Kernel
{

    template <auto use_big_endian>
    struct Stream
    {

        static_assert(sizeof(use_big_endian) == sizeof(bool));

        static_assert(use_big_endian == true or use_big_endian == false);

    private:
        List<std::uint8_t> mutable data{};

        std::string_view mutable file_path = ""_sv;

        std::size_t mutable length = size_t{0};

        inline static auto constexpr buffer_size = static_cast<size_t>(8192);

        inline static auto constexpr close_file = [](FILE *file)
        {
            if (file != nullptr)
            {
                std::fclose(file);
                file = nullptr;
            }
            return;
        };

    public:
        std::size_t mutable read_pos = size_t{0};

        std::size_t mutable write_pos = size_t{0};

        Stream() : read_pos{0}, write_pos{0}, length{0}
        {
        }

        Stream(
            const List<std::uint8_t> &data
        ) : data{data}, read_pos{0}, write_pos{data.size()}, length{data.size()}
        {
        }
        
        Stream(
            List<std::uint8_t> &&data
        ) : data{std::move(data)}, read_pos{0}, write_pos{data.size()}, length{data.size()}
        {
        }

        Stream(
            Stream &&that
        ) noexcept : data{std::move(that.data)}, length{that.length}, read_pos{0}, write_pos{0}
        {
        }

        auto operator=(
            Stream &&that
        ) -> Stream & = delete;

        Stream(
            std::string_view source
        ) : read_pos{0}, write_pos{0}
        {
            #if WINDOWS
            auto file = WindowsFileReader(String::utf8_to_utf16(fmt::format("\\\\?\\{}", String::to_windows_style(source.data()))).data());
            #else
            auto file = std::unique_ptr<FILE, decltype(close_file)>(std::fopen(source.data(), "rb"), close_file);
            #endif
            #if !WINDOWS
            assert_conditional(file != nullptr, fmt::format("{}: {}", Language::get("cannot_read_file"), source), "Stream");
            #endif
            #if WINDOWS
            auto size = std::filesystem::file_size(std::filesystem::path{String::utf8_to_utf16(source.data())});
            #else
            auto size = std::filesystem::file_size(std::filesystem::path{source});
            #endif
            assert_conditional(size < static_cast<std::uintmax_t>(thiz.data.max_size()), fmt::format("{}", Language::get("file_size_is_bigger_than_data_stream_view")), "Stream");
            thiz.reserve(static_cast<std::uint64_t>(size + thiz.buffer_size));
            thiz.file_path = source;
            thiz.length = size;
            thiz.write_pos = size;
            #if WINDOWS
            auto bytes_read = DWORD{};
            auto read_success = ReadFile(file.handle, thiz.data.data(), static_cast<DWORD>(size), &bytes_read, nullptr);
            assert_conditional(SUCCEEDED(read_success), fmt::format("{}: {}", Language::get("cannot_read_file"), String::to_posix_style(std::string{source.data(), source.size()})), "Stream");
            assert_conditional(bytes_read == size, fmt::format("{}: {}", Language::get("cannot_read_file"), String::to_posix_style(std::string{source.data(), source.size()})), "Stream");
            #else
            std::fread(thiz.data.data(), 1, size, file.get());
            #endif
        }

        Stream(
            const std::size_t &length
        ) : read_pos{0}, write_pos{length}, length{length}
        {
            thiz.reserve(length + thiz.buffer_size);
            return;
        }

        ~Stream(

        )
        {
            thiz.close();
        }

        inline auto getFilePath(

        ) const -> std::string_view
        {
            return thiz.file_path;
        }

        inline auto fromString(
            const std::string &it
        ) const -> void
        {
            thiz.close();
            thiz.data.assign(it.begin(), it.end());
            thiz.length = thiz.data.size();
            thiz.write_pos = thiz.length;
            return;
        }

        inline auto begin(

        ) -> decltype(thiz.data.begin())
        {
            return thiz.data.begin();
        }

        inline auto end(

        ) -> decltype(thiz.data.end())
        {
            return thiz.data.end();
        }

        inline auto constexpr get_length(

        ) const -> uint64_t
        {
            return thiz.length;
        }

        inline auto constexpr size(

        ) const -> size_t
        {
            return thiz.length;
        }

        inline auto constexpr capacity(

        ) const -> size_t
        {
            return thiz.data.size();
        }

        inline auto constexpr reserve(
            const std::size_t &capacity
        ) const -> void
        {
            thiz.data.resize(capacity);
            return;
        }

        inline auto toBytes(

        ) const -> List<std::uint8_t>
        {
            auto bytes = List<std::uint8_t>{};
            bytes.reserve(thiz.length);
            bytes.assign(thiz.data.begin(), data.begin() + thiz.length);
            return bytes;
        }

        inline constexpr auto get(

        ) const -> const List<std::uint8_t> &
        {
            return thiz.toBytes();
        }

        inline auto get(
            size_t from,
            size_t to
        ) const -> List<uint8_t>
        {
            if (from < 0 || to > thiz.data.size())
            {
                throw Exception(fmt::format("{} {} {} {}", Language::get("buffer.invalid.size"), from, Language::get("to"), to), std::source_location::current(), "get");
            }
            return List<unsigned char>(thiz.data.begin() + from, thiz.data.begin() + to);
        }

        inline constexpr auto read_position(

        ) const -> std::size_t
        {
            return thiz.read_pos;
        }

        inline constexpr auto write_position(

        ) const -> std::size_t
        {
            return thiz.write_pos;
        }

        inline auto read_position(
            const std::size_t &pos
        ) const -> void
        {
            assert_conditional(pos <= thiz.size(), String::format(fmt::format("{}: {}", Language::get("read_position_cannot_be_smaller_than_size"), thiz.file_path), std::to_string(pos), std::to_string(thiz.size())), "position");
            thiz.read_pos = pos;
            return;
        }

        inline auto constexpr write_position(
            const std::size_t &pos
        ) const -> void
        {
            thiz.write_pos = pos;
            return;
        }

        inline auto toString(

        ) -> std::string
        {
            auto ss = std::stringstream{};
            auto bytes = thiz.data.data();
            ss << bytes;
            return ss.str();
        }

        // insert to end of vector, write_pos will set to end.

        template <typename T>
            requires CharacterOnView<T>
        inline auto append(
            const List<T> &m_data
        ) const -> void
        {
            thiz.data.insert(thiz.data.begin() + thiz.length, m_data.begin(), m_data.end());
            thiz.length += m_data.size();
            thiz.write_pos = thiz.length;
            return;
        }

        template <typename T, size_t n>
            requires CharacterOnView<T>
        inline auto append(
            const std::array<T, n> &m_data
        ) const -> void
        {
            thiz.data.insert(thiz.data.begin() + thiz.length, m_data.begin(), m_data.end());
            thiz.length += m_data.size();
            thiz.write_pos = thiz.length;
            return;
        }

        

        inline auto out_file(
            std::string_view path
        ) const -> void
        {
            #if defined(_WIN32) || defined(_WIN64)
            auto filePath = std::filesystem::path{String::utf8_to_utf16(String::to_windows_style(path.data()))};
            #else
            auto filePath = std::filesystem::path{path};
            #endif
            if (filePath.has_parent_path()) {
                std::filesystem::create_directories(filePath.parent_path());
            }
            #if defined(_WIN32) || defined(_WIN64)
            auto file = WindowsFileWriter{String::utf8_to_utf16(String::to_windows_style(path.data()))};
            #else
            auto file = std::unique_ptr<FILE, decltype(close_file)>(std::fopen(path.data(), "wb"), close_file);
            assert_conditional(file != nullptr, fmt::format("{}: {}", Language::get("write_file_error"), path), "out_file");
            #endif
            auto dataSize = thiz.length; 
            #if defined(_WIN32) || defined(_WIN64)
            auto bytesWritten = DWORD{};
            auto result = WriteFile(file.handle, thiz.data.data(), static_cast<DWORD>(dataSize), &bytesWritten, nullptr);
            assert_conditional(SUCCEEDED(result), fmt::format("{}: {}", Language::get("write_file_error"), String::to_posix_style(std::string{path.data(), path.size()})), "out_file");
            assert_conditional(bytesWritten == dataSize, fmt::format("{}: {}", Language::get("write_file_error"), String::to_posix_style(std::string{path.data(), path.size()})), "out_file");
            #else
            auto written = std::fwrite(thiz.data.data(), 1, dataSize, file.get());
            assert_conditional(written == dataSize, fmt::format("{}: {}", Language::get("write_file_error"), path), "out_file");
            #endif
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto writeUint8(
            std::uint8_t value,
            Args... args
        ) const -> void
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                thiz.write_pos = std::get<0>(std::make_tuple(args...));
            }
            this->template write_LE<std::uint8_t>(value);
            return;
        }

        inline auto allocate(
            const std::size_t &size
        ) -> void
        {
            thiz.data.reserve(size);
            return;
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto writeUint16(
            std::uint16_t value,
            Args... args
        ) const -> void
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                thiz.write_pos = std::get<0>(std::make_tuple(args...));
            }
            if constexpr (use_big_endian)
            {
                this->template write_BE<std::uint16_t>(value);
            }
            else
            {
                this->template write_LE<std::uint16_t>(value);
            }
            return;
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto writeUint24(
            std::uint32_t value,
            Args... args) const -> void
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                thiz.write_pos = std::get<0>(std::make_tuple(args...));
            }
            auto size = 3;
            if constexpr (use_big_endian)
            {
                for (auto i : Range{size})
                {
                    thiz.writeUint8((value >> ((size - 1 - i) * 8)) & 0xFF);
                }
            }
            else
            {
                for (auto i : Range{size})
                {
                    thiz.writeUint8((value >> (i * 8)) & 0xFF);
                }
            }
            return;
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto writeUint32(
            std::uint32_t value,
            Args... args) const -> void
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                thiz.write_pos = std::get<0>(std::make_tuple(args...));
            }
            if constexpr (use_big_endian)
            {
                this->template write_BE<std::uint32_t>(value);
            }
            else
            {
                this->template write_LE<std::uint32_t>(value);
            }
            return;
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto writeUint64(
            std::uint64_t value,
            Args... args) const -> void
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                thiz.write_pos = std::get<0>(std::make_tuple(args...));
            }
            if constexpr (use_big_endian)
            {
                this->template write_BE<std::uint64_t>(value);
            }
            else
            {
                this->template write_LE<std::uint64_t>(value);
            }
            return;
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto writeInt8(
            std::int8_t value,
            Args... args) const -> void
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                thiz.write_pos = std::get<0>(std::make_tuple(args...));
            }
            this->template write_LE<std::int8_t>(value);
            return;
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto writeInt16(
            std::int16_t value,
            Args... args) const -> void
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                thiz.write_pos = std::get<0>(std::make_tuple(args...));
            }
            if constexpr (use_big_endian)
            {
                this->template write_BE<std::int16_t>(value);
            }
            else
            {
                this->template write_LE<std::int16_t>(value);
            }
            return;
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto writeInt24(
            std::int32_t value,
            Args... args) const -> void
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                thiz.write_pos = std::get<0>(std::make_tuple(args...));
            }
            auto size = 3;
            if constexpr (use_big_endian)
            {
                for (auto i : Range{size})
                {
                    thiz.writeInt8((value >> ((size - 1 - i) * 8)) & 0xFF);
                }
            }
            else
            {
                for (auto i : Range{size})
                {
                    thiz.writeInt8((value >> (i * 8)) & 0xFF);
                }
            }
            return;
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto writeInt32(
            std::int32_t value,
            Args... args) const -> void
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                thiz.write_pos = std::get<0>(std::make_tuple(args...));
            }
            if constexpr (use_big_endian)
            {
                this->template write_BE<std::int32_t>(value);
            }
            else
            {
                this->template write_LE<std::int32_t>(value);
            }
            return;
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto writeInt64(
            std::int64_t value,
            Args... args) const -> void
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                thiz.write_pos = std::get<0>(std::make_tuple(args...));
            }
            if constexpr (use_big_endian)
            {
                this->template write_BE<std::int64_t>(value);
            }
            else
            {
                this->template write_LE<std::int64_t>(value);
            }
            return;
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto writeBytes(
            const List<std::uint8_t> &inputBytes,
            Args... args) const -> void
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                thiz.write_pos = std::get<0>(std::make_tuple(args...));
            }
            auto new_pos = thiz.write_pos + inputBytes.size();
            if (new_pos > thiz.capacity())
            {
                thiz.reserve(new_pos + thiz.buffer_size);
            }
            if (new_pos > thiz.length)
            {
                thiz.length = new_pos;
            }
            std::move(inputBytes.begin(), inputBytes.end(), thiz.data.begin() + thiz.write_pos);
            thiz.write_pos = new_pos;
            return;
        }

         template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto writeRaw(
            const uint8_t* inputBytes,
            const size_t &inputLength,
            Args... args) const -> void
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                thiz.write_pos = std::get<0>(std::make_tuple(args...));
            }
            auto new_pos = thiz.write_pos + inputLength;
            if (new_pos > thiz.capacity())
            {
                thiz.reserve(new_pos + thiz.buffer_size);
            }
            if (new_pos > thiz.length)
            {
                thiz.length = new_pos;
            }

            std::move(inputBytes, inputBytes + inputLength, thiz.data.begin() + thiz.write_pos);
            thiz.write_pos = new_pos;
            return;
        }

        template <class T>
        inline static auto set_raw_data(const T &val) -> List<uint8_t>
        {
            auto res = List<uint8_t>(sizeof(T));
            *(reinterpret_cast<T *>(res.data())) = val;
            if constexpr (use_big_endian)
            {
                std::reverse(res.begin(), res.end());
            }
            return res;
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto writeFloat(
            float value,
            Args... args) const -> void
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                thiz.write_pos = std::get<0>(std::make_tuple(args...));
            }
            auto res = set_raw_data(value);
            thiz.writeBytes(res);
            return;
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto writeDouble(
            double value,
            Args... args) const -> void
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                thiz.write_pos = std::get<0>(std::make_tuple(args...));
            }
            auto res = set_raw_data(value);
            thiz.writeBytes(res);
            return;
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto writeChar(
            char value,
            Args... args) const -> void
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                thiz.write_pos = std::get<0>(std::make_tuple(args...));
            }
            thiz.writeUint8(static_cast<uint8_t>(value));
            return;
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto writeVarInt32(
            std::int32_t value,
            Args... args) const -> void
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                thiz.write_pos = std::get<0>(std::make_tuple(args...));
            }
            auto num = 0;
            for (num = static_cast<std::uint32_t>(value); num >= 128; num >>= 7)
            {
                thiz.writeUint8(static_cast<uint8_t>(num | 0x80));
            }
            thiz.writeUint8(static_cast<uint8_t>(num));
            return;
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto writeVarInt64(
            std::int64_t value,
            Args... args) const -> void
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                thiz.write_pos = std::get<0>(std::make_tuple(args...));
            }
            uint64_t num = 0;
            for (num = static_cast<uint64_t>(value); num >= 128; num >>= 7)
            {
                thiz.writeUint8((uint8_t)(num | 0x80));
            }
            thiz.writeUint8((uint8_t)num);
            return;
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto writeZigZag32(
            std::int32_t value,
            Args... args) const -> void
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                thiz.write_pos = std::get<0>(std::make_tuple(args...));
            }
            auto zigzag_num = (std::uint32_t)((value << 1) ^ (value >> 31));
            thiz.writeVarInt32(zigzag_num);
            return;
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto writeZigZag64(
            std::int64_t value,
            Args... args) const -> void
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                thiz.write_pos = std::get<0>(std::make_tuple(args...));
            }
            auto zigzag_num = (std::uint64_t)((value << 1) ^ (value >> 31));
            thiz.writeVarInt64(zigzag_num);
            return;
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto writeString(
            std::string_view str,
            Args... args) const -> void
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                thiz.write_pos = std::get<0>(std::make_tuple(args...));
            }
            auto new_pos = thiz.write_pos + str.size();
            if (new_pos > thiz.capacity())
            {
                thiz.reserve(new_pos + thiz.buffer_size);
            }
            if (new_pos > thiz.length)
            {
                thiz.length = new_pos;
            }
            std::copy(str.begin(), str.end(), thiz.data.begin() + thiz.write_pos);
            thiz.write_pos = new_pos;
            return;
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto writeStringFourByte(
            std::string_view str,
            Args... args) const -> void
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                thiz.write_pos = std::get<0>(std::make_tuple(args...));
            }
            auto new_pos = thiz.write_pos + str.size() * 4;
            if (new_pos > thiz.capacity())
            {
                thiz.reserve(new_pos + thiz.buffer_size);
            }
            for (auto &c : str)
            {
                thiz.data[thiz.write_pos++] = static_cast<std::uint8_t>(c);
                std::fill_n(&thiz.data[thiz.write_pos], 3, 0);
                thiz.write_pos += 3;
            }
            std::fill_n(&thiz.data[thiz.write_pos], 4, 0);
            thiz.write_pos += 4;
            return;
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto writeNull(
            std::size_t size,
            Args... args) const -> void
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                thiz.write_pos = std::get<0>(std::make_tuple(args...));
            }
            if (size == 0)
            {
                return;
            }
            if (thiz.write_pos >= thiz.length)
            {
                auto new_length = thiz.write_pos + size;
                if (new_length > thiz.capacity()) {
                    thiz.reserve(new_length + thiz.buffer_size);
                }
                thiz.length = new_length;
                thiz.write_pos += size;
            }
            else
            {
                for (auto i : Range(size))
                {
                    thiz.writeUint8(0x0);
                }
            }
            return;
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto writeCharByInt16(
            wchar_t value,
            Args... args) const -> void
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                thiz.write_pos = std::get<0>(std::make_tuple(args...));
            }
            thiz.writeInt16(static_cast<int16_t>(value));
            return;
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto writeStringByUint8(
            std::string_view str,
            Args... args) const -> void
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                thiz.write_pos = std::get<0>(std::make_tuple(args...));
            }
            if (str.empty())
            {
                thiz.writeUint8(0);
                return;
            }
            thiz.writeUint8(str.size());
            thiz.writeString(str);
            return;
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto writeStringByUint16(
            std::string_view str,
            Args... args) const -> void
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                thiz.write_pos = std::get<0>(std::make_tuple(args...));
            }
            if (str.empty())
            {
                thiz.writeUint16(0);
                return;
            }
            thiz.writeUint16(str.size());
            thiz.writeString(str);
            return;
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto writeStringByUint32(
            std::string_view str,
            Args... args) const -> void
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                thiz.write_pos = std::get<0>(std::make_tuple(args...));
            }
            if (str.empty())
            {
                thiz.writeUint32(0);
                return;
            }
            thiz.writeUint32(str.size());
            thiz.writeString(str);
            return;
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto writeStringByInt8(
            std::string_view str,
            Args... args) const -> void
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                thiz.write_pos = std::get<0>(std::make_tuple(args...));
            }
            if (str.empty())
            {
                thiz.writeInt8(0);
                return;
            }
            thiz.writeInt8(str.size());
            thiz.writeString(str);
            return;
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto writeStringByInt16(
            std::string_view str,
            Args... args) const -> void
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                thiz.write_pos = std::get<0>(std::make_tuple(args...));
            }
            if (str.empty())
            {
                thiz.writeInt16(0);
                return;
            }
            thiz.writeInt16(str.size());
            thiz.writeString(str);
            return;
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto writeStringByInt32(
            std::string_view str,
            Args... args) const -> void
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                thiz.write_pos = std::get<0>(std::make_tuple(args...));
            }
            if (str.empty())
            {
                thiz.writeInt32(0);
                return;
            }
            thiz.writeInt32(str.size());
            thiz.writeString(str);
            return;
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto writeStringByVarInt32(
            std::string_view str,
            Args... args) const -> void
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                thiz.write_pos = std::get<0>(std::make_tuple(args...));
            }
            if (str.empty())
            {
                thiz.writeVarInt32(0);
                return;
            }
            thiz.writeVarInt32(str.size());
            thiz.writeString(str);
            return;
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto writeStringByEmpty(
            std::string_view str,
            Args... args) const -> void
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                thiz.write_pos = std::get<0>(std::make_tuple(args...));
            }
            if (str.empty())
            {
                thiz.writeUint8(0);
                return;
            }
            thiz.writeString(str);
            thiz.writeUint8(static_cast<std::uint64_t>(0));
            return;
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto writeBoolean(
            bool value,
            Args... args) const -> void
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                thiz.write_pos = std::get<0>(std::make_tuple(args...));
            }
            if (value)
            {
                thiz.writeUint8(0x01);
            }
            else
            {
                thiz.writeUint8(0x00);
            }
            return;
        }

        inline auto operator[](
            size_t position) const -> uint8_t &
        {
            return this->data.at(position);
        }

        template <typename T>
            requires std::is_integral<T>::value
        inline auto write_LE(T value) const -> void
        {
            auto size = sizeof(T);
            auto new_pos = thiz.write_pos + size;
            // append capacity.
            if (new_pos > thiz.capacity())
            {
                thiz.reserve(new_pos + thiz.buffer_size);
            }
            if (new_pos > thiz.length)
            {
                thiz.length = new_pos;
            }
            std::memcpy(&thiz.data[thiz.write_pos], &value, size);
            thiz.write_pos += size;
            return;
        }

        template <typename T>
            requires std::is_integral<T>::value
        inline auto write_BE(
            T value) const -> void
        {
            auto size = sizeof(T);
            auto new_pos = thiz.write_pos + size;
            // append capacity.
            if (new_pos > thiz.capacity())
            {
                thiz.reserve(new_pos + thiz.buffer_size);
            }
            if (new_pos > thiz.length)
            {
                thiz.length = new_pos;
            }
            for (auto i : Range(size))
            {
                thiz.data[thiz.write_pos++] = ((value >> ((size - 1 - i) * 8)) & 0xFF);
            }
            return;
        }

        template <typename T>
            requires std::is_integral_v<T> || std::is_floating_point_v<T>
        inline auto write_of(
            T const &value) const -> void
        {
            if constexpr (use_big_endian)
            {
                this->template write_BE<T>(value);
            }
            else
            {
                this->template write_LE<T>(value);
            }
        }

        template <typename T>
            requires std::is_integral_v<T> || std::is_floating_point_v<T>
        inline auto read_of() const -> T
        {
            if constexpr (use_big_endian)
            {
                return thiz.reverse_endian(this->template read<T>());
            }
            else
            {
                return this->template read<T>();
            }
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto readUint8(
            Args... args) const -> std::uint8_t
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                auto view = std::get<0>(std::make_tuple(args...));
                assert_conditional(view < this->size(), fmt::format("{}, {}: {}, {}: {}", Language::get("buffer.read_offset_outside_bounds_of_dataview"), Language::get("buffer.new_position"), this->write_pos, Language::get("buffer.actual_size"), this->size()),
                                    "readUint8");
                thiz.read_pos = view;
            }
            return this->template read<std::uint8_t>();
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto readUint16(
            Args... args) const -> std::uint16_t
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                auto view = std::get<0>(std::make_tuple(args...));
                assert_conditional(view < this->size(), fmt::format("{}, {}: {}, {}: {}", Language::get("buffer.read_offset_outside_bounds_of_dataview"), Language::get("buffer.new_position"), this->write_pos, Language::get("buffer.actual_size"), this->size()),
                                    "readUint16");
                thiz.read_pos = view;
            }
            if constexpr (use_big_endian)
            {
                return reverse_endian(this->template read<std::uint16_t>());
            }
            else
            {
                return this->template read<std::uint16_t>();
            }
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto readUint24(
            Args... args) const -> std::uint32_t
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                auto view = std::get<0>(std::make_tuple(args...));
                assert_conditional(view < this->size(), fmt::format("{}, {}: {}, {}: {}", Language::get("buffer.read_offset_outside_bounds_of_dataview"), Language::get("buffer.new_position"), this->write_pos, Language::get("buffer.actual_size"), this->size()),
                                    "readUint24");
                thiz.read_pos = view;
            }
            if constexpr (use_big_endian)
            {
                return thiz.reverse_endian(this->template read_has<std::uint32_t>(3));
            }
            else
            {
                return this->template read_has<std::uint32_t>(3);
            }
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto readUint32(
            Args... args) const -> std::uint32_t
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                auto view = std::get<0>(std::make_tuple(args...));
                assert_conditional(view < this->size(), fmt::format("{}, {}: {}, {}: {}", Language::get("buffer.read_offset_outside_bounds_of_dataview"), Language::get("buffer.new_position"), this->write_pos, Language::get("buffer.actual_size"), this->size()),
                                    "readUint32");
                thiz.read_pos = view;
            }
            if constexpr (use_big_endian)
            {
                return reverse_endian(this->template read<std::uint32_t>());
            }
            else
            {
                return this->template read<std::uint32_t>();
            }
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto readUint64(
            Args... args) const -> std::uint64_t
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                auto view = std::get<0>(std::make_tuple(args...));
                assert_conditional(view < this->size(), fmt::format("{}, {}: {}, {}: {}", Language::get("buffer.read_offset_outside_bounds_of_dataview"), Language::get("buffer.new_position"), this->write_pos, Language::get("buffer.actual_size"), this->size()),
                                    "readUint64");
                thiz.read_pos = view;
            }
            if constexpr (use_big_endian)
            {
                return thiz.reverse_endian(this->template read<std::uint64_t>());
            }
            else
            {
                return this->template read<std::uint64_t>();
            }
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto readInt8(
            Args... args) const -> std::int8_t
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                auto view = std::get<0>(std::make_tuple(args...));
                assert_conditional(view < this->size(), fmt::format("{}, {}: {}, {}: {}", Language::get("buffer.read_offset_outside_bounds_of_dataview"), Language::get("buffer.new_position"), this->write_pos, Language::get("buffer.actual_size"), this->size()),
                                    "readInt8");
                thiz.read_pos = view;
            }
            return this->template read<std::int8_t>();
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto readInt16(
            Args... args) const -> std::int16_t
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                auto view = std::get<0>(std::make_tuple(args...));
                assert_conditional(view < this->size(), fmt::format("{}, {}: {}, {}: {}", Language::get("buffer.read_offset_outside_bounds_of_dataview"), Language::get("buffer.new_position"), this->write_pos, Language::get("buffer.actual_size"), this->size()),
                                    "readInt16");
                thiz.read_pos = view;
            }
            if constexpr (use_big_endian)
            {
                return thiz.reverse_endian(this->template read<std::int16_t>());
            }
            else
            {
                return this->template read<std::int16_t>();
            }
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto readInt24(
            Args... args) const -> std::int32_t
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                auto view = std::get<0>(std::make_tuple(args...));
                assert_conditional(view < this->size(), fmt::format("{}, {}: {}, {}: {}", Language::get("buffer.read_offset_outside_bounds_of_dataview"), Language::get("buffer.new_position"), this->write_pos, Language::get("buffer.actual_size"), this->size()),
                                    "readInt24");
                thiz.read_pos = view;
            }
            if constexpr (use_big_endian)
            {
                return thiz.reverse_endian(this->template read_has<std::int32_t>(3));
            }
            else
            {
                return this->template read_has<std::int32_t>(3);
            }
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto readInt32(
            Args... args) const -> std::int32_t
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                auto view = std::get<0>(std::make_tuple(args...));
                assert_conditional(view < this->size(), fmt::format("{}, {}: {}, {}: {}", Language::get("buffer.read_offset_outside_bounds_of_dataview"), Language::get("buffer.new_position"), this->write_pos, Language::get("buffer.actual_size"), this->size()),
                                    "readInt32");
                thiz.read_pos = view;
            }
            if constexpr (use_big_endian)
            {
                return thiz.reverse_endian(this->template read<std::int32_t>());
            }
            else
            {
                return this->template read<std::int32_t>();
            }
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto readInt64(
            Args... args) const -> std::int64_t
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                auto view = std::get<0>(std::make_tuple(args...));
                assert_conditional(view < this->size(), fmt::format("{}, {}: {}, {}: {}", Language::get("buffer.read_offset_outside_bounds_of_dataview"), Language::get("buffer.new_position"), this->write_pos, Language::get("buffer.actual_size"), this->size()),
                                    "readInt64");
                thiz.read_pos = view;
            }
            if constexpr (use_big_endian)
            {
                return thiz.reverse_endian(this->template read<std::int64_t>());
            }
            else
            {
                return this->template read<std::int64_t>();
            }
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto readFloat(
            Args... args) const -> float
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                auto view = std::get<0>(std::make_tuple(args...));
                assert_conditional(view < this->size(), fmt::format("{}, {}: {}, {}: {}", Language::get("buffer.read_offset_outside_bounds_of_dataview"), Language::get("buffer.new_position"), this->write_pos, Language::get("buffer.actual_size"), this->size()),
                                    "readFloat");
                thiz.read_pos = view;
            }
            if constexpr (use_big_endian)
            {
                return thiz.reverse_endian(this->template read<float>());
            }
            else
            {
                return this->template read<float>();
            }
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto readDouble(
            Args... args) const -> double
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                auto view = std::get<0>(std::make_tuple(args...));
                assert_conditional(view < this->size(), fmt::format("{}, {}: {}, {}: {}", Language::get("buffer.read_offset_outside_bounds_of_dataview"), Language::get("buffer.new_position"), this->write_pos, Language::get("buffer.actual_size"), this->size()),
                                    "readDouble");
                thiz.read_pos = view;
            }
            if constexpr (use_big_endian)
            {
                return thiz.reverse_endian(this->template read<double>());
            }
            else
            {
                return this->template read<double>();
            }
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto readBoolean(
            Args... args) const -> bool
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                auto view = std::get<0>(std::make_tuple(args...));
                assert_conditional(view < this->size(), fmt::format("{}, {}: {}, {}: {}", Language::get("buffer.read_offset_outside_bounds_of_dataview"), Language::get("buffer.new_position"), this->write_pos, Language::get("buffer.actual_size"), this->size()),
                                    "readBoolean");
                thiz.read_pos = view;
            }
            return thiz.readUint8() == 0x01;
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto readString(
            std::size_t size,
            Args... args) const -> std::string
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                auto view = std::get<0>(std::make_tuple(args...));
                assert_conditional(view < this->size(), fmt::format("{}, {}: {}, {}: {}", Language::get("buffer.read_offset_outside_bounds_of_dataview"), Language::get("buffer.new_position"), this->write_pos, Language::get("buffer.actual_size"), this->size()),
                                    "readString");
                thiz.read_pos = view;
            }
            auto str = std::string{};
            for (auto i : Range(size))
            {
                str += static_cast<char>(thiz.readUint8());
            }
            return str;
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto readCharByInt16(
            Args... args) const -> wchar_t
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                auto view = std::get<0>(std::make_tuple(args...));
                assert_conditional(view < this->size(), fmt::format("{}, {}: {}, {}: {}", Language::get("buffer.read_offset_outside_bounds_of_dataview"), Language::get("buffer.new_position"), this->write_pos, Language::get("buffer.actual_size"), this->size()),
                                    "readCharByInt16");
                thiz.read_pos = view;
            }
            auto value = static_cast<wchar_t>(thiz.readInt16());
            return value;
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto readStringByUint8(
            Args... args) const -> std::string
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                auto view = std::get<0>(std::make_tuple(args...));
                assert_conditional(view < this->size(), fmt::format("{}, {}: {}, {}: {}", Language::get("buffer.read_offset_outside_bounds_of_dataview"), Language::get("buffer.new_position"), this->write_pos, Language::get("buffer.actual_size"), this->size()),
                                    "readStringByUint8");
                thiz.read_pos = view;
            }
            return thiz.readString(thiz.readUint8());
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto readStringByUint16(
            Args... args) const -> std::string
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                auto view = std::get<0>(std::make_tuple(args...));
                assert_conditional(view < this->size(), fmt::format("{}, {}: {}, {}: {}", Language::get("buffer.read_offset_outside_bounds_of_dataview"), Language::get("buffer.new_position"), this->write_pos, Language::get("buffer.actual_size"), this->size()),
                                    "readStringByUint16");
                thiz.read_pos = view;
            }
            return thiz.readString(thiz.readUint16());
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto readStringByUint32(
            Args... args) const -> std::string
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                auto view = std::get<0>(std::make_tuple(args...));
                assert_conditional(view < this->size(), fmt::format("{}, {}: {}, {}: {}", Language::get("buffer.read_offset_outside_bounds_of_dataview"), Language::get("buffer.new_position"), this->write_pos, Language::get("buffer.actual_size"), this->size()),
                                    "readStringByUint32");
                thiz.read_pos = view;
            }
            return thiz.readString(thiz.readUint32());
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto readStringByInt8(
            Args... args) const -> std::string
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                auto view = std::get<0>(std::make_tuple(args...));
                assert_conditional(view < this->size(), fmt::format("{}, {}: {}, {}: {}", Language::get("buffer.read_offset_outside_bounds_of_dataview"), Language::get("buffer.new_position"), this->write_pos, Language::get("buffer.actual_size"), this->size()),
                                    "readStringByInt8");
                thiz.read_pos = view;
            }
            return thiz.readString(thiz.readInt8());
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto readStringByInt16(
            Args... args) const -> std::string
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                auto view = std::get<0>(std::make_tuple(args...));
                assert_conditional(view < this->size(), fmt::format("{}, {}: {}, {}: {}", Language::get("buffer.read_offset_outside_bounds_of_dataview"), Language::get("buffer.new_position"), this->write_pos, Language::get("buffer.actual_size"), this->size()),
                                    "readStringByInt8");
                thiz.read_pos = view;
            }
            return thiz.readString(thiz.readInt16());
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto readStringByInt32(
            Args... args) const -> std::string
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                auto view = std::get<0>(std::make_tuple(args...));
                assert_conditional(view < this->size(), fmt::format("{}, {}: {}, {}: {}", Language::get("buffer.read_offset_outside_bounds_of_dataview"), Language::get("buffer.new_position"), this->write_pos, Language::get("buffer.actual_size"), this->size()),
                                    "readStringByInt32");
                thiz.read_pos = view;
            }
            return thiz.readString(thiz.readInt32());
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto readStringByVarInt32(
            Args... args) const -> std::string
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                auto view = std::get<0>(std::make_tuple(args...));
                assert_conditional(view < this->size(), fmt::format("{}, {}: {}, {}: {}", Language::get("buffer.read_offset_outside_bounds_of_dataview"), Language::get("buffer.new_position"), this->write_pos, Language::get("buffer.actual_size"), this->size()),
                                    "readStringByVarInt32");
                thiz.read_pos = view;
            }
            return thiz.readString(thiz.readVarInt32());
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto readStringByEmpty(
            Args... args) const -> std::string
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                auto view = std::get<0>(std::make_tuple(args...));
                assert_conditional(view < this->size(), fmt::format("{}, {}: {}, {}: {}", Language::get("buffer.read_offset_outside_bounds_of_dataview"), Language::get("buffer.new_position"), this->write_pos, Language::get("buffer.actual_size"), this->size()),
                                    "readStringByEmpty");
                thiz.read_pos = view;
            }
            auto ss = std::string{};
            auto byte = 0;
            while (true)
            {
                if ((byte = thiz.readUint8()) == 0)
                {
                    break;
                }
                ss += (char)byte;
            }
            return ss;
        }

        inline auto getStringByEmpty(std::size_t pos) const -> std::string
        {
            auto thiz_pos = read_pos;
            read_pos = pos;
            auto str = readStringByEmpty();
            read_pos = thiz_pos;
            return str;
        }

        inline auto getBytes(
            size_t from,
            size_t to) const -> List<std::uint8_t>
        {
            auto bytes = List<std::uint8_t>{};
            bytes.assign(thiz.data.begin() + from, thiz.data.begin() + to);
            if (use_big_endian)
            {
                std::reverse(bytes.begin(), bytes.end());
            }
            return bytes;
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto readBytes(
            std::size_t size,
            Args... args) const -> List<std::uint8_t>
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                auto view = std::get<0>(std::make_tuple(args...));
                assert_conditional(view < this->size(), fmt::format("{}, {}: {}, {}: {}", Language::get("buffer.read_offset_outside_bounds_of_dataview"), Language::get("buffer.new_position"), this->write_pos, Language::get("buffer.actual_size"), this->size()),
                                    "readBytes");
                thiz.read_pos = view;
            }
            auto bytes = List<std::uint8_t>{};
            bytes.assign(thiz.data.begin() + thiz.read_pos, thiz.data.begin() + thiz.read_pos + size);
            thiz.read_pos += size;
            return bytes;
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto readVarInt32(
            Args... args) const -> std::int32_t
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                auto view = std::get<0>(std::make_tuple(args...));
                assert_conditional(view < this->size(), fmt::format("{}, {}: {}, {}: {}", Language::get("buffer.read_offset_outside_bounds_of_dataview"), Language::get("buffer.new_position"), this->write_pos, Language::get("buffer.actual_size"), this->size()),
                                    "readVarInt32");
                thiz.read_pos = view;
            }
            auto num = 0;
            auto num_2 = 0;
            auto byte = 0;
            do
            {
                if (num_2 == 35)
                {
                    throw Exception(fmt::format("{} {}", Language::get("invalid_varint_number"), num_2),
                                    std::source_location::current(), "readVarInt32");
                }
                byte = thiz.readUint8();
                num |= ((byte & 0x7F) << num_2);
                num_2 += 7;
            } while ((byte & 0x80) != 0);
            return num;
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto readVarInt64(
            Args... args) const -> std::int64_t
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                auto view = std::get<0>(std::make_tuple(args...));
                assert_conditional(view < this->size(), fmt::format("{}, {}: {}, {}: {}", Language::get("buffer.read_offset_outside_bounds_of_dataview"), Language::get("buffer.new_position"), this->write_pos, Language::get("buffer.actual_size"), this->size()),
                                    "readVarInt64");
                thiz.read_pos = view;
            }
            auto num = int64_t{0};
            auto num_2 = 0;
            auto byte = uint8_t{0};
            do
            {
                if (num_2 == 70)
                {
                    throw Exception(fmt::format("{} {}", Language::get("invalid_varint_number"), num_2),
                                    std::source_location::current(), "readVarInt64");
                }
                byte = thiz.readUint8();
                num |= (static_cast<int64_t>(byte & 0x7F) << num_2);
                num_2 += 7;
            } while ((byte & 0x80) != 0);
            return num;
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto readVarUint32(
            Args... args) const -> std::uint32_t
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                auto view = std::get<0>(std::make_tuple(args...));
                assert_conditional(view < this->size(), fmt::format("{}, {}: {}, {}: {}", Language::get("buffer.read_offset_outside_bounds_of_dataview"), Language::get("buffer.new_position"), this->write_pos, Language::get("buffer.actual_size"), this->size()),
                                    "readVarUint32");
                thiz.read_pos = view;
            }
            return static_cast<std::uint32_t>(thiz.readVarInt32());
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto readVarUint64(
            Args... args) const -> std::uint64_t
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                auto view = std::get<0>(std::make_tuple(args...));
                assert_conditional(view < this->size(), fmt::format("{}, {}: {}, {}: {}", Language::get("buffer.read_offset_outside_bounds_of_dataview"), Language::get("buffer.new_position"), this->write_pos, Language::get("buffer.actual_size"), this->size()),
                                    "readVarUint64");
                thiz.read_pos = view;
            }
            return static_cast<std::uint64_t>(thiz.readVarInt64());
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto readZigZag32(
            Args... args) const -> std::int32_t
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                auto view = std::get<0>(std::make_tuple(args...));
                assert_conditional(view < this->size(), fmt::format("{}, {}: {}, {}: {}", Language::get("buffer.read_offset_outside_bounds_of_dataview"), Language::get("buffer.new_position"), this->write_pos, Language::get("buffer.actual_size"), this->size()),
                                    "readZigZag32");
                thiz.read_pos = view;
            }
            auto zigzag_num = thiz.readVarInt32();
            auto decoded = static_cast<std::int32_t>((zigzag_num >> 1) ^ -(zigzag_num & 1));
            return decoded;
        }

        inline auto begin_pointer (

        ) -> std::uint8_t*
        {
            return thiz.data.data();
        }

        template <typename... Args>
            requires(IsValidArgument<Args> && ...)
        inline auto readZigZag64(
            Args... args) const -> std::int64_t
        {
            static_assert(sizeof...(Args) == 1 || sizeof...(Args) == 0, "Expected 0 or 1 argument only");
            if constexpr (sizeof...(Args) == 1)
            {
                auto view = std::get<0>(std::make_tuple(args...));
                assert_conditional(view < this->size(), fmt::format("{}, {}: {}, {}: {}", Language::get("buffer.read_offset_outside_bounds_of_dataview"), Language::get("buffer.new_position"), this->write_pos, Language::get("buffer.actual_size"), this->size()),
                                    "readZigZag64");
                thiz.read_pos = view;
            }
            auto zigzag_num = thiz.readVarInt64();
            auto decoded = static_cast<std::int64_t>((zigzag_num >> 1) ^ -(zigzag_num & 1));
            return decoded;
        }

        template <typename T>
            requires std::is_integral_v<T> || std::is_floating_point_v<T>
        inline auto static reverse_endian(T num) -> T
        {
            auto bytes = std::array<uint8_t, sizeof(T)>{};
            std::memcpy(bytes.data(), &num, sizeof(T));
            std::reverse(bytes.begin(), bytes.end());
            return std::bit_cast<T>(bytes);
        }

        template <typename T>
            requires std::is_integral_v<T> || std::is_floating_point_v<T>
        inline auto read(

        ) const -> T
        {
            if (thiz.read_pos + sizeof(T) > thiz.size())
            {
                throw Exception(fmt::format("{}, {}: thiz.read_pos + sizeof(T) <= thiz.size(), {}: {} + {} <= {}, source: {}", Language::get("offset_outside_bounds_of_data_stream"), Language::get("conditional"), Language::get("but_received"), thiz.read_pos, sizeof(T), thiz.size(), thiz.file_path),
                                std::source_location::current(), "read");
            }
            auto value = T{0};
            std::memcpy(&value, thiz.data.data() + thiz.read_pos, sizeof(T));
            thiz.read_pos += sizeof(T);
            return value;
        }

        template <typename T>
            requires std::is_integral_v<T> || std::is_floating_point_v<T>
        inline auto read_has(
            std::size_t size) const -> T
        {
            if (thiz.read_pos + size > thiz.size())
            {
                throw Exception(fmt::format("{}, {}: thiz.read_pos + sizeof(T) <= thiz.size(), {}: {} + {} <= {}", Language::get("offset_outside_bounds_of_data_stream"), Language::get("conditional"), Language::get("but_received"), thiz.read_pos, sizeof(T), thiz.size()),
                                std::source_location::current(), "read_has");
            }
            auto value = T{0};
            std::memcpy(&value, thiz.data.data() + thiz.read_pos, size);
            this->read_pos += size;
            return value;
        }

        inline auto constexpr close(

        ) const -> void
        {
            thiz.data.clear();
            thiz.length = 0_size;
            thiz.read_pos = 0_size;
            thiz.write_pos = 0_size;
            return;
        }
    };

    using DataStreamView = Stream<false>;

    using DataStreamViewBigEndian = Stream<true>;
}
