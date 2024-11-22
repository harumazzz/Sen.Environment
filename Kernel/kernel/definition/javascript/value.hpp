#pragma once

#include "kernel/dependencies/quickjs.hpp"
#include "kernel/definition/utility.hpp"

namespace Sen::Kernel::Definition::JavaScript {

	template<typename T>
	using Pointer = T*;

	using CString = const char *;

	struct Runtime;

	struct Context;

	struct Context;

	struct Runtime {

		Pointer<JSRuntime> value;

		Runtime(

		) = delete;

		Runtime(
			Runtime& const value
		) = delete;

		Runtime(
			Runtime&& other
		) = delete;

		auto operator=(
			Runtime &&other
		) = delete;

		explicit Runtime (
			Pointer<JSRuntime> value
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
	};

	struct Context {

		Pointer<JSContext> value;

		Context(

		) = delete;

		Context(
			Context &other
		) = delete;

		Context(
			Context const & other
		) = delete;

		Context(
			Context &&other
		) = delete;

		explicit Context(
			Runtime& const runtime
		) : value{JS_NewContext(runtime.value)}
		{

		}

		~Context(

		) 
		{
			if (thiz.value != nullptr) {
				JS_FreeContext(this->value);
				this->value = nullptr;
			}
		}
	};

	struct Atom {

		explicit Atom(
			Pointer<JSContext> ctx, 
			CString name, 
			size_t len
		): context(ctx), value(JS_NewAtomLen(ctx, name, len)) {}

		explicit Atom(
			Pointer<JSContext> ctx, 
			std::string_view name
		): context(ctx), value(JS_NewAtomLen(ctx, name.data(), name.size())) {}

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
}