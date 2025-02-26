#pragma once

#include "kernel/utility/stream/readable_stream.hpp"

namespace Sen::Kernel {

    class ReadMemoryStream : public ReadableStream<ReadMemoryStream> {

    protected:

        Uint8Array m_data{};

     private:

        using Base = ReadableStream;

    public:

        explicit ReadMemoryStream(
            Uint8Array &source
        ) : Base{}, m_data{} {
            m_data.assign(source);
        }

        explicit ReadMemoryStream(
            const StringView &source
        ) : Base{}, m_data{}
        {
            FileSystem::read_file(source, m_data);
        }

        ReadMemoryStream(
            const ReadMemoryStream &other
        ) = delete;

        auto operator=(
            const ReadMemoryStream &other
        ) -> ReadMemoryStream & = delete;

        ReadMemoryStream(
            ReadMemoryStream &&other
        ) = delete;

        auto operator=(
            ReadMemoryStream &&
        ) -> ReadMemoryStream & = delete;

         auto string(
            const usize& from,
            const usize &to
         ) -> String override {
            assert_index(from <= thiz.m_data.size(), "From index must be smaller than Stream size", "string");
            assert_index(to <= thiz.m_data.size(), "To index must be smaller than Stream size", "string");
            assert_index(from <= to, "From index must be smaller than to index", "string");
            auto destination = String{to - from};
            std::memcpy(destination.data(), thiz.m_data.begin() + from, to - from);
            thiz.m_position = to;
            return destination;
         }

        auto string(
            const usize& size
         ) -> String override {
             return thiz.string(m_position, m_position + size);
         }

        template<typename T> requires is_numeric_v<T>
        auto read_impl(
            const usize &size
        ) -> T {
            assert_conditional(thiz.m_position + sizeof(T) <= thiz.m_data.size(), "Outside bounds of ReadMemoryStream", "read");
            auto value = T{};
            std::memcpy(&value, thiz.m_data.begin() + thiz.m_position, size);
            thiz.m_position += size;
            return value;
        }

        template<typename T> requires is_numeric_v<T>
        auto read_impl(

        ) -> T {
            assert_conditional(thiz.m_position + sizeof(T) <= thiz.m_data.size(), "Outside bounds of ReadMemoryStream","read");
            auto value = T{};
            std::memcpy(&value, thiz.m_data.begin() + thiz.m_position, sizeof(T));
            thiz.m_position += sizeof(T);
            return value;
        }

        auto read(
            const usize &from,
            const usize &to
        ) -> Uint8Array override {
            assert_index(from <= thiz.m_data.size(), "From index must be smaller than Stream size", "read");
            assert_index(to <= thiz.m_data.size(), "To index must be smaller than Stream size", "read");
            assert_index(from <= to, "From index must be smaller than to index", "read");
            auto destination = Uint8Array{to - from};
            std::memcpy(destination.data(), thiz.m_data.begin() + from, to - from);
            thiz.m_position = to;
            return destination;
        }

        constexpr auto size(

        ) const -> usize {
            return thiz.m_data.size();
        }

        constexpr auto current_iterator(

        ) const -> decltype(thiz.m_data.begin() + thiz.m_position) {
            return thiz.m_data.begin() + thiz.m_position;
        }

        constexpr auto begin() const -> decltype(thiz.m_data.begin()) {
            return thiz.m_data.begin();
        }

        constexpr auto end() const -> decltype(thiz.m_data.end()) {
            return thiz.m_data.end();
        }

        auto view(

        ) -> Uint8Array & {
            return thiz.m_data;
        }

        constexpr auto set_position(
            const usize &index
        ) -> void {
            assert_conditional(index < thiz.m_data.size(), "Index must be smaller than data size", "set_position");
            thiz.m_position = index;
        }

        auto operator +=(
            const usize& index
        ) -> ReadMemoryStream& {
			assert_conditional(index <= thiz.m_data.size(), "Index must be smaller than data size", "operator_plus");
            thiz.m_position += index;
            return thiz;
        }

        auto operator -=(
            const usize& index
        ) -> ReadMemoryStream& {
            assert_conditional(index <= thiz.m_data.size(), "Index must be smaller than data size", "operator_minus");
            thiz.m_position -= index;
            return thiz;
        }

        auto has_space(
            const usize& index
        ) const -> bool {
            return thiz.m_position + index <= thiz.m_data.size();
        }

        auto release_stream (
            Uint8Array& that
        ) -> void {
            that.assign(thiz.m_data);
        }

        auto release_stream (
            Uint8List& that
        ) -> void {
            that.assign(thiz.m_data);
        }

        auto read_view (
            const usize& from, 
            const usize& to
        ) -> Uint8ArrayView {
            assert_index(from <= thiz.m_data.size(), "From index must be smaller than Stream size", "read_view");
            assert_index(to <= thiz.m_data.size(), "To index must be smaller than Stream size", "read_view");
            assert_index(from <= to, "From index must be smaller than to index", "read_view");
            auto destination = Uint8ArrayView{thiz.m_data.begin() + from, to - from};
            thiz.m_position = to;
            return destination;
        }

        auto subview(
            const usize&offset, 
            const usize& size
        ) -> Uint8ArrayView {
            const auto current_position = m_position;
            auto value = read_view(offset, offset + size);
            thiz.m_position = current_position;
            return value;
        }

    };

}