#pragma once

#include "kernel/utility/javascript/common.hpp"
#include "kernel/utility/javascript/value.hpp"
#include "kernel/utility/javascript/runtime.hpp"
#include "kernel/utility/javascript/converter.hpp"


namespace Sen::Kernel::JavaScript {

	template <typename F>
	struct Proxy;

	using Value = JavaScript::Value;

	template <typename T>
	concept HasValueMember = requires(T t) {
		{ t.value } -> std::convertible_to<JSValue>;
	};

	template <
		typename ReturnType,
		typename ContextType,
		typename ValueType,
		typename ListType
	> requires HasValueMember<ReturnType>
	struct Proxy<ReturnType(ContextType, ValueType, ListType)> {

		static_assert(sizeof(decltype(std::declval<ReturnType>().value)) == sizeof(JSValue));

		using Function = JSValue (*)(JSContext*, JSValue, int, JSValue*);

		using Value = JavaScript::Value;

		using Context = JavaScript::Context;

		template <ReturnType (*function)(ContextType, ValueType, ListType)> requires HasValueMember<ReturnType>
		static auto call(
			JSContext* context,
			JSValue value,
			int argc,
			JSValue* argv
		) -> JSValue
		{
			auto ctx = Context{context};
			auto val = Value{context, value};
			auto arguments = std::vector<Value>{};  
			arguments.reserve(argc);
			std::transform(argv, argv + argc, std::back_inserter(arguments),
				[&](const JSValue& v) { return Value{context, v}; }
			);
			auto result = function(ctx, val, arguments);
			return result.value; 
		}
	};

	
}