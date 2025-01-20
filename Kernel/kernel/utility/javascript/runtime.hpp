#pragma once

#include "kernel/utility/javascript/value.hpp"
#include "kernel/utility/trait/trait.hpp"
#include "kernel/utility/utility.hpp"

namespace Sen::Kernel::JavaScript
{

	namespace FileSystem = Sen::Kernel::FileSystem;
		
	class Engine {
		
		protected:

			using Atom = JavaScript::Atom;

			using Mutex = std::mutex;

			using Boolean = bool;

			inline static auto constexpr MEMORY_SIZE = 256_size;

		private:

			Runtime m_runtime;

			Context m_context;

			Boolean is_module;

			List<JSValue> resources;

		public:

			using Function = JSValue (*)(JSContext*, JSValue, int, JSValue*);

			explicit Engine(

			) : is_module{ false }, m_runtime{ Runtime::as_new_instance() }, m_context{ thiz.m_runtime }, resources{}
			{
				thiz.resources.reserve(MEMORY_SIZE);
			}

			inline auto evaluate_fs (
				std::string_view source
			) -> JSValue
			{
				return thiz.evaluate(FileSystem::read_quick_file(source), source);
			}

			inline auto has_promise(
			) -> bool
			{
				return static_cast<bool>(JS_IsJobPending(thiz.m_runtime.value));
			}

			inline auto context(
			) -> Context&
			{
				return thiz.m_context;
			}

			inline static auto custom_module_loader(
				JSContext* context,
				char const* module_name,
				void* opaque
			) -> JSModuleDef*
			{
				auto source = std::string{module_name, std::strlen(module_name)};
				if (!FileSystem::is_file(source)) {
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
				JS_SetModuleLoaderFunc(thiz.m_runtime.value, nullptr, &custom_module_loader, nullptr);
				return;
			}

			inline auto disable_module_load (
			) -> void
			{
				thiz.is_module = false;
				JS_SetModuleLoaderFunc(thiz.m_runtime.value, nullptr, nullptr, nullptr);
				return;
			}

			inline auto execute_pending_job (
			) -> void
			{
				auto job_context = static_cast<JSContext *>(nullptr);
				auto count = JS_ExecutePendingJob(thiz.m_runtime.value, &job_context);
				assert_conditional(count != 0, "Unhandled promise: Promise still not finished", "execute_pending_job");
			}

			inline auto constexpr evaluate_flag (

			) -> int
			{
				return thiz.is_module ? JS_EVAL_TYPE_MODULE : JS_EVAL_TYPE_GLOBAL;
			}

			inline auto evaluate (
				std::string_view source_data,
				std::string_view source_file
			) -> JSValue
			{
				auto result = JS_Eval(thiz.m_context.value, source_data.data(), source_data.size(), source_file.data(), JS_EVAL_FLAG_STRICT | thiz.evaluate_flag());
				if(JS_IsException(result)){
					auto exception = Value{thiz.m_context.value, JS_GetException(thiz.m_context.value)};
					auto error = exception.get<std::shared_ptr<Error>>();
					throw Exception{ error->make_exception(), std::source_location::current(), "evaluate" };
				}
				thiz.resources.push_back(result);
				return result;
			}

			inline auto define_property (
				JSValue parent, 
				std::string_view name, 
				JSValue value
			) -> void
			{
				auto atom = Atom{ m_context.value, name};
				JS_DefinePropertyValue(m_context.value, parent, atom.value, value, int{JS_PROP_C_W_E});
				return;
			}

			inline auto dump_memory_usage (

			) -> void
			{
				auto mem_usage = JSMemoryUsage{};
    			JS_ComputeMemoryUsage(thiz.m_runtime.value, &mem_usage);
				JS_DumpMemoryUsage(stdout, &mem_usage, thiz.m_runtime.value);
			}

			inline auto set_context_opaque (
				void* opaque
			) -> void
			{
				JS_SetContextOpaque(m_context.value, opaque);
				return;
			}

			inline auto release_value(

			) -> void
			{
				for (auto& e : thiz.resources) {
					JS_FreeValue(thiz.m_context.value, e);
				}
				thiz.resources.clear();
				return;
			}

			~Engine(

			)
			{
				thiz.release_value();
			}
	};
}