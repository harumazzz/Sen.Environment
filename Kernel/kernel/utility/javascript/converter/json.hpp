#pragma once


#include "kernel/utility/javascript/common.hpp"
#include "kernel/utility/javascript/value.hpp"
#include "kernel/utility/javascript/runtime.hpp"

namespace Sen::Kernel::JavaScript {

	using Map = nlohmann::ordered_json;

	using Array = nlohmann::ordered_json;

	using json = nlohmann::ordered_json;

	enum class Tag : uint8_t {

		number,

		bigint,

		string,

		boolean,

		array,

		object,

		null,

		undefined,

	};

	inline auto get_tag (
		Value& value
	) -> std::optional<Tag>;

	inline auto from (
		Value& value
	) -> json;

	inline auto to (
		JSContext* context,
		json& value
	) -> JSValue;

	template<Tag T>
	struct JSON;

	template <>
	struct JSON<Tag::number> {

		static auto from (
			Value& value
		) -> double
		{
			return value.get_number<double>();
		}

		static auto to (
			JSContext* context,
			json& value
		) -> JSValue
		{
			auto destination = Value::as_new_instance(context, JS_UNINITIALIZED);
			destination.set_number(value.get<double>());
			return destination.release();
		}

	};

	template <>
	struct JSON<Tag::bigint> {

		static auto from (
			Value& value
		) -> int64_t
		{
			return value.get_bigint<int64_t>();
		}

		template <typename T> requires std::is_integral<T>::value
		static auto to (
			JSContext* context,
			json& value
		) -> JSValue
		{
			auto destination = Value::as_new_instance(context, JS_UNINITIALIZED);
			destination.set_bigint(value.get<T>());
			return destination.release();
		}

	};

	template <>
	struct JSON<Tag::string> {

		static auto from (
			Value& value
		) -> std::string
		{
			return value.get<std::string>();
		}

		static auto to (
			JSContext* context,
			json& value
		) -> JSValue
		{
			auto destination = Value::as_new_instance(context, JS_UNINITIALIZED);
			destination.set_string(value.get<std::string>());
			return destination.release();
		}

	};

	template <>
	struct JSON<Tag::boolean> {

		static auto from (
			Value& value
		) -> bool
		{
			return value.get<bool>();
		}

		static auto to (
			JSContext* context,
			json& value
		) -> JSValue
		{
			auto destination = Value::as_new_instance(context, JS_UNINITIALIZED);
			destination.set_boolean(value.get<bool>());
			return destination.release();
		}

	};

	template <>
	struct JSON<Tag::null> {

		static auto from (
			Value& value
		) -> std::nullptr_t
		{
			return nullptr;
		}

		static auto to (
			JSContext* context,
			json& value
		) -> JSValue
		{
			auto destination = Value::as_new_instance(context, JS_UNINITIALIZED);
			destination.set_null();
			return destination.release();
		}

	};

	template <>
	struct JSON<Tag::undefined> {

		static auto from (
			Value& value
		) -> std::nullptr_t
		{
			return nullptr;
		}

		static auto to (
			JSContext* context,
			json& value
		) -> JSValue
		{
			auto destination = Value::as_new_instance(context, JS_UNINITIALIZED);
			destination.set_undefined();
			return destination.release();
		}

	};

	template <>
	struct JSON<Tag::array> {

		static auto from (
			Value& value
		) -> Array
		{
			auto length = static_cast<std::size_t>(value.get_property("length").get_number<double>());
			auto destination = nlohmann::ordered_json::array();
			auto &array = destination.get_ref<nlohmann::ordered_json::array_t &>();
			array.reserve(length); 
			for (auto index : Range(length)) {
				auto element = value.get_property(index);
				destination.emplace_back(JavaScript::from(element));
			}
			return destination;
		}

		static auto to (
			JSContext* context,
			json& value
		) -> JSValue
		{
			auto destination = Value::as_new_instance(context, JS_UNINITIALIZED);
			destination.set_array();
			for (auto index : Range<size_t>(value.size())) {
				destination.set_property(index, JavaScript::to(context, value[index]));
			}
			return destination.release();
		}
	};
	
	template <>
	struct JSON<Tag::object> {

		static auto from (
			Value& value
		) -> Map
		{
			auto property_count = std::uint32_t{};
			auto property_enum = std::add_pointer_t<JSPropertyEnum>{nullptr};
			JS_GetOwnPropertyNames(value._context(), &property_enum, &property_count, value.value, JS_GPN_STRING_MASK);
			auto destination = nlohmann::ordered_json::object();
			for (auto index : Range(property_count)) {
				auto keystring = JS_AtomToString(value._context(), property_enum[index].atom);
				auto size = std::size_t{};
				auto key = JS_ToCStringLen(value._context(), &size, keystring);
				auto element = Value::as_new_instance(value._context(), JS_GetProperty(value._context(), value.value, property_enum[index].atom));
				if (!element.is_undefined()) {
					destination.emplace(std::string{key, size}, JavaScript::from(element)); 
				}
				JS_FreeValue(value._context(), keystring);
				JS_FreeCString(value._context(), key);
			}
			JS_FreePropertyEnum(value._context(), property_enum, property_count);
			return destination;
		}

		static auto to (
			JSContext* context,
			json& value
		) -> JSValue
		{
			auto destination = Value::as_new_instance(context, JS_UNINITIALIZED);
			destination.set_object();
			for (auto & [key, val] : value.items()) {
				destination.set_property(key, JavaScript::to(context, val));
			}
			return destination.release();
		}
	};
	
	inline auto get_tag (
		Value& value
	) -> std::optional<Tag>
	{
		if (value.is_number()) {
			return Tag::number;
		}
		else if (value.is_bigint()) {
			return Tag::bigint;
		}
		else if (value.is_string()) {
			return Tag::string;
		}
		else if (value.is_boolean()) {
			return Tag::boolean;
		}
		else if (value.is_array()) {
			return Tag::array;
		}
		else if (value.is_object()) {
			return Tag::object;
		}
		else if (value.is_null()) {
			return Tag::null;
		}
		else if (value.is_undefined()) {
			return Tag::undefined; 
		}
		else {
			return std::nullopt;
		}
	}

	inline auto from (
		Value& value
	) -> json
	{
		auto tag = get_tag(value);
		assert_conditional(tag != std::nullopt, "JS Value is invalid", "from");
		switch (tag.operator*()) {
			case Tag::string:
				return JSON<Tag::string>::from(value);
			case Tag::number:
				return JSON<Tag::number>::from(value);
			case Tag::bigint:
				return JSON<Tag::bigint>::from(value);
			case Tag::boolean:
				return JSON<Tag::boolean>::from(value);
			case Tag::null:
				return JSON<Tag::null>::from(value);
			case Tag::undefined:
				return JSON<Tag::undefined>::from(value);
			case Tag::array:
				return JSON<Tag::array>::from(value);
			case Tag::object:
				return JSON<Tag::object>::from(value);
			default:
				assert_conditional(false, "Invalid tag, the tag got is not defined in the tag enumeration", "from");
		}
	}

	inline auto to (
		JSContext* context,
		json& value
	) -> JSValue
	{
		switch (value.type()) {
			case nlohmann::ordered_json::value_t::null:
				return JSON<Tag::null>::to(context, value);
			case nlohmann::ordered_json::value_t::object:
				return JSON<Tag::object>::to(context, value);
			case nlohmann::ordered_json::value_t::array:
				return JSON<Tag::array>::to(context, value);
			case nlohmann::ordered_json::value_t::string:
				return JSON<Tag::string>::to(context, value);
			case nlohmann::ordered_json::value_t::boolean:
				return JSON<Tag::boolean>::to(context, value);
			case nlohmann::ordered_json::value_t::number_integer:
				return JSON<Tag::bigint>::to<int64_t>(context, value);
			case nlohmann::ordered_json::value_t::number_unsigned:
				return JSON<Tag::bigint>::to<uint64_t>(context, value);
			case nlohmann::ordered_json::value_t::number_float:
				return JSON<Tag::number>::to(context, value);
			default:
				assert_conditional(false, "Invalid type, the specified type is not defined", "to");
		}
	}
	

}