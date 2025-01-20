#pragma once

#include "kernel/utility/javascript/common.hpp"
#include "kernel/utility/javascript/value.hpp"

namespace Sen::Kernel::JavaScript {

	class NamespaceBuilder {

		private:

			Pointer<JSContext> context;

			JSValue current;           

		public:

			explicit NamespaceBuilder(
				Pointer<JSContext> context,
				const JSValue &current
			) : context{context}, current{current} {
				
			}

			NamespaceBuilder(
				const NamespaceBuilder&
			) = delete;

			auto operator=(
				const NamespaceBuilder&
			) -> NamespaceBuilder& = delete;

			explicit NamespaceBuilder(
				Pointer<JSContext> context
			) : context{ context },  current{JS_GetGlobalObject(context)} {
				
			}

			~NamespaceBuilder(
				
			) {
				JS_FreeValue(thiz.context, current);
			}

			inline auto add_space(
				std::string_view name
			) -> NamespaceBuilder {
				auto atom = JS_NewAtomLen(thiz.context, name.data(), name.size());
				auto new_instance = JS_NewObject(context);
				JS_DefinePropertyValue(thiz.context, thiz.current, atom, new_instance, int{ JS_PROP_C_W_E });
				JS_FreeAtom(context, atom);
				return NamespaceBuilder{thiz.context, JS_DupValue(context, new_instance) };
			}

			inline auto add_proxy (
				std::string_view name,
				JSValue value
			) -> void
			{
				auto atom = JS_NewAtomLen(thiz.context, name.data(), name.size());
				thiz.add_proxy(atom, value);
				JS_FreeAtom(context, atom);
				return;
			}

			inline auto delete_proxy (
				std::string_view name
			) -> void
			{
				auto atom = JS_NewAtomLen(thiz.context, name.data(), name.size());
				JS_DeleteProperty(thiz.context, thiz.current, atom, JS_PROP_THROW);
				JS_FreeAtom(context, atom);
				return;
			}

			inline auto add_proxy (
				JSAtom atom,
				JSValue value
			) -> void
			{
				JS_DefinePropertyValue(thiz.context, thiz.current, atom, value, int{JS_PROP_C_W_E});
				return;
			}

			inline auto add_function (
				std::string_view name,
				JSCFunction value
			) -> NamespaceBuilder&
			{
				thiz.add_proxy(name, JS_NewCFunction2(thiz.context, value, name.data(), 0, JS_CFUNC_generic, 0));
				return thiz;
			}
	};
	
}