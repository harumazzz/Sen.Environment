#pragma once

#include "kernel/utility/javascript/generalization.hpp"

namespace Sen::Kernel::JavaScript {

	template <typename Value, typename Child> requires (std::is_class<Value>::value && std::is_class<Child>::value && !std::is_pointer<Value>::value && !std::is_pointer<Child>::value)
	inline static auto to_array_of_string(
		JSContext* context,
		Value* list
	) -> JSValue
	{
		auto destination = JS_NewArray(context);
		for (auto index : Range<std::size_t>(list->size)) {
			auto atom_index = JS_NewAtomUInt32(context, static_cast<std::uint32_t>(index));
			auto& str_view = list->value[index];
			auto js_str = JS_NewStringLen(context, reinterpret_cast<const char*>(str_view.value), str_view.size);
			JS_SetProperty(context, destination, atom_index, js_str);
			JS_FreeAtom(context, atom_index);
		}
		return destination;
	}

	template <typename Value, typename Child> requires (std::is_class<Value>::value && std::is_class<Child>::value && !std::is_pointer<Value>::value && !std::is_pointer<Child>::value)
	inline static auto to_string_list(
		JSContext* context,
		JSValue value,
		Value& list
	) -> void
	{
		auto length_atom = JS_NewAtomLen(context, "length", std::strlen("length"));
		auto array_length = Converter::get_property_uint32(context, value, length_atom);
		JS_FreeAtom(context, length_atom);
		list.size = array_length;
		list.value = new Child[array_length];
		for (auto i : Range{ array_length }) {
			auto atom = JS_NewAtomUInt32(context, i);
			auto js_element = JS_GetProperty(context, value, atom);
			JS_FreeAtom(context, atom);
			auto str_len = size_t{};
			auto str = JS_ToCStringLen(context, &str_len, js_element);
			auto temporary = std::unique_ptr<uint8_t[]>{new uint8_t[str_len + 1]};
			temporary.get()[str_len] = '\0';
			std::memcpy(temporary.get(), str, str_len);
			list.value[i].value = temporary.release();
			list.value[i].size = str_len;
			JS_FreeCString(context, str);
			JS_FreeValue(context, js_element);
		}
		return;
	}

	template <typename Value> requires (std::is_class<Value>::value && !std::is_pointer<Value>::value)
	inline static auto to_string(
		JSContext* context,
		Value* value
	) -> JSValue
	{
		return JS_NewStringLen(context, reinterpret_cast<const char*>(value->value), value->size);
	}

	template <typename T> requires is_container<T>::value && std::is_class<T>::value
	static auto make_array(
		JSContext* context,
		const T& value
	) -> JSValue
	{
		return Converter::to_array<T>(context, value);
	}

	template <typename T> requires is_map<T>::value && std::is_class<T>::value
	static auto make_object(
		JSContext* context,
		const T& value
	) -> JSValue
	{
		// TODO
		return JS_UNDEFINED;
	}

	template <typename T>
	static auto to_value(JSContext* context, const T& value) -> JSValue {
		if constexpr (std::is_same<T, bool>::value) {
			return Converter::to_bool(context, value);
		} 
		else if constexpr (std::is_integral<T>::value && !std::is_floating_point<T>::value && !std::is_pointer<T>::value) {
			static_assert(sizeof(T) != sizeof(bool), "value cannot be bool");
			return Converter::to_bigint<T>(context, value);
		} 
		else if constexpr (std::is_floating_point<T>::value) {
			return Converter::to_number(context, value);
		} 
		else if constexpr (std::is_same<T, std::string>::value || std::is_same<T, std::string_view>::value) {
			return Converter::to_string(context, value);
		} 
		else if constexpr (is_map<T>::value) {
			return make_object<T>(context, value);
		} 
		else if constexpr (is_container<T>::value) {
			return make_array<T>(context, value);
		} 
		else if constexpr (std::is_same<T, JSValue>::value) {
			return value;
		} 
		else {
			static_assert(false, "to_value case not implemented for this type");
		}
	}

	template <typename T>
	auto from_value(
		JSContext* context,
		JSValue value
	) -> T
	{
		static_assert(false, "Base case need to be implemented!");
	}

	template <>
	auto from_value<std::string>(
		JSContext* context,
		JSValue value
	) -> std::string
	{
		return Converter::get_string(context, value);
	}

	template <>
	auto from_value<bool>(
		JSContext* context,
		JSValue value
	) -> bool
	{
		return Converter::get_bool(context, value);
	}

	

	template <>
	auto from_value<JSValue>(
		JSContext* context,
		JSValue value
	) -> JSValue
	{
		return value;
	}

	template <typename T> requires (std::is_integral_v<T> && !std::is_unsigned<T>::value)
	auto from_value(
		JSContext* context,
		JSValue value
	) -> T
	{
		return Converter::get_bigint64(context, value);
	}

	template <typename T> requires (std::is_integral_v<T> && std::is_unsigned<T>::value)
	auto from_value(
		JSContext* context,
		JSValue value
	) -> T
	{
		return Converter::get_uint64(context, value);
	}

	template <typename T> requires (!std::is_integral_v<T> && std::is_floating_point<T>::value)
	auto from_value(
		JSContext* context,
		JSValue value
	) -> double
	{
		return static_cast<double>(Converter::to_number(context, value));
	}

	struct ArrayBuffer {
		uint8_t* value{ nullptr };
		size_t size{ 0 };
	};

	// ArrayBuffer
	template <>
	auto from_value<std::shared_ptr<ArrayBuffer>>(
		JSContext* context,
		JSValue value
	) -> std::shared_ptr<ArrayBuffer>
	{
		auto destination = std::make_shared<ArrayBuffer>();
		destination->value = Converter::get_array_buffer(context, &destination->size, value);
		return destination;
	}

}