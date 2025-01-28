#pragma once

#include "kernel/utility/javascript/common.hpp"

namespace Sen::Kernel::JavaScript {

	struct Value;

	struct Error;

	struct Value {

		private:

			Pointer<JSContext> context;

		public:

			JSValue value;

		public:

			explicit Value(

			) : context{nullptr}, value{JS_UNINITIALIZED}
			{

			}
			
			Value(
				const Value &other
			) : context{other.context}, value{other.context == nullptr ? other.value : JS_DupValue(other.context, other.value)} {

			}

			Value(Value &&other) noexcept 
				: context{std::move(other.context)}, value{thiz.context == nullptr ? other.value : other.release_value()} {
				other.value = JS_UNINITIALIZED;
			}

			auto operator=(
				const Value &other
			) -> Value &
			{
				if (this != &other) {
					thiz.reset_value();
					thiz.context = other.context;
					thiz.value = other.context == nullptr ? other.value : JS_DupValue(other.context, other.value); 
				}
				return thiz;
			}

			auto operator=(
				Value &&other
			) -> Value&
			{
				if (this != &other) { 
					thiz.reset_value();
					thiz.context = std::move(other.context);
					thiz.value = thiz.context == nullptr ? other.value : other.release_value();
				}
				return thiz;
			}

			explicit Value(
				const JSValue & value
			) : context{nullptr}, value{value}
			{

			}

			explicit Value(
				const Pointer<JSContext> & context,
				const JSValue & value
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
				return static_cast<bool>(JS_IsUninitialized(value));
			}

			auto _context (

			) -> Pointer<JSContext>
			{
				return thiz.context;
			}

			auto is_undefined(

			) -> bool
			{
				return static_cast<bool>(JS_IsUndefined(value));
			}

			auto is_array_buffer(

			) -> bool
			{
				return static_cast<bool>(JS_IsArrayBuffer(value));
			}

			auto is_null(

			) -> bool
			{
				return static_cast<bool>(JS_IsNull(value));
			}

			auto is_bigint(

			) -> bool
			{
				return static_cast<bool>(JS_IsBigInt(thiz.context, thiz.value));
			}

			auto is_exception(

			) -> bool
			{
				return static_cast<bool>(JS_IsException(thiz.value));
			}

			auto is_boolean(

			) -> bool
			{
				return static_cast<bool>(JS_IsBool(thiz.value));
			}

			auto is_number(

			) -> bool
			{
				return static_cast<bool>(JS_IsNumber(thiz.value));
			}
		
			auto is_string(

			) -> bool
			{
				return static_cast<bool>(JS_IsString(thiz.value));
			}

			auto is_object(

			) -> bool
			{
				return static_cast<bool>(JS_IsObject(thiz.value));
			}

			auto is_array(

			) -> bool
			{
				return static_cast<bool>(JS_IsArray(thiz.context, thiz.value));
			}

			auto rebind_value(
				JSValue const& new_value
			) -> void 
			{
				if (thiz.context != nullptr) {
					JS_FreeValue(thiz.context, thiz.value);
				}
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

			inline static auto as_new_reference(
				Pointer<JSContext> const& context,
				JSValue const& value
			) -> Value
			{
				return Value{context, JS_DupValue(context, value)};
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
				std::string_view name,
				Value&& value
			) -> void
			{
				auto atom = Atom{thiz.context, name};
				JS_DefinePropertyValue(thiz.context, thiz.value, atom.value, value.release_value(), JS_PROP_C_W_E);
				return;
			}

			auto define_property(
				std::string_view name,
				JSValue value
			) -> void
			{
				auto atom = Atom{thiz.context, name};
				JS_DefinePropertyValue(thiz.context, thiz.value, atom.value, value, JS_PROP_C_W_E);
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
				uint32_t index,
				JSValue value
			) -> void
			{
				auto atom = Atom{ thiz.context, index };
				JS_DefinePropertyValue(thiz.context, thiz.value, atom.value, value, JS_PROP_C_W_E);
				return;
			}

			auto as_reference (

			) -> Value
			{
				return Value{thiz._context(), JS_DupValue(thiz._context(), thiz.value)};
			}

			auto as_instance (

			) -> Value
			{
				return Value{thiz._context(), thiz.value};
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
				std::string_view name
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

			auto call (

			) -> JSValue
			{
				if (thiz.is_undefined()) {
					return JS_UNDEFINED;
				}
				return JS_Call(thiz.context, thiz.value, JS_UNDEFINED, 0, nullptr);
			}

			auto set_property(
				std::string_view name,
				Value&& value
			) -> void
			{
				auto atom = Atom{ thiz.context, name };
				JS_SetProperty(thiz.context, thiz.value, atom.value, value.release_value());
				return;
			}

			auto set_property(
				std::string_view name,
				JSValue value
			) -> void
			{
				auto atom = Atom{ thiz.context, name };
				JS_SetProperty(thiz.context, thiz.value, atom.value, value);
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

			auto set_property(
				uint32_t index,
				JSValue value
			) -> void
			{
				auto atom = Atom{ thiz.context, index };
				JS_SetProperty(thiz.context, thiz.value, atom.value, value);
				return;
			}

			template <typename T>
			auto get() -> T;

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

			template <typename T>
			auto get_array_bufffer(

			) -> std::shared_ptr<T>
			{
				assert_conditional(thiz.is_array_buffer(), "Value is not an ArrayBuffer", "get");
				auto size = std::size_t{};
				auto value = JS_GetArrayBuffer(thiz.context, &size, thiz.value);
				return std::make_shared<T>(value, size);
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
				auto property_enum = std::add_pointer_t<JSPropertyEnum>{nullptr};
				auto property_count = std::uint32_t{};
				JS_GetOwnPropertyNames(thiz.context, &property_enum, &property_count, thiz.value, JS_GPN_STRING_MASK);
				auto map = tsl::ordered_map<std::string, Value>{ property_count };
				for (auto element = property_enum; element < property_enum + property_count; ++element) {
					auto name = JS_AtomToString(thiz.context, element->atom);
					auto size = std::size_t{};
					auto key = JS_ToCStringLen(thiz.context, &size, name);
					map.insert({ std::string{ key, size }, as_new_instance(thiz.context, JS_GetProperty(thiz.context, thiz.value, element->atom)) });
					JS_FreeValue(thiz.context, name);
					JS_FreeCString(thiz.context, key);
				}
				JS_FreePropertyEnum(thiz.context, property_enum, property_count);
				return map;
			}

			auto collect_array(

			) -> List<Value>
			{
				auto map = thiz.collect_object_properties();
				auto length = static_cast<std::size_t>(map["length"].get_number<double>());
				auto result = List<Value>{  };
				result.reserve(length);
				for (auto index : Range{ length }) {
					result.push_back(std::move(map.at(std::to_string(index))));
				}
				return result;
			}

			auto release (

			) -> JSValue
			{
				auto result = thiz.value;
				thiz.value = JS_UNINITIALIZED;
				return result;
			}

			auto is_promise (

			) -> bool
			{
				return static_cast<bool>(JS_IsPromise(thiz.value));
			}
	};

	struct Error {

		std::string message;

		std::string stack;

		inline auto make_exception (
			
		) -> std::string {
			auto pretty_exception = "Error: " + message;
			if (!stack.empty()) {
				pretty_exception += "\nStack Trace:\n" + stack;
			}
			return pretty_exception;
		}

	};

	template <>
	auto Value::get<std::string>(

	) -> std::string
	{
		assert_conditional(is_string(), "Value is not string", "get");
		auto size = std::size_t{};
		auto str = JS_ToCStringLen(context, &size, value);
		auto result = std::string{str, size};
		JS_FreeCString(context, str);
		return result;
	}

	template <>
	auto Value::get<std::shared_ptr<Error>>(

	) -> std::shared_ptr<Error>
	{
		auto message_size = std::size_t{};
		auto message = JS_ToCStringLen(context, &message_size, value);
		auto stack_trace = thiz.get_property("stack");
		auto stack_size = std::size_t{};
		auto stack = JS_ToCStringLen(context, &stack_size, stack_trace.value);
		auto destination = std::make_shared<Error>(std::string{ message, message_size },std::string{ stack, stack_size });
		#if WINDOWS
		std::replace(destination->stack.data(), destination->stack.data() + destination->stack.size(), '\\', '/');
		#endif
		JS_FreeCString(context, message);
		JS_FreeCString(context, stack);
		return destination;
	}

	template <>
	auto Value::get<bool>(

	) -> bool
	{
		assert_conditional(is_boolean(), "Value is not boolean", "get");
		auto val = JS_ToBool(context, value);
		return static_cast<bool>(val);
	}

	inline auto constexpr add_reference = JS_DupValue;

}