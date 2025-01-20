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
	};

	struct Context {

		Pointer<JSContext> value;

		Context(

		) = delete;

		Context(
			Context &other
		) = delete;

		Context(
			Pointer<JSContext>& value
		) : value{value}
		{

		}

		Context(
			Context const & other
		) = delete;

		Context(
			Context &&other
		) = delete;

		explicit Context(
			Runtime& runtime
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