#pragma once

#include "kernel/utility/javascript/common.hpp"
#include "kernel/utility/javascript/value.hpp"
#include "kernel/utility/javascript/runtime.hpp"
#include "kernel/utility/javascript/converter.hpp"
#include <typeinfo>

namespace Sen::Kernel::JavaScript {

	template <typename T>
	struct Generalization {

		static auto make_native_declaration(
			JSContext* context,
			T& value
		) -> JSValue
		{
			static_assert(false, "<% this case is not implemented %>");
			JS_ThrowInternalError(context, fmt::format("{} is not implemented", typeid(T).name()).data());
			return JS_EXCEPTION;
		}

	};

	template <typename T> requires (std::is_integral_v<T> && !std::is_unsigned<T>::value)
	struct Generalization<T> {
		static auto make_native_declaration(
			JSContext* context,
			T& value
		) -> JSValue
		{
			return Converter::to_bigint(context, value);
		}
	};

	template <typename T> requires (std::is_integral_v<T> && std::is_unsigned<T>::value)
	struct Generalization<T> {
		static auto make_native_declaration(
			JSContext* context,
			T& value
		) -> JSValue
		{
			return Converter::to_bigint<uint64_t>(context, static_cast<uint64_t>(value));
		}
	};

	template <typename T> requires (!std::is_integral_v<T> && std::is_floating_point<T>::value)
	struct Generalization<T> {
		static auto make_native_declaration(
			JSContext* context,
			T& value
		) -> JSValue
		{
			return Converter::to_number(context, static_cast<double>(value));
		}
	};

	template <typename T>
	concept StringLike =
		std::is_same_v<T, std::string> ||
		std::is_same_v<T, std::string_view> ||
		std::is_same_v<T, const char*>;

	template <StringLike T>
	struct Generalization<T> {
		static auto make_native_declaration(
			JSContext* context,
			T& value
		) -> JSValue
		{
			return Converter::to_string(context, value);
		}
	};

	template <>
	struct Generalization<bool> {
		static auto make_native_declaration(
			JSContext* context,
			bool& value
		) -> JSValue
		{
			return Converter::to_bool(context, value);
		}
	};


}