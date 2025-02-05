#pragma once

#include "kernel/utility/library.hpp"
#include "kernel/utility/macro.hpp"
#include "kernel/utility/container/string/basic_string.hpp"

namespace Sen::Kernel::Interface {

	struct StringView {

		uint8_t* value{nullptr};

		size_t size{0};

	};

	struct StringList {

		StringView* value{nullptr};

		size_t size{0};

	};

	template <typename T>
	auto clean_up(
		T& value
	) -> void
	{
		return;
	}

	template <>
	auto clean_up<StringView>(
		StringView& str
	) -> void
	{
		if (str.value != nullptr) {
			delete[] str.value;
			str.value = nullptr;
		}
		return;
	}

	template <>
	auto clean_up<StringList>(
		StringList& list
	) -> void
	{
		if (list.value != nullptr) {
			for (auto i : Range{list.size}) {
				clean_up<StringView>(list.value[i]);
			}
			delete[] list.value;
			list.value = nullptr;
		}
		return;
	}

	template <typename T>
	auto finalizer(
		T* ptr
	) -> void
	{
		if (ptr != nullptr) {
			delete ptr;  
		}
	}

	template <>
	auto finalizer<StringList>(
		StringList* ptr
	) -> void
	{
		if (ptr != nullptr) {
			for (auto i : Range{ptr->size}) {
				clean_up<StringView>(ptr->value[i]);
			}
			delete[] ptr->value;
			delete ptr;  
		}
	}

	template <>
	auto finalizer<StringView>(
		StringView* ptr
	) -> void
	{
		if (ptr != nullptr) {
			if (ptr->value != nullptr) {
				delete[] ptr->value;  
				ptr->value = nullptr;
			}
			delete ptr;
		}
	}

	using CStringView = StringView;
	
	using CStringList = StringList;

	using Callback = int(*)(CStringList* list, StringView* destination);

	using StringFinalizer = void(*)(StringView*);

	using StringListFinalizer = void(*)(StringList*);

	inline static auto construct_string(
		std::string_view that,
		StringView& destination
	) -> void
	{
		auto temporary = std::unique_ptr<uint8_t[]>(new uint8_t[that.size() + 1]);
		std::memcpy(temporary.get(), that.data(), that.size());
		temporary.get()[that.size()] = '\0'; 
		destination.value = temporary.release();
		destination.size = that.size();
		return;
	}

	inline static auto construct_string(
		StringView* that
	) -> Kernel::String
	{
		return Kernel::String {
			reinterpret_cast<const char*>(that->value),
			that->size,
		};
	}

	inline static auto construct_string_list(
		const List<std::string>& that,
		CStringList& destination
	) -> void
	{
		destination.value = new StringView[that.size()];
		destination.size = that.size();
		for (auto i : Range(that.size())) {
			construct_string(that[i], destination.value[i]);
		}
		return;
	}

	template <std::size_t N>
	inline static auto construct_string_list(
		const Array<std::string, N>& that,
		CStringList& destination
	) -> void
	{
		destination.value = new StringView[N];
		destination.size = N;
		for (auto i : Range(N)) {
			construct_string(that[i], destination.value[i]);
		}
		return;
	}
	

}