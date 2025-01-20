#pragma once

#include "kernel/utility/javascript/value_adapter.hpp"

namespace Sen::Kernel::JavaScript {

	inline static auto throw_exception(
		JSContext* context,
		std::string_view error,
		std::string_view source,
		std::string_view function_name
	) -> JSValue
	{
		auto func_name = function_name.empty() ? "internal" : function_name;
		auto command = fmt::format(
			R"((function {0}() {{
				let e = new Error(`{1}`); 
				e.source = `{2}`;
				throw e;
			}}()))",
			func_name, error, source
		);
		auto value = JS_Eval(context, command.data(), command.size(), source.data(), JS_EVAL_FLAG_STRICT | JS_EVAL_TYPE_GLOBAL);
		JS_FreeValue(context, value);
		return JS_EXCEPTION;
	}

	using JSGetter = JSValue(*)(JSContext* context, JSValue value, int magic);

	using JSSetter = JSValue(*)(JSContext* context, JSValue value, JSValue argv, int magic);

	using JSFunction = JSValue(*)(JSContext* context, JSValue value, int argc, JSValue* argv);

	template <typename T> requires (std::is_class<T>::value && !std::is_pointer<T>::value)
	inline static auto get_opaque_value(
		JSContext* context,
		JSValue value,
		JSClassID class_id
	) -> T*
	{
		auto obj = static_cast<T*>(JS_GetOpaque2(context, value, class_id));
		assert_conditional(obj != nullptr, fmt::format("Cannot get instance of class, class id: {}", class_id), "get_opaque_value");
		return obj;
	}

	template <typename T> requires std::is_same<T, uint32_t>::value || std::is_same<T, std::string_view>::value
	inline auto get_value_from_object (
		JSContext* context,
		JSValue& source,
		T name
	) -> JSValue
	{
		auto atom = Atom{context, name};
		auto value = JS_GetProperty(context, source, atom.value);
		assert_conditional(!(static_cast<bool>(JS_IsException(value))), fmt::format("Cannot get property \"{}\" in object", name), "get_value_from_object");
		return value;
	}

	inline auto get_prototype_from_object (
		JSContext* context,
		JSValue& source
	) -> JSValue
	{
		return get_value_from_object<std::string_view>(context, source, "prototype");
	}

	inline auto make_instance_of_class (
		JSContext *context, 
		JSValue &proto, 
		JSClassID &class_id
	) -> JSValue
	{
		auto destination = JS_NewObjectProtoClass(context, proto, class_id);
		assert_conditional(!(static_cast<bool>(JS_IsException(destination))), fmt::format("Cannot initialize an object, class id: \"{}\"", class_id), "get_prototype_from_object");
		return destination;
	}

	template <typename Callable> requires std::is_invocable<Callable>::value
	struct Builder {

	private:

		static auto make_return_function (
			JSContext* context,
			Callable&& function
		) -> JSValue
		{
			auto result = function();
			return to_value<std::invoke_result_t<Callable>>(context, result);
		}

	public:

		static auto make_function_declaration(
			JSContext* context,
			Callable&& function
		) -> JSValue
		{
			if constexpr (std::is_same<std::invoke_result_t<Callable>, void>::value) {
				function();
				return JS_UNDEFINED;
			}
			else {
				return make_return_function(context, std::forward<Callable>(function));
			}
		}
	};
}