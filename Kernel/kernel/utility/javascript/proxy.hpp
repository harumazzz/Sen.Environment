#pragma once

#include "kernel/utility/javascript/builder.hpp"


namespace Sen::Kernel::JavaScript {

	template <typename ReturnType, typename... Args>
	struct Proxy;

	template <typename T> requires IsClass<T>
	struct ClassBuilder;

	using Value = JavaScript::Value;

	template <typename... Args, size_t... Indices>
	auto _parse_arguments(JSContext* ctx, JSValueConst* argv, std::index_sequence<Indices...>) -> std::tuple<std::decay_t<Args>...> {
		return std::make_tuple(from_value<std::decay_t<Args>>(ctx, argv[Indices])...);
	}

	template <typename... Args>
	auto parse_arguments(JSContext* ctx, JSValueConst* argv) -> std::tuple<std::decay_t<Args>...> {
		return _parse_arguments<Args...>(ctx, argv, std::index_sequence_for<Args...>{});
	}

	template <typename ReturnType, typename... Args>
	struct Proxy {

		using NativeFunction = ReturnType (*)(Args...);

		template <NativeFunction function>
		static auto as_function(
			JSContext* context, 
			JSValue value,
			int argc, 
			JSValue* argv
		) -> JSValue
		{
			try {
				auto constexpr expected_args = sizeof...(Args);
				if constexpr (expected_args != 0) {
					assert_conditional(argc == expected_args, fmt::format("{} {}, {}: {}", Kernel::Language::get("kernel.argument_expected"), expected_args, Kernel::Language::get("kernel.argument_received"), argc), "assert_conditional");
				}
				auto arguments = parse_arguments<Args...>(context, argv);
				if constexpr (std::is_void_v<ReturnType>) {
					std::apply(function, arguments);
					return JS_UNDEFINED;
				} else {
					auto result = std::apply(function, arguments);
					return to_value<ReturnType>(context, result);
				}
			} catch (...) {
				auto exception = parse_exception();
				return throw_exception(context, exception.message(), exception.source, exception.function_name);
			}
		}
	};

	template <typename T, typename... Args>
	struct ClassConstructorProxy {

		using Constructor = Pointer<T>(*)(Args...);

		template <Constructor constructor>
		static auto as_constructor(
			JSContext* context,
			JSValue value,
			int argc,
			JSValue* argv
		) -> JSValue
		{
			try {
				auto constexpr expected_args = sizeof...(Args);
				if constexpr (expected_args > 0) {
					assert_conditional(argc == expected_args, fmt::format("{} {}, {}: {}", Kernel::Language::get("kernel.argument_expected"), expected_args, Kernel::Language::get("kernel.argument_received"), argc), "assert_conditional");
				}
				auto arguments = parse_arguments<Args...>(context, argv);
				auto class_pointer = std::apply(constructor, arguments);
				auto proto = get_prototype_from_object(context, value);
				auto object = make_instance_of_class(context, proto, ClassBuilder<T>::class_id.value);
				JS_SetOpaque(object, class_pointer);
				return object;
			}
			catch (...) {
				auto exception = parse_exception();
				return throw_exception(context, exception.message(), exception.source, exception.function_name);
			}
		}
	};

	template <typename T, typename ReturnType, typename... Args> requires IsClass<T>
	struct ClassFunctionProxy {

		using NativeFunction = ReturnType (*)(Pointer<T>, Args...);

		template <NativeFunction function>
		static auto as_function(
			JSContext* context, 
			JSValue value,
			int argc, 
			JSValue* argv
		) -> JSValue
		{
			try {
				auto raw_pointer = get_opaque_value<T>(context, value, ClassBuilder<T>::class_id.value);
				auto constexpr expected_args = sizeof...(Args);
				if constexpr (expected_args != 0) {
					assert_conditional(argc == expected_args, fmt::format("{} {}, {}: {}", Kernel::Language::get("kernel.argument_expected"), expected_args, Kernel::Language::get("kernel.argument_received"), argc), "assert_conditional");
				}
				auto arguments = parse_arguments<Args...>(context, argv);
				if constexpr (std::is_void_v<ReturnType>) {
					std::apply([&](auto&&... params) { 
						function(raw_pointer, std::forward<decltype(params)>(params)...); 
					}, arguments);
					return JS_UNDEFINED;
				} else {
					auto result = std::apply([&](auto&&... params) { 
						return function(raw_pointer, std::forward<decltype(params)>(params)...); 
					}, arguments);
					return to_value<ReturnType>(context, result);
				}
			} catch (...) {
				auto exception = parse_exception();
				return throw_exception(context, exception.message(), exception.source, exception.function_name);
			}
		}
	};

	template <typename T, typename ReturnType>
	struct ClassGetterProxy {

		using NativeGetter = ReturnType(*)(Pointer<T>);

		template <NativeGetter getter>
		static auto as_getter(
			JSContext* context,
			JSValue value,
			int argc,
			JSValue* argv
		) -> JSValue
		{
			try {
				auto raw_pointer = get_opaque_value<T>(context, value, ClassBuilder<T>::class_id.value);
				auto result = getter(raw_pointer);
				return to_value<ReturnType>(context, result);
			}
			catch (...) {
				auto exception = parse_exception();
				return throw_exception(context, exception.message(), exception.source, exception.function_name);
			}
		}
	};

	template <typename T, typename U> requires IsClass<T>
	struct ClassSetterProxy {

		using NativeSetter = void(*)(Pointer<T>, U&);

		template <NativeSetter setter>
		static auto as_setter(
			JSContext* context,
			JSValue value,
			int argc,
			JSValue* argv
		) -> JSValue
		{
			try {
				auto raw_pointer = get_opaque_value<T>(context, value, ClassBuilder<T>::class_id.value);
				assert_conditional(argc == 1, fmt::format("{} {}, {}: {}", Kernel::Language::get("kernel.argument_expected"), 1, Kernel::Language::get("kernel.argument_received"), argc), "assert_conditional");
				auto argument = parse_arguments<U>(context, argv);
				std::apply([&](auto&&... args) {
					setter(raw_pointer, args...);
				}, argument);
				return JS_UNDEFINED;
			}
			catch (...) {
				auto exception = parse_exception();
				return throw_exception(context, exception.message(), exception.source, exception.function_name);
			}
		}
	};

	template <typename T> requires IsClass<T>
	struct ClassBuilder {

	protected:

		using String = std::string;

		std::optional<JSValue> constructor;

	public:

		Pointer<JSContext> context;

		inline static ClassID class_id{};

		Atom atom;

		JSValue class_proto;
		
		String class_name;

		explicit ClassBuilder(
			Pointer<JSContext> context,
			std::string_view name
		) : context{ context }, atom{context, name}, class_proto{JS_NewObject(thiz.context)}, class_name{name.data(), name.size()}, constructor{std::nullopt}
		{
			class_id.allocate_new(thiz.context);
			auto class_definition = make_class_definition<T>(name, class_id.value);
			assert_conditional(JS_NewClass(JS_GetRuntime(thiz.context), class_id.value, &class_definition) == 0, fmt::format("{} class register failed", name), "ClassBuilder");
		}

		template <auto Function, typename ReturnType, typename... Args>
		auto add_member_function(
			std::string_view function_name
		) -> ClassBuilder<T> &
		{
			auto atom = JS_NewAtomLen(context, function_name.data(), function_name.size());
			auto function = JS_NewCFunction2(context, ClassFunctionProxy<T, ReturnType, Args...>::template as_function<Function>, function_name.data(), 0, JS_CFUNC_generic, 0);
			JS_DefinePropertyValue(context, class_proto, atom, function, int{ JS_PROP_C_W_E });
			JS_FreeAtom(context, atom);
			return thiz;
		}

		template <typename Type, auto Getter, auto Setter>
		auto add_getter_setter(
			std::string_view name
		) -> ClassBuilder<T>&
		{
			auto atom = JS_NewAtomLen(context, name.data(), name.size());
			auto getter = JS_NewCFunction2(context, ClassGetterProxy<T, Type>::template as_getter<Getter>, name.data(), 0, JS_CFUNC_generic, 0);
			auto setter = JS_NewCFunction2(context, ClassSetterProxy<T, Type>::template as_setter<Setter>, name.data(), 0, JS_CFUNC_generic, 0);
			JS_DefinePropertyGetSet(context, class_proto, atom, getter, setter, int{ JS_PROP_C_W_E });
			JS_FreeAtom(context, atom);
			return thiz;
		}

		template <auto Constructor, typename... Args>
		auto add_constructor(
		) -> ClassBuilder<T>&
		{
			assert_conditional(!thiz.constructor.has_value(), fmt::format("Class {} must not have a constructor before", class_name), "add_constructor");
			thiz.constructor.emplace(JS_NewCFunction2(context, ClassConstructorProxy<T, Args...>::template as_constructor<Constructor>, class_name.data(), 0, JS_CFUNC_constructor, 0));
			JS_SetConstructor(context, thiz.constructor.value(), thiz.class_proto);
			JS_SetClassProto(context, class_id.value, thiz.class_proto);
			return thiz;
		}

		template <auto Function, typename ReturnType, typename... Args>
		auto add_static_function(
			std::string_view function_name
		) -> ClassBuilder<T> &
		{
			assert_conditional(thiz.constructor.has_value(), fmt::format("Class {} must have a constructor first before adding static methods", class_name), "add_static_function");
			auto atom = JS_NewAtomLen(context, function_name.data(), function_name.size());
			auto function = JS_NewCFunction2(context, Proxy<ReturnType, Args...>::template as_function<Function>, function_name.data(), 0, JS_CFUNC_generic, 0);
			JS_DefinePropertyValue(context, thiz.constructor.value(), atom, function, int{JS_PROP_C_W_E});
			JS_FreeAtom(context, atom);
			return thiz;
		}

		template <size_t InstanceCount>
		auto build(
			const std::array<std::string_view, InstanceCount>& instance_names
		) -> void
		{
			assert_conditional(thiz.constructor.has_value(), fmt::format("Class {} must have a constructor", class_name), "build");
			auto global_obj = JS_GetGlobalObject(context);
			auto parent_obj = JS_DupValue(context, global_obj);
			auto allocated = std::array<JSValue, InstanceCount>{};
			auto index = std::size_t{ 0 };
			for (auto& name : instance_names) {
				auto new_obj = make_instance_object(context, parent_obj, name);
				allocated[index++] = new_obj;
				parent_obj = new_obj;
			}
			auto atom = Atom{ context, class_name };
			JS_DefinePropertyValue(context, parent_obj, atom.value, thiz.constructor.value(), int{JS_PROP_C_W_E});
			for (auto it = allocated.rbegin(); it != allocated.rend(); ++it) {
				JS_FreeValue(context, *it);
			}
			JS_FreeValue(context, global_obj);
			return;
		}

		~ClassBuilder(

		)
		{
			JS_FreeValue(context, thiz.class_proto);
		}

	};

	
}