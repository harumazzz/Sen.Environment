#pragma once

#include "kernel/utility/javascript/common.hpp"

namespace Sen::Kernel::JavaScript {

	struct Value {

		private:

			Pointer<JSContext> context;

		public:

			JSValue value;

		public:

			explicit Value(

			) : context(nullptr), value{JS_UNINITIALIZED}
			{

			}

			explicit Value(
				Pointer<JSContext> const& context
			) : context{context}, value{JS_UNINITIALIZED}
			{

			}

			explicit Value(
				Pointer<JSContext> const& context,
				JSValue const& value
			) : context{ context }, value{ value }
			{

			}

			~Value(

			) 
			{
				thiz.reset_value();
			}

			auto is_uninitialized(

			) -> bool
			{
				return JS_IsUninitialized(value);
			}

			auto is_undefined(

			) -> bool
			{
				return JS_IsUndefined(value);
			}

			auto is_null(

			) -> bool
			{
				return JS_IsNull(value);
			}

			auto is_bigint(

			) -> bool
			{
				return JS_IsBigInt(thiz.context, thiz.value);
			}

			auto is_boolean(

			) -> bool
			{
				return JS_IsBool(thiz.value);
			}

			auto is_number(

			) -> bool
			{
				return JS_IsNumber(thiz.value);
			}
		
			auto is_string(

			) -> bool
			{
				return JS_IsString(thiz.value);
			}

			auto is_object(

			) -> bool
			{
				return JS_IsObject(thiz.value);
			}

			auto is_exception(

			) -> bool
			{
				return JS_IsException(thiz.value);
			}

			auto is_array(

			) -> bool
			{
				return JS_IsArray(thiz.context, thiz.value);
			}

			auto rebind_value(
				JSValue const& new_value
			) -> void 
			{
				JS_FreeValue(thiz.context, thiz.value);
				thiz.value = new_value;
				return;
			}

			auto set_string(
				std::string const & str
			) -> void
			{
				return rebind_value(JS_NewStringLen(thiz.context, str.data(), str.size()));
			}

			auto set_undefined(

			) -> void
			{
				return rebind_value(JS_UNDEFINED);
			}

			auto set_null(

			) -> void
			{
				return rebind_value(JS_NULL);
			}

			auto set_boolean(
				bool value
			) -> void
			{
				return rebind_value(JS_NewBool(thiz.context, static_cast<int>(value)));
			}

			template <typename U> requires std::is_integral<U>::value and (!std::is_unsigned<U>::value)
			auto set_bigint(
				U const& value
			) -> void
			{
				return rebind_value(JS_NewBigInt64(thiz.context, static_cast<int64_t>(value)));
			}

			template <typename U> requires std::is_integral<U>::value and std::is_unsigned<U>::value
			auto set_bigint(
				U const& value
			) -> void
			{
				return rebind_value(JS_NewBigUint64(thiz.context, static_cast<uint64_t>(value)));
			}

			template <typename U> requires std::is_integral<U>::value or std::is_floating_point<U>::value
			auto set_number(
				U const& value
			) -> void
			{
				return rebind_value(JS_NewFloat64(thiz.context, static_cast<double>(value)));
			}

			auto set_object(

			) -> void
			{
				return rebind_value(JS_NewObject(thiz.context));
			}

			auto set_array(

			) -> void
			{
				return rebind_value(JS_NewArray(thiz.context));
			}

			auto reset_value(

			) -> void 
			{
				return rebind_value(JS_UNINITIALIZED);
			}

			auto release_value(

			) -> JSValue
			{
				auto result = thiz.value;
				thiz.reset_value();
				return result;
			}

			inline static auto as_new_instance(
				Pointer<JSContext> const& context,
				JSValue const& value
			) -> Value
			{
				return Value{context, value};
			}

			auto get_prototype(

			) -> Value 
			{
				return as_new_instance(thiz.context, JS_GetPrototype(thiz.context, thiz.value));
			}

			auto set_prototype(
				Value&& that
			) -> void
			{
				JS_SetPrototype(thiz.context, thiz.value, that.release_value());
				return;
			}

			auto define_property(
				std::string const& name,
				Value&& value
			) -> void
			{
				auto atom = Atom{thiz.context, name};
				JS_DefinePropertyValue(thiz.context, thiz.value, atom.value, value.release_value(), JS_PROP_C_W_E);
				return;
			}

			auto define_property(
				uint32_t index,
				Value&& value
			) -> void
			{
				auto atom = Atom{ thiz.context, index };
				JS_DefinePropertyValue(thiz.context, thiz.value, atom.value, value.release_value(), JS_PROP_C_W_E);
				return;
			}

			auto define_property(
				std::string const& name,
				Value&& getter,
				Value&& setter
			) -> void
			{
				auto atom = Atom{ thiz.context, name };
				JS_DefinePropertyGetSet(thiz.context, thiz.value, atom.value, getter.release_value(), setter.release_value(), JS_PROP_C_W_E);
				return;
			}

			auto define_property(
				uint32_t index,
				Value&& getter,
				Value&& setter
			) -> void
			{
				auto atom = Atom{ thiz.context, index };
				JS_DefinePropertyGetSet(thiz.context, thiz.value, atom.value, getter.release_value(), setter.release_value(), JS_PROP_C_W_E);
				return;
			}

			auto delete_property(
				std::string const& name
			) -> void
			{
				auto atom = Atom{ thiz.context, name };
				JS_DeleteProperty(thiz.context, thiz.value, atom.value, JS_PROP_THROW);
				return;
			}

			auto delete_property(
				uint32_t index
			) -> void
			{
				auto atom = Atom{ thiz.context, index };
				JS_DeleteProperty(thiz.context, thiz.value, atom.value, JS_PROP_THROW);
				return;
			}

			auto has_property(
				std::string const& name
			) -> bool
			{
				auto atom = Atom{ thiz.context, name };
				auto result = static_cast<bool>(JS_HasProperty(thiz.context, thiz.value, atom.value) == 1);
				return result;
			}

			auto has_property(
				uint32_t index
			) -> bool
			{
				auto atom = Atom{ thiz.context, index };
				auto result = static_cast<bool>(JS_HasProperty(thiz.context, thiz.value, atom.value) == 1);
				return result;
			}

			auto get_property(
				std::string const& name
			) -> Value
			{
				auto atom = Atom{ thiz.context, name };
				auto value = thiz.as_new_instance(thiz.context, JS_GetProperty(thiz.context, thiz.value, atom.value));
				return value;
			}

			auto get_property(
				uint32_t index
			) -> Value
			{
				auto atom = Atom{ thiz.context, index };
				auto value = thiz.as_new_instance(thiz.context, JS_GetProperty(thiz.context, thiz.value, atom.value));
				return value;
			}

			auto set_property(
				std::string const& name,
				Value&& value
			) -> void
			{
				auto atom = Atom{ thiz.context, name };
				JS_SetProperty(thiz.context, thiz.value, atom.value, value.release_value());
				return;
			}

			auto set_property(
				uint32_t index,
				Value&& value
			) -> void
			{
				auto atom = Atom{ thiz.context, index };
				JS_SetProperty(thiz.context, thiz.value, atom.value, value.release_value());
				return;
			}

			template <typename T>
			auto get() -> T;

			template <>
			auto get(

			) -> std::string
			{
				assert_conditional(thiz.is_string(), "Value is not string", "get");
				auto size = std::size_t{};
				auto str = JS_ToCStringLen(thiz.context, &size, thiz.value);
				auto result = std::string{str, size};
				JS_FreeCString(thiz.context, str);
				return result;
			}

			template <>
			auto get(

			) -> bool
			{
				assert_conditional(thiz.is_boolean(), "Value is not boolean", "get");
				auto val = JS_ToBool(thiz.context, thiz.value);
				auto result = val == 1;
				return result;
			}

			template <typename T> requires std::is_floating_point<T>::value and (!std::is_integral<T>::value)
			auto get_number(

			) -> T
			{
				assert_conditional(thiz.is_number(), "Value is not number", "get");
				auto result = double{};
				JS_ToFloat64(thiz.context, &result, thiz.value);
				return static_cast<T>(result);
			}

			// For typescript enum, because ts enum convert to js number

			template <typename T> requires (!std::is_floating_point<T>::value) and (std::is_integral<T>::value)
			auto get_number(

			) -> T
			{
				assert_conditional(thiz.is_number(), "Value is not number", "get");
				auto result = int64_t{};
				JS_ToInt64(thiz.context, &result, thiz.value);
				return static_cast<T>(result);
			}

			template <typename T> requires (!std::is_floating_point<T>::value) and std::is_integral<T>::value and (!std::is_unsigned<T>::value)
			auto get_bigint(

			) -> T
			{
				assert_conditional(thiz.is_bigint(), "Value is not bigint", "get");
				auto result = int64_t{};
				JS_ToBigInt64(thiz.context, &result, thiz.value);
				return static_cast<T>(result);
			}

			template <typename T> requires (!std::is_floating_point<T>::value) and std::is_integral<T>::value and (std::is_unsigned<T>::value)
			auto get_bigint(

			) -> T
			{
				assert_conditional(thiz.is_bigint(), "Value is not bigint", "get");
				auto result = uint64_t{};
				JS_ToBigUint64(thiz.context, &result, thiz.value);
				return static_cast<T>(result);
			}


			auto collect_object_properties(

			) -> tsl::ordered_map<std::string, Value>
			{
				auto property_enum = Pointer<JSPropertyEnum>{};
				auto property_count = std::uint32_t{};
				JS_GetOwnPropertyNames(thiz.context, &property_enum, &property_count, thiz.value, JS_GPN_STRING_MASK);
				auto map = tsl::ordered_map<std::string, Value>{ property_count };
				for (auto element = property_enum; element < property_enum + property_count; ++element) {
					auto name = JS_AtomToCString(thiz.context, element->atom);
					map.insert({ std::string{ name, std::strlen(name) }, as_new_instance(thiz.context, JS_GetProperty(thiz.context, thiz.value, element->atom)) });
					JS_FreeCString(thiz.context, name);
				}
				JS_FreePropertyEnum(thiz.context, property_enum, property_count);
				return map;
			}

			auto collect_array(

			) -> List<Value>
			{
				auto map = thiz.collect_object_properties();
				auto length = static_cast<std::size_t>(map["length"].get_number<double>());
				auto result = List<Value>{ length };
				for (auto index : Range{ length }) {
					result.push_back(std::move(map.at(std::to_string(index))));
				}
				return result;
			}
	};

}