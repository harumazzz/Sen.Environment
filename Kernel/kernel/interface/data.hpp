#pragma once

#include "kernel/utility/library.hpp"
#include "kernel/utility/macro.hpp"
#include "kernel/utility/container/array.hpp"

namespace Sen::Kernel::Interface {

	template <typename T, std::size_t N>
	using Array = std::array<T, N>;

	struct StringView {

		const char* value{nullptr};

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
			for (auto i : Range(list.size)) {
				clean_up<StringView>(list.value[i]);
			}
			delete[] list.value;
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
	auto finalizer<StringView>(
		StringView* ptr
	) -> void
	{
		if (ptr != nullptr) {
			if (ptr->value != nullptr) {
				delete[] ptr->value;  
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
		std::string_view that
	) -> StringView
	{
		return StringView{
			.value = that.data(),
			.size = that.size(),
		};
	}

	inline static auto construct_string(
		StringView* that
	) -> std::string
	{
		return std::string{
			that->value,
			that->size,
		};
	}

	inline static auto construct_string_list(
		const List<std::string>& that
	) -> std::shared_ptr<CStringList>
	{
		auto destination = std::make_shared<CStringList>(new StringView[that.size()], that.size());
		for (auto i : Range(that.size())) {
			destination->value[i] = construct_string(that[i]);
		}
		return destination;
	}

	template <std::size_t N>
	inline static auto construct_string_list(
		const Array<std::string, N>& that
	) -> std::shared_ptr<CStringList>
	{
		auto destination = std::make_shared<CStringList>(new StringView[N], N);
		for (auto i : Range(N)) {
			destination->value[i] = construct_string(that[i]);
		}
		return destination;
	}

	inline static auto destruct_string_list(
		CStringList* that
	) -> List<std::string>
	{
		auto destination = List<std::string>{};
		destination.reserve(that->size);
		for (auto i : Range(static_cast<std::size_t>(that->size))) {
			destination.emplace_back(std::string{ that->value[i].value, static_cast<std::size_t>(that->value[i].size) });
		}
		return destination;
	}
	

}