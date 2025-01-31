#pragma once

#include "kernel/utility/stream/base_stream.hpp"

namespace Sen::Kernel {

    class ReadStream : public ReadableStream {

    protected:

        Uint8Array m_data{};

        usize m_position{};

    public:


        explicit ReadStream(
            Uint8Array& source
        ) : m_data{}, m_position{0_size} {
            m_data.assign(source);
        }

        ~ReadStream(

        ) = default;

        ReadStream(
            const ReadStream& other
        ) = delete;

        auto operator=(
            const ReadStream& other
        ) -> ReadStream & = delete;

        ReadStream(
            ReadStream&& other
        ) = delete;

        auto operator=(
            ReadStream&&
        ) -> ReadStream& = delete;

        template <typename T>
        auto read(

        )  -> T
        {
            assert_conditional(thiz.m_position + sizeof(T) <= thiz.m_data.size(), "Outside bounds of ReadStream", "read");
            auto value = T{};
            std::memcpy(&value, thiz.m_data.begin() + thiz.m_position, sizeof(T));
            thiz.m_position += sizeof(T);
            return value;
        }

        auto read(
            const usize& from,
            const usize& to
        ) -> Uint8Array
        {
            assert_conditional(from <= thiz.m_data.size(), "From index must be smaller than Stream size", "read");
            assert_conditional(to <= thiz.m_data.size(), "To index must be smaller than Stream size", "read");
            assert_conditional(from < to, "From index must be smaller than To index", "read");
            auto destination = Uint8Array{to - from};
            std::memcpy(destination.data(), thiz.m_data.begin() + from, to - from);
            return destination;
        }

        auto current_position(

        ) -> usize
        {
            return thiz.m_position;
        }

        auto size(

        ) -> usize
        {
            return thiz.m_data.size();
        }

        auto current_iterator(

        ) -> decltype(thiz.m_data.begin() + thiz.m_position)
        {
            return thiz.m_data.begin() + thiz.m_position;
        }

        auto view(

        ) -> Uint8Array&
        {
            return thiz.m_data;
        }

        auto set_position(
            const usize& index
        ) -> void
        {
            assert_conditional(index < thiz.m_data.size(), "Index must be smaller than data size", "set_position");
            thiz.m_position = index;
        }

    };

}