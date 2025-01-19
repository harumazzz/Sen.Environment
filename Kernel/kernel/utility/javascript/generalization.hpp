#pragma once

#include "kernel/utility/javascript/common.hpp"
#include "kernel/utility/javascript/value.hpp"
#include "kernel/utility/javascript/runtime.hpp"
#include "kernel/utility/javascript/converter.hpp"

namespace Sen::Kernel::JavaScript {

	struct VariableDuplicator {

		constexpr VariableDuplicator(

		) = default;

		constexpr ~VariableDuplicator(
		
		) = default;

		inline static auto clone_undefined(
		) -> JSValue
		{
			return JS_UNDEFINED;
		}

		inline static auto clone_null(
		) -> JSValue
		{
			return JS_NULL;
		}

		inline static auto clone_array(
			JSContext* context,
			JSValue value
		) -> JSValue
		{
			auto destination = JS_NewArray(context);
			auto atom = JS_NewAtomLen(context, "length", std::strlen("length"));
			auto length = Converter::get_property_uint32(context, value, atom);
			JS_FreeAtom(context, atom);
			for (auto index : Range{ length })
			{
				auto index_atom = JS_NewAtomUInt32(context, index);
				auto current_value = JS_GetProperty(context, value, index_atom);
				JS_SetProperty(context, destination, index_atom, deep_clone(context, current_value));
				JS_FreeValue(context, current_value);
				JS_FreeAtom(context, index_atom);
			}
			return destination;
		}

		inline static auto clone_object(
			JSContext* context, 
			JSValue value
		) -> JSValue
		{
			auto json = JS_NewObject(context);
			auto property_enum = std::add_pointer_t<JSPropertyEnum>{ nullptr };
			auto property_count = uint32_t{};
			JS_GetOwnPropertyNames(context, &property_enum, &property_count, value, JS_GPN_STRING_MASK);
			for (auto i : Range<uint32_t>{ property_count })
			{
				auto keystring = JS_AtomToString(context, property_enum[i].atom);
				auto size = std::size_t{};
				auto key = JS_ToCStringLen(context, &size, keystring);
				auto object_value = JS_GetProperty(context, value, property_enum[i].atom);
				JS_DefinePropertyValue(context, json, property_enum[i].atom, deep_clone(context, object_value), int{ JS_PROP_C_W_E });
				JS_FreeValue(context, keystring);
				JS_FreeValue(context, object_value);
				JS_FreeCString(context, key);
			}
			JS_FreePropertyEnum(context, property_enum, property_count);
			return json;
		}

		inline static auto clone_string(
			JSContext* context, 
			JSValue value
		) -> JSValue
		{
			auto size = std::size_t{};
			auto c_str = JS_ToCStringLen(context, &size, value);
			auto destination = JS_NewStringLen(context, c_str, size);
			JS_FreeCString(context, c_str);
			return destination;
		}

		inline static auto clone_bool(
			JSContext* context, 
			JSValue value
		) -> JSValue
		{
			return JS_NewBool(context, JS_ToBool(context, value));
		}

		inline static auto clone_big_int(
			JSContext* context, 
			JSValue value
		) -> JSValue
		{
			auto val = int64_t{};
			JS_ToBigInt64(context, &val, value);
			return JS_NewBigInt64(context, val);
		}

		inline static auto clone_float64(
			JSContext* context, 
			JSValue value
		) -> JSValue
		{
			auto result = double{};
			JS_ToFloat64(context, &result, value);
			return JS_NewFloat64(context, result);
		}

		inline static auto clone_integer(
			JSContext* context, 
			JSValue value
		) -> JSValue
		{
			return JS_NewFloat64(context, JS_VALUE_GET_INT(value));
		}

		inline static auto deep_clone(
			JSContext* context,
			JSValue value
		) -> JSValue
		{
			switch (JS_VALUE_GET_TAG(value))
			{
				case JS_TAG_UNDEFINED: 
					return clone_undefined();
				case JS_TAG_NULL: 
					return clone_null();
				case JS_TAG_OBJECT:
					if (JS_IsArray(context, value))
						return clone_array(context, value);
					if (JS_IsObject(value))
						return clone_object(context, value);
				case JS_TAG_STRING: 
					return clone_string(context, value);
				case JS_TAG_BOOL: 
					return clone_bool(context, value);
				case JS_TAG_BIG_INT: 
					return clone_big_int(context, value);
				case JS_TAG_FLOAT64: 
					return clone_float64(context, value);
				case JS_TAG_INT: 
					return clone_integer(context, value);
			}
		}

	};

}