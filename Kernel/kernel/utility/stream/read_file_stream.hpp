#pragma once

#include "kernel/utility/stream/readable_stream.hpp"

namespace Sen::Kernel {

    class ReadFileStream : public ReadableStream<ReadFileStream> {

     protected:

        using Base = ReadableStream;

        using File = FileSystem::FileHandler;

    protected:

        File m_file;

        usize m_size;

    public:

        explicit ReadFileStream(
            const StringView &source
        ) : Base{}, m_file{FileSystem::open_read(source)}, m_size{FileSystem::size_file(source)}
        {
        }

        ReadFileStream(
            const ReadFileStream &other
        ) = delete;

        auto operator=(
            const ReadFileStream &other
        ) -> ReadFileStream & = delete;

        ReadFileStream(
            ReadFileStream &&other
        ) = delete;

        auto operator=(
            ReadFileStream &&
        ) -> ReadFileStream & = delete;

         auto string(
            const usize& from,
            const usize &to
        ) -> String override {
            assert_conditional(from <= thiz.m_size, "From index must be smaller than Stream size", "string");
            assert_conditional(to <= thiz.m_size, "To index must be smaller than Stream size", "string");
            assert_conditional(from < to, "From index must be smaller than to index", "string");
            auto destination = String{to - from};
            thiz.m_file.read(reinterpret_cast<uint8_t*>(destination.data()), to - from);
            thiz.m_position = to;
            return destination;
        }

        template<typename T> requires is_numeric_v<T>
        auto read_impl(
            const usize &size
        ) -> T {
            assert_conditional(thiz.m_position + sizeof(T) <= thiz.m_size, "Outside bounds of ReadFileStream", "read");
            auto value = T{};
            thiz.m_file.read(reinterpret_cast<uint8_t*>(&value), size);
            thiz.m_position += size;
            return value;
        }

        template<typename T> requires is_numeric_v<T>
        auto read_impl(

        ) -> T {
            assert_conditional(thiz.m_position + sizeof(T) <= thiz.m_size, "Outside bounds of ReadFileStream","read");
            auto value = T{};
            thiz.m_file.read(reinterpret_cast<uint8_t*>(&value), sizeof(value));
            thiz.m_position += sizeof(T);
            return value;
        }

        auto set_position(
            const usize &index
        ) -> void {
             assert_conditional(index < thiz.m_file.size(), "Index must be smaller than data size", "set_position");
             thiz.m_position = index;
         }


        auto operator +=(
           const usize& index
       ) -> ReadFileStream& {
             assert_conditional(index <= thiz.m_file.size(), "Index must be smaller than data size", "operator_plus");
             thiz.m_position += index;
             return thiz;
         }

        auto operator -=(
            const usize& index
        ) -> ReadFileStream& {
             assert_conditional(index <= thiz.m_file.size(), "Index must be smaller than data size", "operator_minus");
             thiz.m_position -= index;
             return thiz;
         }


        auto read(
            const usize &from,
            const usize &to
        ) -> Uint8Array override {
            assert_conditional(from <= thiz.m_size, "From index must be smaller than Stream size", "read");
            assert_conditional(to <= thiz.m_size, "To index must be smaller than Stream size", "read");
            assert_conditional(from < to, "From index must be smaller than to index", "read");
            auto destination = Uint8Array{to - from};
            thiz.m_file.read(destination.begin(), to - from);
            thiz.m_position = to;
            return destination;
        }

        constexpr auto size(

        ) const -> usize {
            return thiz.m_size;
        }

        auto view(

        ) -> File & {
            return thiz.m_file;
        }


    };

}