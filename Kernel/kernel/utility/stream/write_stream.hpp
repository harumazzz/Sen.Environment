#pragma once

#include "kernel/utility/stream/base_stream.hpp"

namespace Sen::Kernel {

    class WriteStream : public WritableStream {

		protected:

			Uint8List m_data{};

			usize m_position{0};

		public:

			template <typename T> requires std::is_same_v<T, Uint8Array> || std::is_same_v<T, Uint8List>
			explicit WriteStream(T& source) : m_data{}, m_position{ 0 } {
				m_data.assign(source);
			}

			constexpr explicit WriteStream(
			) = default;

			~WriteStream(

			) = default;

			WriteStream(
				const WriteStream& other
			) = delete;

			auto operator=(
				const WriteStream& other
			)->WriteStream & = delete;

			WriteStream(
				WriteStream&& other
			) = delete;

			auto operator=(
				WriteStream&&
			)->WriteStream & = delete;

			constexpr auto current_position(

			) -> usize
			{
				return thiz.m_position;
			}

			constexpr auto size(

			) -> usize
			{
				return thiz.m_data.size();
			}

			constexpr auto current_iterator(

			) -> decltype(thiz.m_data.begin() + thiz.m_position)
			{
				return thiz.m_data.begin() + thiz.m_position;
			}

			constexpr auto view(

			) -> Uint8List&
			{
				return thiz.m_data;
			}

			constexpr auto set_position(
				const usize& index
			) -> void
			{
				assert_conditional(index <= thiz.m_data.capacity(), "Index must be smaller than capacity", "set_position");
				thiz.m_position = index;
			}

			auto allocate_full(
				const usize& new_size
			) -> void
			{
				if (new_size > thiz.m_data.capacity())
				{
					thiz.m_data.reallocate(new_size);
				}
			}

			template <typename T> requires std::is_arithmetic_v<T>
			auto write(
				const T& value
			) -> void
			{
				constexpr auto sz = sizeof(T);
				auto temporary = thiz.m_position + sz;
				thiz.allocate_full(temporary);
				thiz.m_data.size(thiz.m_data.size() + sz);
				std::memcpy(&thiz.m_data[thiz.m_position], &value, sz);
				thiz.set_position(temporary);
			}

    		template <typename T> requires (std::is_same_v<T, Uint8Array> or std::is_same_v<T, Uint8List>) && requires(T a) {
					{ a.size() } -> std::convertible_to<usize>;
					{ a.begin() } -> std::convertible_to<u8*>;
    		}
			auto write(
				T& value
			) -> void
			{
				auto temporary = value.size() + thiz.m_position;
				thiz.allocate_full(temporary);
				thiz.m_data.size(thiz.m_data.size() + value.size());
				std::memcpy(&thiz.m_data[thiz.m_position], value.begin(), value.size());
				thiz.set_position(temporary);
			}
	};

}