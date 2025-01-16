#pragma once

#include "kernel/utility/javascript/value_adapter.hpp"


namespace Sen::Kernel::JavaScript {

	template <typename ReturnType, typename... Args>
	struct Proxy;

	using Value = JavaScript::Value;

	template <typename... Args>
	auto parse_arguments(JSContext* ctx, JSValueConst* argv) -> std::tuple<std::decay_t<Args>...> {
		return _parse_arguments<Args...>(ctx, argv, std::index_sequence_for<Args...>{});
	}

	template <typename... Args, size_t... Indices>
	auto _parse_arguments(JSContext* ctx, JSValueConst* argv, std::index_sequence<Indices...>) -> std::tuple<std::decay_t<Args>...> {
		return std::make_tuple(from_value<std::decay_t<Args>>(ctx, argv[Indices])...);
	}

	template <typename ReturnType, typename... Args>
	struct Proxy {

		using NativeFunction = ReturnType (*)(Args...);

		template <NativeFunction function>
		static auto as_function(
			JSContext* context, 
			JSValue value,
			int argc, 
			JSValue* argv
		) -> JSValue
		{
			try {
				auto constexpr expected_args = sizeof...(Args);
				assert_conditional(argc == expected_args, fmt::format("{} {}, {}: {}", Kernel::Language::get("kernel.argument_expected"), expected_args, Kernel::Language::get("kernel.argument_received"), argc), "assert_conditional");
				auto arguments = parse_arguments<Args...>(context, argv);
				if constexpr (std::is_void_v<ReturnType>) {
					std::apply(function, arguments);
					return JS_UNDEFINED;
				} else {
					auto result = std::apply(function, arguments);
					return to_value<ReturnType>(context, result);
				}
			} catch (...) {
				auto exception = parse_exception();
				return throw_exception(context, exception.message(), exception.source, exception.function_name);
			}
		}
	};

	
}