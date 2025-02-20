#pragma once

#include "kernel/support/popcap/reflection_object_notation/common.hpp"
#include "kernel/utility/stream/writeable_stream.hpp"


namespace Sen::Kernel {
    class WriteMemoryStream : public WritableStream<WriteMemoryStream> {
    protected:
        Uint8List m_data{};

    private:
        using Base = WritableStream;

    public:
        template<typename T>
            requires std::is_same_v<T, Uint8Array> || std::is_same_v<T, Uint8List>
        explicit WriteMemoryStream(T &source) : Base{}, m_data{} {
            thiz.m_data.assign(source);
        }


        explicit WriteMemoryStream(usize const &size) : Base{}, m_data{} {
            thiz.m_data.allocate(size);
        }

        explicit WriteMemoryStream() : Base{}, m_data{} {
        }

        WriteMemoryStream(
            const WriteMemoryStream &other
        ) = delete;

        auto operator=(
            const WriteMemoryStream &other
        ) -> WriteMemoryStream & = delete;

        WriteMemoryStream(
            WriteMemoryStream &&other
        ) = delete;

        auto operator=(
            WriteMemoryStream &&
        ) -> WriteMemoryStream & = delete;

        constexpr auto size(

        ) const -> usize {
            return thiz.m_data.size();
        }

        constexpr auto resize(
            const usize& size
        ) -> void {
            return thiz.m_data.resize(size);
        }

        constexpr auto current_iterator(

        ) -> decltype(thiz.m_data.begin() + thiz.m_position) {
            return thiz.m_data.begin() + thiz.m_position;
        }

        constexpr auto begin(

        ) -> decltype(thiz.m_data.begin()) {
            return thiz.m_data.begin();
        }

        auto capacity(
        ) -> usize {
            return thiz.m_data.capacity();
        }

        constexpr auto end(

        ) -> decltype(thiz.m_data.end()) {
            return thiz.m_data.end();
        }

        auto view(

        ) -> Uint8List & {
            return thiz.m_data;
        }

        constexpr auto set_position(
            const usize &index
        ) -> void {
            assert_conditional(index <= thiz.m_data.capacity(), "Index must be smaller than capacity", "set_position");
            thiz.m_position = index;
        }

        auto allocate_full(
            const usize &new_size
        ) -> void {
            if (new_size > thiz.m_data.capacity()) {
                thiz.m_data.reallocate(new_size + 640000);
            }
        }

        auto allocate_size(const usize &size) -> void {
            auto temporary = size + thiz.m_position;
            thiz.allocate_full(temporary);
            thiz.m_data.resize(thiz.m_data.size() + size);
            thiz.set_position(temporary);
        }

        auto raw(
            const uint8_t *data,
            const usize &size
        ) -> void {
            auto temporary = size + thiz.m_position;
            thiz.allocate_full(temporary);
            thiz.m_data.resize(thiz.m_data.size() + size);
            std::memcpy(&thiz.m_data[thiz.m_position], data, size);
            thiz.set_position(temporary);
        }

        template<typename T> requires std::is_arithmetic_v<T>
        auto write_impl(
            const T &value
        ) -> void {
            constexpr auto sz = sizeof(T);
            auto temporary = thiz.m_position + sz;
            thiz.allocate_full(temporary);
            thiz.m_data.resize(thiz.m_data.size() + sz);
            std::memcpy(&thiz.m_data[thiz.m_position], &value, sz);
            thiz.set_position(temporary);
        }

        template<typename... Args> requires (sizeof...(Args) > 1 && (is_numeric_v<Args> && ...))
        auto write_impl(
            const Args &... args
        ) -> void {
            auto temporary = Uint8Array{total_sizeof<Args...>()}; {
                auto offset = 0_size;
                (forward_bytes(std::forward<Args>(args), temporary, offset), ...);
            }
            thiz.write_impl(temporary);
        }

        template<typename T> requires (std::is_same_v<T, Uint8Array> or std::is_same_v<T, Uint8List>) && requires(T a)
        {
            { a.size() } -> std::convertible_to<usize>;
            { a.begin() } -> std::convertible_to<uint8_t *>;
        }
        auto write_impl(
            T &value
        ) -> void {
            return thiz.raw(value.begin(), value.size());
        }

        auto operator +=(
            const usize &index
        ) -> WriteMemoryStream & {
            assert_conditional(index <= thiz.m_data.capacity(), "Index must be smaller than data size", "operator_plus");
            thiz.m_position += index;
            return thiz;
        }

        auto operator -=(
            const usize &index
        ) -> WriteMemoryStream & {
            assert_conditional(index <= thiz.m_data.capacity(), "Index must be smaller than data size", "operator_minus");
            thiz.m_position -= index;
            return thiz;
        }

        auto has_space(
            const usize &index
        ) -> bool {
            return thiz.m_position + index <= thiz.m_data.capacity();
        }

        auto release_stream(
            Uint8Array &that
        ) -> void {
            that.assign(thiz.m_data);
        }

        auto release_stream(
            Uint8List &that
        ) -> void {
            that.assign(thiz.m_data);
        }


    };
}
