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

			using Function = JSValue (*)(JSContext*, JSValue, int, JSValue*);

			explicit Handler(

			) : mutex{}, is_module{false}, runtime{Runtime::as_new_instance()}, context{thiz.runtime}
			{
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

			inline auto evaluate_fs (
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
				if (!Path::Script::is_file(source)) {
					JS_ThrowInternalError(context, "Cannot read module, import file is missing, path: %s", module_name);
					return nullptr;
				}
				auto file = FileSystem::read_file(source);
				auto value = JS_Eval(context, file.data(), file.size(), module_name, JS_EVAL_TYPE_MODULE | JS_EVAL_FLAG_COMPILE_ONLY | JS_EVAL_FLAG_STRICT);
				if (JS_IsException(value)) {
					JS_ThrowInternalError(context, "Cannot read module: %s", module_name);
					return nullptr;
				}
				auto definition = static_cast<JSModuleDef *>(JS_VALUE_GET_PTR(value));
				JS_FreeValue(context, value);
				auto meta = JS_GetImportMeta(context, definition);
				if (JS_IsException(meta)) {
					JS_ThrowReferenceError(context, "could not load module '%s': could not get import meta", module_name);
					return nullptr;
				}
				auto atom = Atom{context, "name"};
				JS_DefinePropertyValue(context, meta, atom.value, JS_NewStringLen(context, module_name, std::strlen(module_name)), JS_PROP_C_W_E);
				JS_FreeValue(context, meta);
				return definition;
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
				assert_conditional(count != 0, "Unhandled promise: Promise still not finished", "execute_pending_job");
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

			inline auto evaluate (
				std::string_view source_data,
				std::string_view source_file
			) -> JSValue
			{
				thiz.mutex.lock();
				auto eval_result = JS_Eval(thiz.context.value, source_data.data(), source_data.size(), source_file.data(), JS_EVAL_FLAG_STRICT | thiz.evaluate_flag());
				if(JS_IsException(eval_result)){
					auto exception = Value{thiz.context.value, JS_GetException(thiz.context.value)};
					auto error = exception.get<Error>();
					throw Exception(error.make_exception(), std::source_location::current(), "evaluate");
				}
				thiz.mutex.unlock();
				return eval_result;
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

			inline auto add_function (
				JSValue (*func)(JSContext*, JSValue, int, JSValue*),
                std::string_view function_name, 
				JSValue target_object
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
				return;
			}

			inline auto _add_proxy(
				std::function<void(Value &global_object)> callback
			) -> void 
			{
				auto global_obj = Value::as_new_instance(thiz.context.value, JS_GetGlobalObject(context.value));
				callback(global_obj);
			}

			inline auto add_proxy(
				JSValue (*func)(JSContext *, JSValue, int, JSValue *),
				std::string_view function_name
			) -> void
			{
				return _add_proxy([&](auto &global_obj){
					return add_function(func, function_name, global_obj.value);
				});
			}

			template <std::size_t Size>
			inline auto add_proxy (
				JSValue (*func)(JSContext *, JSValue, int, JSValue *),
				const std::array<std::string_view, Size>& object_names,
				std::string_view function_name
			) -> void 
			{
				return _add_proxy([&](auto &global_obj) {
					auto current_object = std::accumulate(
						object_names.begin(), object_names.end(),
						global_obj.value,
						[&](auto&& obj, const std::string_view& name) {
							auto next_object = get_or_create_object(obj, name);
							define_property(obj, name, next_object);
							return next_object; 
						});
					return add_function(func, function_name, current_object);
				});
			}

			inline auto dump_memory_usage (

			) -> void
			{
				auto mem_usage = JSMemoryUsage{};
    			JS_ComputeMemoryUsage(thiz.runtime.value, &mem_usage);
				JS_DumpMemoryUsage(stdout, &mem_usage, thiz.runtime.value);
			}

			inline auto set_context_opaque (
				void* opaque
			) -> void
			{
				JS_SetContextOpaque(context.value, opaque);
				return;
			}

			~Handler(

			) = default;
	};
}