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

		private:

			Runtime& m_runtime;

			Context& m_context;

		public:

			using Function = JSValue (*)(JSContext*, JSValue, int, JSValue*);

			explicit Engine(
				Runtime& runtime,
				Context& context
			) :  m_runtime{ runtime }, m_context{ context }
			{
			}

			inline auto context(
			) -> Context&
			{
				return thiz.m_context;
			}

			inline auto runtime(
			) -> Runtime&
			{
				return thiz.m_runtime;
			}

			inline auto enable_module_load (
			) -> void
			{
				thiz.m_context.is_module = true;
				JS_SetModuleLoaderFunc(thiz.m_runtime.value, nullptr, &Runtime::custom_module_loader, nullptr);
			}

			inline auto disable_module_load (
			) -> void
			{
				thiz.m_context.is_module = false;
				JS_SetModuleLoaderFunc(thiz.m_runtime.value, nullptr, nullptr, nullptr);
			}

			template <typename Value, typename Error> requires (std::is_class<Value>::value && !std::is_pointer<Value>::value && std::is_class<Error>::value && !std::is_pointer<Error>::value)
			inline auto evaluate_fs(
				std::string& source
			) -> JSValue
			{
				return thiz.m_context.evaluate_fs<Value, Error>(source);
			}

			~Engine(

			) = default;
	};
}