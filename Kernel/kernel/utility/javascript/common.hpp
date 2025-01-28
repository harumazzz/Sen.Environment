#pragma once

#include "kernel/dependencies/quickjs.hpp"
#include "kernel/utility/utility.hpp"

namespace Sen::Kernel::JavaScript {

	template<typename T>
	using Pointer = T*;

	using CString = const char *;

	struct Runtime;

	struct Context;

	struct Atom;

	struct ClassID;

	struct Runtime {

		Pointer<JSRuntime> value;

		Runtime(

		) = delete;

		Runtime(
			Runtime& value
		) = delete;

		Runtime(
			Runtime&& other
		) = delete;

		auto operator=(
			Runtime &&other
		) = delete;

		explicit Runtime (
			const Pointer<JSRuntime>& value
		) : value{value}
		{

		}

		~Runtime(

		) 
		{
			if (thiz.value != nullptr) {
				JS_FreeRuntime(this->value);
				this->value = nullptr;
			}
		}

		static auto as_new_instance (

		) -> Runtime
		{
			return Runtime{JS_NewRuntime()};
		}

		inline auto has_promise(

		) -> bool {
			return static_cast<bool>(JS_IsJobPending(thiz.value));
		}

		inline auto dump_memory_usage(

		) -> void {
			auto mem_usage = JSMemoryUsage{};
			JS_ComputeMemoryUsage(thiz.value, &mem_usage);
			JS_DumpMemoryUsage(stdout, &mem_usage, thiz.value);
		}

		inline auto execute_pending_job(

		) -> void {
			auto job_context = static_cast<JSContext*>(nullptr);
			auto count = JS_ExecutePendingJob(thiz.value, &job_context);
			assert_conditional(count != 0, "Unhandled promise: Promise still not finished", "execute_pending_job");
		}

		inline static auto custom_module_loader(
			JSContext* context,
			char const* module_name,
			void* opaque
		) -> JSModuleDef* {
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
			auto definition = static_cast<JSModuleDef*>(JS_VALUE_GET_PTR(value));
			JS_FreeValue(context, value);
			auto meta = JS_GetImportMeta(context, definition);
			if (JS_IsException(meta)) {
				JS_ThrowReferenceError(context, "could not load module '%s': could not get import meta", module_name);
				return nullptr;
			}
			auto atom = JS_NewAtomLen(context, "name", std::strlen("name"));
			JS_DefinePropertyValue(context, meta, atom, JS_NewStringLen(context, module_name, std::strlen(module_name)), JS_PROP_C_W_E);
			JS_FreeValue(context, meta);
			JS_FreeAtom(context, atom);
			return definition;
		}
	};

	struct Context {

	protected:

		using Boolean = bool;

		inline static auto constexpr MEMORY_SIZE = 256_size;

	public:

		Pointer<JSContext> value;

		Boolean is_module;

		List<JSValue> m_resources;

		Context(

		) = delete;

		Context(
			Context &other
		) = delete;

		Context(
			Pointer<JSContext>& value
		) : value{value}, is_module{false}, m_resources{}
		{
			m_resources.reserve(MEMORY_SIZE);
		}

		Context(
			Context const & other
		) = delete;

		Context(
			Context &&other
		) = delete;

		explicit Context(
			Runtime& runtime
		) : value{JS_NewContext(runtime.value)}, is_module{false}, m_resources{}
		{
			m_resources.reserve(MEMORY_SIZE);
		}

		~Context(

		) 
		{
			thiz.release_value();
			if (thiz.value != nullptr) {
				JS_FreeContext(this->value);
				this->value = nullptr;
			}
		}

		inline auto constexpr evaluate_flag (

		) -> int
		{
			return thiz.is_module ? JS_EVAL_TYPE_MODULE : JS_EVAL_TYPE_GLOBAL;
		}

		template <typename Value, typename Error> requires (std::is_class<Value>::value && !std::is_pointer<Value>::value && std::is_class<Error>::value && !std::is_pointer<Error>::value)
		inline auto evaluate_fs (
			std::string_view source
		) -> JSValue
		{
			return thiz.evaluate<Value, Error>(FileSystem::read_quick_file(source), source);
		}

		template <typename Value, typename Error> requires (std::is_class<Value>::value && !std::is_pointer<Value>::value && std::is_class<Error>::value && !std::is_pointer<Error>::value)
		inline auto evaluate (
			std::string_view source_data,
			std::string_view source_file
		) -> JSValue
		{
			auto result = JS_Eval(thiz.value, source_data.data(), source_data.size(), source_file.data(), JS_EVAL_FLAG_STRICT | thiz.evaluate_flag());
			if(JS_IsException(result)){
				auto exception = Value{thiz.value, JS_GetException(thiz.value)};
				auto error = exception.get<std::shared_ptr<Error>>();
				throw Exception{ error->make_exception(), std::source_location::current(), "evaluate" };
			}
			thiz.m_resources.push_back(result);
			return result;
		}

		inline auto release_value(

		) -> void
		{
			for (auto& e : thiz.m_resources) {
				JS_FreeValue(thiz.value, e);
			}
			thiz.m_resources.clear();
			return;
		}

		inline auto set_context_opaque(
			void* opaque
		) -> void {
			JS_SetContextOpaque(thiz.value, opaque);
		}

		inline auto collect_garbage(
			JSValue& value
		) -> void
		{
			thiz.m_resources.push_back(value);
		}


	};

	struct Atom {

		explicit Atom(
			Pointer<JSContext> &context, 
			const std::string_view& name
		): context(context), value(JS_NewAtomLen(context, name.data(), name.size())) {}

		explicit Atom(
			Pointer<JSContext>& context,
			const uint32_t& value
		) : context(context), value(JS_NewAtomUInt32(context, value)) {
		}

		~Atom(

		)
		{ 
			JS_FreeAtom(context, value); 
		}

		Atom(const Atom&) = delete;

		Atom& operator=(const Atom&) = delete;

		Atom(Atom &&other) = delete;

		Atom &operator=(Atom &&other) = delete;

		JSAtom value;

		protected:
			
			Pointer<JSContext> context;
	};

	struct ClassID {

		JSClassID value{};

		explicit constexpr ClassID(

		) = default;

		~ClassID(

		) = default;

		ClassID(
			ClassID &other
		) = delete;

		ClassID(
			ClassID &&other
		) = delete;

		auto operator =(
			ClassID&& other
		) = delete;

		inline auto allocate_new (
			JSRuntime* runtime
		) -> void
		{
			JS_NewClassID(runtime, &thiz.value);
			return;
		}

		inline auto allocate_new (
			JSContext* context
		) -> void
		{
			JS_NewClassID(JS_GetRuntime(context), &thiz.value);
			return;
		}

		inline static auto constexpr temporary (

		) -> ClassID
		{
			return ClassID{};
		}

	};


}