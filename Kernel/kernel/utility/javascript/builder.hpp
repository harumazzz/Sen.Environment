#pragma once

#include "kernel/utility/javascript/generalization.hpp"

namespace Sen::Kernel::JavaScript {

	inline static auto throw_exception(
		JSContext* context,
		std::string_view error,
		std::string_view source,
		std::string_view function_name
	) -> JSValue
	{
		auto evaluate_context = fmt::format(
			R"(function {0}() {{
				let e = new Error(`{1}`); 
				e.source = `{2}`;
				throw e;
			}}
			{0}();)",
			function_name, error, source
		);
		JS_Eval(context, evaluate_context.data(), evaluate_context.length(), source.data(), JS_EVAL_TYPE_GLOBAL);
		return JS_EXCEPTION;
	}

	inline static auto proxy_wrapper(
		JSContext* context,
		std::string_view func_name,
		std::function<JSValue()> code
	) -> JSValue
	{
		auto result = JS_UNDEFINED;
		try {
			result = code();
		}
		catch (...) {
			auto exception = parse_exception();
			if (exception.function_name.empty()) {
				exception.function_name = func_name;
			}
			result = throw_exception(context, exception.message(), exception.source, exception.function_name);
		}
		return result;
	}

	using JSGetter = JSValue(*)(JSContext* context, JSValue value, int magic);

	using JSSetter = JSValue(*)(JSContext* context, JSValue value, JSValue argv, int magic);

	using JSFunction = JSValue(*)(JSContext* context, JSValue value, int argc, JSValue* argv);

	template <typename T> requires (std::is_class<T>::value && !std::is_pointer<T>::value)
	inline auto make_finalizer(
		JSClassID class_id
	) -> std::function<void(JSRuntime*, JSValue)>
	{
		return [class_id](JSRuntime* rt, JSValue value) {
			auto object_class = static_cast<T*>(JS_GetOpaque(value, class_id));
			if (object_class != nullptr) {
				delete object_class;
			}
		};
	}

	template <typename T> requires (std::is_class<T>::value && !std::is_pointer<T>::value)
	inline static auto make_handle(
		JSContext* context,
		JSValue value,
		int argc,
		JSValue* argv,
		std::string_view method_name,
		std::function<JSValue(T*)> method,
		JSClassID class_id
	) -> JSValue
	{
		auto class_pointer = get_opaque_value<T>(context, value, class_id);
		return method(class_pointer);
	}

	template <typename T> requires (std::is_class<T>::value && !std::is_pointer<T>::value)
	inline auto constexpr make_class_definition(
		std::string_view class_name,
		JSClassID class_id
	) -> JSClassDef
	{
		return JSClassDef{
			.class_name = class_name.data(),
			.finalizer = make_finalizer<T>(class_id).template target<JSClassFinalizer>(),
			.gc_mark = nullptr,
			.call = nullptr,
			.exotic = nullptr,
		};
	}

	inline static auto make_instance_object(
		JSContext* context,
		JSValue parent,
		std::string_view name
	) -> JSValue
	{
		auto atom = Atom{ context, name };
		auto obj = JS_GetProperty(context, parent, atom.value);
		if (JS_IsUndefined(obj)) {
			obj = JS_NewObject(context);
			JS_DefinePropertyValue(context, parent, atom.value, obj, int{ JS_PROP_C_W_E });
		}
		return obj;
	}

	template <typename T> requires (std::is_class<T>::value && !std::is_pointer<T>::value)
	inline auto constexpr make_deleter(

	) 
	{
		return [](T* obj) {
			delete obj;
		};
	}

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

	template <typename T>
	inline static auto throw_constructor_error(
		JSContext* context,
		std::string_view error_message
	) -> JSValue
	{
		JS_ThrowInternalError(context, error_message.data());
		return JS_EXCEPTION;
	}

	template <typename T>
	inline static auto generate_constructor(
		JSContext* context,
		JSValue value,
		std::unique_ptr<T, decltype(make_deleter<T>())> class_ptr,
		ClassID& class_id
	) -> JSValue
	{
		auto atom = Atom{ context, "prototype" };
		auto proto = JS_GetProperty(context, value, atom.value);
		if (JS_IsException(proto)) {
			return throw_constructor_error<T>(context, "Failed to get prototype");
		}
		auto obj = JS_NewObjectProtoClass(context, proto, class_id.value);
		JS_FreeValue(context, proto);
		if (JS_IsException(obj)) {
			return throw_constructor_error<T>(context, "Failed to create JS object");
		}
		JS_SetOpaque(obj, class_ptr.release());
		return obj;
	}

	template <typename T> requires std::is_class<T>::value
	inline static auto initialize_constructor(
		JSContext* context,
		JSValue value,
		int argc,
		JSValue* argv,
		std::function<T* (int argc, JSValue* argv)> initializer,
		ClassID& class_id,
		std::string_view error_message
	) -> JSValue
	{
		if (initializer) {
			auto class_pointer = std::unique_ptr<T, decltype(make_deleter<T>())>(initializer(argc, argv), make_deleter<T>());
			if (class_pointer != nullptr) {
				return generate_constructor<T>(context, value, std::move(class_pointer), class_id);
			}
		}
		return throw_constructor_error<T>(context, error_message);
	}

	template <typename T, int magic> requires (std::is_class<T>::value && !std::is_pointer<T>::value)
	inline auto constexpr generate_getter_setter(
		std::string_view function_name,
		JSGetter getter,
		JSSetter setter
	) -> JSCFunctionListEntry
	{
		auto entry = JSCFunctionListEntry{};
		entry.name = function_name.data();
		entry.prop_flags = JS_PROP_CONFIGURABLE;
		entry.def_type = JS_DEF_CGETSET_MAGIC;
		entry.magic = static_cast<int16_t>(magic);
		entry.u.getset.get.getter_magic = reinterpret_cast<JSValue(*)(JSContext*, JSValue, int)>(getter);
		entry.u.getset.set.setter_magic = reinterpret_cast<JSValue(*)(JSContext*, JSValue, JSValue, int)>(setter);
		return entry;
	}

	template <typename T, int length> requires (std::is_class<T>::value && !std::is_pointer<T>::value)
	inline auto constexpr generate_class_function(
		std::string_view function_name,
		JSFunction function
	) -> JSCFunctionListEntry
	{
		auto entry = JSCFunctionListEntry{};
		entry.name = function_name.data();
		entry.prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE;
		entry.def_type = JS_DEF_CFUNC;
		entry.magic = 0;
		entry.u.func.length = static_cast<std::uint8_t>(length);
		entry.u.func.cproto = JS_CFUNC_generic;
		entry.u.func.cfunc.generic = reinterpret_cast<JSCFunction*>(function);
		return entry;
	}

	template <typename Class, typename Constructor, std::size_t InstanceCount, std::size_t ProtoFunctionCount> requires std::is_class<Class>::value && 
	std::is_function<Constructor>::value
	inline static auto build_class(
		JSContext* context,
		Class& class_id,
		const Constructor& constructor_func,
		std::string_view class_name,
		const std::array<JSCFunctionListEntry, ProtoFunctionCount>& proto_functions,
		const JSClassDef& class_definition,
		const std::array<std::string_view, InstanceCount>& instance_names
	) -> void
	{
		class_id.allocate_new(context);
		assert_conditional(JS_NewClass(JS_GetRuntime(context), class_id.value, &class_definition) == 0, fmt::format("{} class register failed", class_name), "register_class");
		auto ctor = JS_NewCFunction2(context, constructor_func, class_name.data(), 0, JS_CFUNC_constructor, 0);
		auto proto = JS_NewObject(context);
		JS_SetPropertyFunctionList(context, proto, proto_functions.data(), proto_functions.size());
		JS_SetConstructor(context, ctor, proto);
		auto global_obj = JS_GetGlobalObject(context);
		auto parent_obj = global_obj;
		auto allocated = std::array<JSValue, InstanceCount>{};
		auto index = std::size_t{ 0 };
		for (auto& name : instance_names) {
			auto new_obj = make_instance_object(context, parent_obj, name);
			allocated[index++] = new_obj;
			parent_obj = new_obj;
		}
		auto atom = Atom{ context, class_name };
		JS_DefinePropertyValue(context, parent_obj, atom.value, ctor, int{ JS_PROP_C_W_E });
		parent_obj = JS_UNDEFINED;
		JS_FreeValue(context, parent_obj);
		for (auto& value : allocated) {
			JS_FreeValue(context, value);
		}
		JS_FreeValue(context, global_obj);
		JS_FreeValue(context, proto);
		return;
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
		assert_conditional(static_cast<bool>(JS_IsException(value)), fmt::format("Cannot get property \"{}\" in object", name), "get_value_from_object");
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
		assert_conditional(static_cast<bool>(JS_IsException(destination)), fmt::format("Cannot initialize an object, class id: \"{}\"", class_id), "get_prototype_from_object");
		return destination;
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

}