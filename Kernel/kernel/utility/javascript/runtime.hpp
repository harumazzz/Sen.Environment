#pragma once

#include "kernel/utility/javascript/value.hpp"
#include "kernel/utility/utility.hpp"

namespace Sen::Kernel::JavaScript
{

	namespace FileSystem = Sen::Kernel::FileSystem;

	template <typename Type>
	concept SpaceX = std::is_same_v<Type, int32_t> || std::is_same_v<Type, uint32_t>
	|| std::is_same_v<Type, int64_t> || std::is_same_v<Type, uint64_t> || std::is_same_v<Type, float> || std::is_same_v<Type, double> || std::is_same_v<Type, std::string> || std::is_same_v<Type, bool>;

	template <typename Type>
	concept PrimitiveJSValue = std::is_same_v<Type, bool> or std::is_same_v<Type, std::string_view> or
		std::is_integral<Type>::value or std::is_floating_point<Type>::value;
		
	struct Handler {
		
		protected:

			using JS = Handler;

			using Atom = JavaScript::Atom;

			using Mutex = std::mutex;

			using Boolean = bool;

		private:

			Runtime runtime;

			Context context;

			Mutex mutex;

			Boolean is_module;

		public:

			explicit Handler(

			) : mutex{}, is_module{false}, runtime{Runtime::as_new_instance()}, context{thiz.runtime}
			{

			}

			inline auto exception(
			) -> std::string
			{
				auto result = std::string{};
				auto exception = JS_GetException(thiz.context.value);
				auto size = std::size_t{};
				auto exception_stack = JS_ToCStringLen(thiz.context.value, &size, exception);
				result += std::string{exception_stack, size};
				if(JS_IsException(exception)){
					auto atom = Atom{thiz.context.value, "stack"};
					auto js_stack = JS_GetProperty(thiz.context.value, exception, atom.value);
					if (not_undefined(js_stack)) {
						auto res_size = std::size_t{};
						auto js_exception = JS_ToCStringLen(thiz.context.value, &res_size, js_stack);
						result += std::string{js_exception};
						JS_FreeCString(context.value, js_exception);
					}
					JS_FreeValue(context.value, js_stack);
				}
				JS_FreeCString(context.value, exception_stack);
				JS_FreeValue(context.value, exception);
				return result;
			}

			inline auto register_object(
				std::function<void (JSRuntime*, JSContext*)> register_method
			) -> void
			{
				register_method(thiz.runtime.value, thiz.context.value);
				return;
			}

			inline auto unregister_object(
				std::function<void (JSContext*)> callback
			) -> void
			{
				callback(thiz.context.value);
				return;
			}

			inline auto register_object(
				std::function<void (JSContext*)> register_method
			) -> void
			{
				register_method(thiz.context.value);
				return;
			}

			inline auto evaluate_fs(
				std::string_view source
			) -> JSValue
			{
				return thiz.evaluate(FileSystem::read_quick_file(source), source);
			}

			inline static auto constexpr not_undefined(
				const JSValue & that
			) -> bool
			{
				return JS_VALUE_GET_TAG(that) != JS_TAG_UNDEFINED;
			}

			inline static auto constexpr not_null(
				const JSValue & that
			) -> bool
			{
				return JS_VALUE_GET_TAG(that) != JS_TAG_NULL;
			}

			inline auto has_promise(
			) -> bool
			{
				return static_cast<bool>(JS_IsJobPending(thiz.runtime.value));
			}

			inline static auto custom_module_loader(
				JSContext* context,
				char const* module_name,
				void* opaque
			) -> JSModuleDef*
			{
				auto source = std::string{module_name, std::strlen(module_name)};
				auto file = FileSystem::read_file(source);
				auto module_val = JS_Eval(context, file.data(), file.size(), module_name, JS_EVAL_TYPE_MODULE | JS_EVAL_FLAG_COMPILE_ONLY | JS_EVAL_FLAG_STRICT);
				if (JS_IsException(module_val)) {
					JS_FreeValue(context, module_val);
					JS_ThrowInternalError(context, "Cannot read module: %s", module_name);
					return nullptr;
				}
				auto module_def = static_cast<JSModuleDef *>(JS_VALUE_GET_PTR(module_val));
				JS_FreeValue(context, module_val);
				return module_def;
			}

			inline auto enable_module_load (
			) -> void
			{
				thiz.is_module = true;
				JS_SetModuleLoaderFunc(thiz.runtime.value, nullptr, &custom_module_loader, nullptr);
				return;
			}

			inline auto disable_module_load (
			) -> void
			{
				thiz.is_module = false;
				JS_SetModuleLoaderFunc(thiz.runtime.value, nullptr, nullptr, nullptr);
				return;
			}

			inline auto execute_pending_job (
			) -> void
			{
				auto job_context = static_cast<JSContext *>(nullptr);
				auto count = JS_ExecutePendingJob(thiz.runtime.value, &job_context);
				if (count < 0) {
					throw Exception{thiz.exception()};
				}
				return;
			}

			inline auto constexpr evaluate_flag (

			) -> int
			{
				return thiz.is_module ? JS_EVAL_TYPE_MODULE : JS_EVAL_TYPE_GLOBAL;
			}

			inline auto get_or_create_object(
				JSValue parent, 
				std::string_view name
			) -> JSValue 
			{
				auto atom = Atom{context.value, name};
				auto property = JS_GetProperty(context.value, parent, atom.value);
				if (JS_IsUndefined(property)) {
					property = JS_NewObject(context.value);
				}
				return property;
			}

			inline auto define_property (
				JSValue parent, 
				std::string_view name, 
				JSValue value
			) -> void
			{
				auto atom = Atom{context.value, name};
				JS_DefinePropertyValue(context.value, parent, atom.value, value, int{JS_PROP_C_W_E});
				return;
			}

			inline auto evaluate (
				std::string_view source_data,
				std::string_view source_file
			) -> JSValue
			{
				thiz.mutex.lock();
				auto eval_result = JS_Eval(thiz.context.value, source_data.data(), source_data.size(), source_file.data(), JS_EVAL_FLAG_STRICT | thiz.evaluate_flag());
				if(JS_IsException(eval_result)){
					throw Exception(thiz.exception(), std::source_location::current(), "evaluate");
				}
				thiz.mutex.unlock();
				return eval_result;
			}

			inline auto add_proxy(
				JSValue (*func)(JSContext*, JSValueConst, int, JSValueConst*),
                std::string_view function_name, 
				JSValueConst target_object
			) -> void
			{
				auto func_atom = Atom{context.value, function_name};
				JS_DefinePropertyValue(
					context.value,
					target_object,
					func_atom.value,
					JS_NewCFunction2(context.value, func, function_name.data(), 0, JS_CFUNC_generic, 0),
					JS_PROP_C_W_E
				);
			}

			inline auto _add_proxy(
				std::function<void(JSValue &global_object)> callback
			) -> void 
			{
				auto global_obj = JS_GetGlobalObject(context.value);
				callback(global_obj);
				JS_FreeValue(context.value, global_obj);
			}

			inline auto add_proxy(
				JSValue (*func)(JSContext *, JSValueConst, int, JSValueConst *),
				std::string_view function_name
			) -> void
			{
				_add_proxy([&](auto &global_obj){
					add_proxy(func, function_name, global_obj);
				});
			}


			inline auto add_proxy(
				JSValue (*func)(JSContext *, JSValueConst, int, JSValueConst *),
				std::string_view object_name,
				std::string_view function_name
			) -> void 
			{
				_add_proxy([&](auto &global_obj){
					auto obj = get_or_create_object(global_obj, object_name);
					add_proxy(func, function_name, obj);
					define_property(global_obj, object_name, obj);
				});
			}

			inline auto add_proxy(
				JSValue (*func)(JSContext *, JSValueConst, int, JSValueConst *),
				std::string_view obj1_name,
				std::string_view obj2_name,
				std::string_view function_name
			) -> void 
			{
				_add_proxy([&](auto &global_obj){
					auto outer_object = get_or_create_object(global_obj, obj1_name);
					auto inner_object = get_or_create_object(outer_object, obj2_name);
					add_proxy(func, function_name, inner_object);
					define_property(outer_object, obj2_name, inner_object);
					define_property(global_obj, obj1_name, outer_object);
				});
			}

			inline auto add_proxy(
				JSValue (*func)(JSContext *, JSValueConst, int, JSValueConst *),
				std::string_view obj1_name,
				std::string_view obj2_name,
				std::string_view obj3_name,
				std::string_view function_name
			) -> void 
			{
				_add_proxy([&](auto &global_obj){
					auto outer_object = get_or_create_object(global_obj, obj1_name);
					auto middle_object = get_or_create_object(outer_object, obj2_name);
					auto inner_object = get_or_create_object(middle_object, obj3_name);
					add_proxy(func, function_name, inner_object);
					define_property(middle_object, obj3_name, inner_object);
					define_property(outer_object, obj2_name, middle_object);
					define_property(global_obj, obj1_name, outer_object);
				});
			}

			inline auto add_proxy(
				JSValue (*func)(JSContext *, JSValueConst, int, JSValueConst *),
				std::string_view obj1_name,
				std::string_view obj2_name,
				std::string_view obj3_name,
				std::string_view obj4_name,
				std::string_view function_name
			) -> void 
			{
				_add_proxy([&](auto &global_obj){
					auto outer_object = get_or_create_object(global_obj, obj1_name);
					auto middle1_object = get_or_create_object(outer_object, obj2_name);
					auto middle2_object = get_or_create_object(middle1_object, obj3_name);
					auto inner_object = get_or_create_object(middle2_object, obj4_name);
					add_proxy(func, function_name, inner_object);
					define_property(middle2_object, obj4_name, inner_object);
					define_property(middle1_object, obj3_name, middle2_object);
					define_property(outer_object, obj2_name, middle1_object);
					define_property(global_obj, obj1_name, outer_object);
				});
			}

			inline auto add_proxy(
				JSValue (*func)(JSContext *, JSValueConst, int, JSValueConst *),
				std::string_view obj1_name,
				std::string_view obj2_name,
				std::string_view obj3_name,
				std::string_view obj4_name,
				std::string_view obj5_name,
				std::string_view function_name
			) -> void 
			{
				_add_proxy([&](auto &global_obj){
					auto outer_object = get_or_create_object(global_obj, obj1_name);
					auto middle_1_object = get_or_create_object(outer_object, obj2_name);
					auto middle_2_object = get_or_create_object(middle_1_object, obj3_name);
					auto middle_3_object = get_or_create_object(middle_2_object, obj4_name);
					auto inner_object = get_or_create_object(middle_3_object, obj5_name);
					add_proxy(func, function_name, inner_object);
					define_property(middle_3_object, obj5_name, inner_object);
					define_property(middle_2_object, obj4_name, middle_3_object);
					define_property(middle_1_object, obj3_name, middle_2_object);
					define_property(outer_object, obj2_name, middle_1_object);
					define_property(global_obj, obj1_name, outer_object);
				});
			}

			inline auto add_proxy(
				JSValue (*func)(JSContext *, JSValueConst, int, JSValueConst *),
				std::string_view obj1_name,
				std::string_view obj2_name,
				std::string_view obj3_name,
				std::string_view obj4_name,
				std::string_view obj5_name,
				std::string_view obj6_name,
				std::string_view function_name
			) -> void 
			{
				_add_proxy([&](auto &global_obj){
					auto outer_object = get_or_create_object(global_obj, obj1_name);
					auto middle1_object = get_or_create_object(outer_object, obj2_name);
					auto middle2_object = get_or_create_object(middle1_object, obj3_name);
					auto middle3_object = get_or_create_object(middle2_object, obj4_name);
					auto middle4_object = get_or_create_object(middle3_object, obj5_name);
					auto inner_object = get_or_create_object(middle4_object, obj6_name);
					add_proxy(func, function_name, inner_object);
					define_property(middle4_object, obj6_name, inner_object);
					define_property(middle3_object, obj5_name, middle4_object);
					define_property(middle2_object, obj4_name, middle3_object);
					define_property(middle1_object, obj3_name, middle2_object);
					define_property(outer_object, obj2_name, middle1_object);
					define_property(global_obj, obj1_name, outer_object);
				});
			}

			inline auto dump_memory_usage (

			) -> void
			{
				auto mem_usage = JSMemoryUsage{};
    			JS_ComputeMemoryUsage(thiz.runtime.value, &mem_usage);
				JS_DumpMemoryUsage(stdout, &mem_usage, thiz.runtime.value);
			}

			~Handler(

			) = default;
	};
}