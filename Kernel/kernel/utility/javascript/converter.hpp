#pragma once

#include "kernel/utility/javascript/value.hpp"
#include "kernel/utility/javascript/runtime.hpp"

namespace Sen::Kernel::JavaScript::Converter {

    namespace Detail {

        inline auto throw_exception(
			JSContext* context, 
			const JSValue& value, 
			std::string_view type
		) -> void 
		{
			assert_conditional(false, fmt::format("Failed to convert JS value to {}", type), "throw_exception");
        }
    }

	inline static auto get_string (
		JSContext* context, 
		const JSValue& value
	) -> std::string {
		assert_conditional(static_cast<bool>(JS_IsString(value)), "Value must be string, but it isn't", "get_string");
        auto size = std::size_t{};
        auto c_str = JS_ToCStringLen(context, &size, value);
        auto str = std::string { c_str, size };
        JS_FreeCString(context, c_str);
        return str;
    }

    inline static auto get_int32(JSContext* context, const JSValue& value) -> int32_t {
		assert_conditional(static_cast<bool>(JS_IsNumber(value)), "Value must be number, but it isn't", "get_int32");
		auto result = int32_t{};
		JS_ToInt32(context, &result, value);
		return result;
	}

	inline static auto get_float64(JSContext* context, const JSValue& value) -> double {
		assert_conditional(static_cast<bool>(JS_IsNumber(value)), "Value must be number, but it isn't", "get_float64");
		auto result = double{};
		JS_ToFloat64(context, &result, value);
		return result;
	}

	inline static auto get_float32(JSContext* context, const JSValue& value) -> float {
		assert_conditional(static_cast<bool>(JS_IsNumber(value)), "Value must be number, but it isn't", "get_float64");
		auto result = double{};
		JS_ToFloat64(context, &result, value);
		return static_cast<float>(result);
	}

	inline static auto get_bigint64(JSContext* context, const JSValue& value) -> int64_t {
		assert_conditional(static_cast<bool>(JS_IsBigInt(context, value)), "Value must be bigint, but it isn't", "get_bigint64");
		auto result = int64_t{};
		JS_ToBigInt64(context, &result, value);
		return result;
	}

	inline static auto get_uint32(JSContext* context, const JSValue& value) -> uint32_t {
		assert_conditional(static_cast<bool>(JS_IsNumber(value)), "Value must be number, but it isn't", "get_uint32");
		auto result = uint32_t{};
		JS_ToUint32(context, &result, value);
		return result;
	}

	inline static auto get_uint64(JSContext* context, const JSValue& value) -> uint64_t {
		assert_conditional(static_cast<bool>(JS_IsBigInt(context, value)), "Value must be number, but it isn't", "get_uint64");
		auto result = uint64_t{};
		JS_ToBigUint64(context, &result, value);
		return result;
	}

    inline static auto get_bool(JSContext* context, const JSValue& value) -> bool {
		assert_conditional(static_cast<bool>(JS_IsBool(value)), "Value must be boolean, but it isn't", "get_bool");
        return static_cast<bool>(JS_ToBool(context, value));
    }

    inline static auto to_string(JSContext* context, std::string_view content) -> JSValue {
        return JS_NewStringLen(context, content.data(), content.size());
    }

    inline static auto to_bool(JSContext* context, bool value) -> JSValue {
        return JS_NewBool(context, value ? 1 : 0);
    }

    inline static auto to_number(JSContext* context, int32_t value) -> JSValue {
        return JS_NewInt32(context, value);
    }

    inline static auto to_number(JSContext* context, double value) -> JSValue {
        return JS_NewFloat64(context, value);
    }

    inline static auto to_number(JSContext* context, uint32_t value) -> JSValue {
        return JS_NewUint32(context, value);
    }

    inline static auto to_number(JSContext* context, int64_t value) -> JSValue {
        return JS_NewInt64(context, value);
    }

    inline static auto to_number(JSContext* context, uint64_t value) -> JSValue {
        return JS_NewBigUint64(context, value);
    }

	inline auto get_array_buffer (
		JSContext* context,
		size_t* size,
		JSValue& value
	) -> uint8_t*
	{
		auto data = JS_GetArrayBuffer(context, size, value);
		assert_conditional(data != nullptr, "Cannot get instance of ArrayBuffer", "get_array_buffer");
		return data;
	}

	template <typename T>
    inline static auto to_bigint(JSContext* context, T value) -> JSValue {
        if constexpr (std::is_same<T, uint64_t>::value) {
            return JS_NewBigUint64(context, value);
        } else if constexpr (std::is_integral<T>::value) {
            return JS_NewBigInt64(context, static_cast<int64_t>(value));
        }
    }

	template <typename T>
	inline static auto get_array(
		JSContext* context, 
		const JSValue& that
	) -> List<T> {
		auto atom = Atom{context, "length"};
		auto length_value = JS_GetProperty(context, that, atom.value);
		auto length = get_uint32(context, length_value);
		JS_FreeValue(context, length_value);
		auto m_list = List<T>{};
		m_list.reserve(static_cast<std::size_t>(length));
		for (auto i : Range<int>(length)) {
			auto atom = JS_NewAtomUInt32(context, i);
			auto value = JS_GetProperty(context, that, atom);
			if constexpr (std::is_same<T, int>::value) {
				m_list.emplace_back(get_int32(context, value));
			} 
			else if constexpr (std::is_same<T, bool>::value) {
				m_list.emplace_back(get_bool(context, value));
			} 
			else if constexpr (std::is_same<T, long long>::value) {
				m_list.emplace_back(get_bigint64(context, value));
			} 
			else if constexpr (std::is_same<T, uint32_t>::value) {
				m_list.emplace_back(get_uint32(context, value));
			} 
			else if constexpr (std::is_same<T, uint64_t>::value) {
				m_list.emplace_back(get_uint64(context, value));
			} 
			else if constexpr (std::is_same<T, float>::value) {
				m_list.emplace_back(get_float32(context, value));
			} 
			else if constexpr (std::is_same<T, double>::value) {
				m_list.emplace_back(get_float64(context, value));
			} 
			else if constexpr (std::is_same<T, std::string>::value) {
				m_list.emplace_back(get_string(context, value));
			} 
			else {
				static_assert(false, "<% this case has not been implemented %>");
			}
			JS_FreeValue(context, value);
			JS_FreeAtom(context, atom);
		}
		return m_list;
	}

	template <typename Container>
	auto to_array(
		JSContext* context, 
		const Container& container
	) -> JSValue 
	{
		auto destination = JS_NewArray(context);
		for (auto i : Range<std::size_t>(container.size())) {
			auto atom = JS_NewAtomUInt32(context, i);
			if constexpr (std::is_same<typename Container::value_type, int>::value) {
				JS_SetProperty(context, destination, atom, JS_NewInt32(context, container[i]));
			} 
			else if constexpr (std::is_same<typename Container::value_type, uint32_t>::value) {
				JS_SetProperty(context, destination, atom, JS_NewBigInt64(context, static_cast<int64_t>(container[i])));
			} 
			else if constexpr (std::is_same<typename Container::value_type, float>::value) {
				JS_SetProperty(context, destination, atom, JS_NewFloat64(context, static_cast<double>(container[i])));
			} 
			else if constexpr (std::is_same<typename Container::value_type, double>::value) {
				JS_SetProperty(context, destination, atom, JS_NewFloat64(context, container[i]));
			} 
			else if constexpr (std::is_same<typename Container::value_type, uint64_t>::value) {
				JS_SetProperty(context, destination, atom, JS_NewBigUint64(context, container[i]));
			} 
			else if constexpr (std::is_same<typename Container::value_type, bool>::value) {
				JS_SetProperty(context, destination, atom, JS_NewBool(context, container[i]));
			} 
			else if constexpr (std::is_same<typename Container::value_type, long long>::value) {
				JS_SetProperty(context, destination, atom, JS_NewBigInt64(context, container[i]));
			} 
			else if constexpr (std::is_same<typename Container::value_type, std::string>::value) {
				JS_SetProperty(context, destination, atom, JS_NewStringLen(context, container[i].data(), container[i].size()));
			} 
			else if constexpr (std::is_same<typename Container::value_type, JSValue>::value) {
				JS_SetProperty(context, destination, atom, container[i]);
			} 
			else {
				static_assert(false, "<% this case has not been implemented %>");
			}
			JS_FreeAtom(context, atom);
		}
		return destination;
	}


	inline static auto constexpr close_buffer = [](auto buffer)
	{ 
		std::free(buffer); 
		return; 
	};

	inline static auto to_binary_list(
		JSContext* context,
		JSValue array_buffer
	) -> List<uint8_t>
	{
		auto size = std::size_t{};
		auto data = JS_GetArrayBuffer(context, &size, array_buffer);
		assert_conditional(data != nullptr, "Cannot get ArrayBuffer from value", "to_binary_list");
		return make_list(data, size);
	}


	inline static auto toArrayBuffer(
		JSContext* context,
		const List<uint8_t> & v
	) -> JSValue
	{
		return JS_NewArrayBufferCopy(context, v.data(), v.size());
	}

	template <typename T>
	inline static auto get_property(
		JSContext* context, 
		JSValueConst obj, 
		JSAtom atom, 
		const char* type_name, 
		T(*converter)(JSContext*, const JSValue&)
	) -> T 
	{
		auto value = JS_GetProperty(context, obj, atom);
		if (JS_IsException(value)) {
			Detail::throw_exception(context, obj, type_name);
		}
		auto result = converter(context, value);
		JS_FreeValue(context, value);
		return result;
	}

	inline static auto get_property_string(JSContext* context, JSValueConst obj, JSAtom atom) -> std::string {
		return get_property(context, obj, atom, "string", get_string);
	}

	inline static auto get_property_int32(JSContext* context, JSValueConst obj, JSAtom atom) -> int32_t {
		return get_property(context, obj, atom, "int32", get_int32);
	}

	inline static auto get_property_float64(JSContext* context, JSValueConst obj, JSAtom atom) -> double {
		return get_property(context, obj, atom, "double", get_float64);
	}

	inline static auto get_property_float32(JSContext* context, JSValueConst obj, JSAtom atom) -> float {
		return get_property(context, obj, atom, "float", get_float32);
	}

	inline static auto get_property_bigint64(JSContext* context, JSValueConst obj, JSAtom atom) -> int64_t {
		return get_property(context, obj, atom, "bigint64", get_bigint64);
	}

	inline static auto get_property_uint32(JSContext* context, JSValueConst obj, JSAtom atom) -> uint32_t {
		return get_property(context, obj, atom, "uint32", get_uint32);
	}

	inline static auto get_property_uint64(JSContext* context, JSValueConst obj, JSAtom atom) -> uint64_t {
		return get_property(context, obj, atom, "uint64", get_uint64);
	}

	inline static auto get_property_bool(JSContext* context, JSValueConst obj, JSAtom atom) -> bool {
		return get_property(context, obj, atom, "bool", get_bool);
	}
}