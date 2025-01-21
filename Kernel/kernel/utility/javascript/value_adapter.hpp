#pragma once

#include "kernel/utility/javascript/generalization.hpp"

namespace Sen::Kernel::JavaScript {

	template <typename Value, typename Child> requires (std::is_class<Value>::value && std::is_class<Child>::value && !std::is_pointer<Value>::value && !std::is_pointer<Child>::value)
	inline static auto to_array_of_string(
		JSContext* context,
		Value* list
	) -> JSValue
	{
		auto destination = JS_NewArray(context);
		for (auto index : Range<std::size_t>(list->size)) {
			auto atom_index = JS_NewAtomUInt32(context, static_cast<std::uint32_t>(index));
			auto& str_view = list->value[index];
			auto js_str = JS_NewStringLen(context, reinterpret_cast<const char*>(str_view.value), str_view.size);
			JS_SetProperty(context, destination, atom_index, js_str);
			JS_FreeAtom(context, atom_index);
		}
		return destination;
	}

	template <typename Value, typename Child> requires (std::is_class<Value>::value && std::is_class<Child>::value && !std::is_pointer<Value>::value && !std::is_pointer<Child>::value)
	inline static auto to_string_list(
		JSContext* context,
		JSValue value,
		Value& list
	) -> void
	{
		auto length_atom = JS_NewAtomLen(context, "length", std::strlen("length"));
		auto array_length = Converter::get_property_uint32(context, value, length_atom);
		JS_FreeAtom(context, length_atom);
		list.size = array_length;
		list.value = new Child[array_length];
		for (auto i : Range{ array_length }) {
			auto atom = JS_NewAtomUInt32(context, i);
			auto js_element = JS_GetProperty(context, value, atom);
			JS_FreeAtom(context, atom);
			auto str_len = size_t{};
			auto str = JS_ToCStringLen(context, &str_len, js_element);
			auto temporary = std::unique_ptr<uint8_t[]>{new uint8_t[str_len + 1]};
			temporary.get()[str_len] = '\0';
			std::memcpy(temporary.get(), str, str_len);
			list.value[i].value = temporary.release();
			list.value[i].size = str_len;
			JS_FreeCString(context, str);
			JS_FreeValue(context, js_element);
		}
		return;
	}

	template <typename Value> requires (std::is_class<Value>::value && !std::is_pointer<Value>::value)
	inline static auto to_string(
		JSContext* context,
		Value* value
	) -> JSValue
	{
		return JS_NewStringLen(context, reinterpret_cast<const char*>(value->value), value->size);
	}

	template <typename T> requires is_container<T>::value && std::is_class<T>::value
	static auto make_array(
		JSContext* context,
		const T& value
	) -> JSValue
	{
		return Converter::to_array<T>(context, value);
	}

	template <typename T> requires is_map<T>::value && std::is_class<T>::value
	static auto make_object(
		JSContext* context,
		const T& value
	) -> JSValue
	{
		// TODO
		return JS_UNDEFINED;
	}

	template <typename T>
	inline auto to_value(
		JSContext* context, 
		const T& value
	) -> JSValue {
		static_assert(false, "Base case of to_value is not implemented");
	}

	template <>
	inline auto to_value<bool>(
		JSContext* context, 
		const bool& value
	) -> JSValue {
		return Converter::to_bool(context, value);
	}

	template <typename T> requires (std::is_integral_v<T> && !std::is_floating_point_v<T> && !std::is_same_v<T, bool>)
	inline auto to_value(
		JSContext* context, 
		const T& value
	) -> JSValue {
		static_assert(sizeof(T) != sizeof(bool), "value cannot be bool");
		return Converter::to_bigint<T>(context, value);
	}

	template <typename T> requires (std::is_floating_point_v<T> && !std::is_integral_v<T> && !std::is_same_v<T, bool>)
	inline auto to_value(
		JSContext* context, 
		const T& value
	) -> JSValue {
		static_assert(sizeof(T) != sizeof(bool), "value cannot be bool");
		return Converter::to_number(context, value);
	}

	template <typename T> requires std::is_same_v<T, std::string> || std::is_same_v<T, std::string_view>
	inline auto to_value(
		JSContext* context, 
		const T& value
	) -> JSValue {
		return Converter::to_string(context, value);
	}

	template <typename T> requires is_map<T>::value
	inline auto to_value(
		JSContext* context, 
		const T& value
	) -> JSValue {
		return make_object<T>(context, value);
	}

	template <typename T> requires is_container<T>::value
	inline auto to_value(
		JSContext* context, 
		const T& value
	) -> JSValue {
		return make_array<T>(context, value);
	}

	template <>
	inline auto to_value<JSValue>(
		JSContext* context, 
		const JSValue& value
	) -> JSValue {
		return value;
	}

	template <typename T>
	inline auto from_value(
		JSContext* context,
		JSValue value
	) -> T
	{
		static_assert(false, "Base case of from_value need to be implemented!");
	}

	template <>
	inline auto from_value<std::string>(
		JSContext* context,
		JSValue value
	) -> std::string
	{
		return Converter::get_string(context, value);
	}

	template <>
	inline auto from_value<bool>(
		JSContext* context,
		JSValue value
	) -> bool
	{
		return Converter::get_bool(context, value);
	}

	template <>
	inline auto from_value<JSValue>(
		JSContext* context,
		JSValue value
	) -> JSValue
	{
		return value;
	}

	template <typename T> requires (std::is_integral_v<T> && !std::is_unsigned<T>::value && !std::is_same<T, bool>::value)
	inline auto from_value(
		JSContext* context,
		JSValue value
	) -> T
	{
		return static_cast<T>(Converter::get_bigint64(context, value));
	}

	template <typename T> requires (std::is_integral_v<T> && std::is_unsigned<T>::value && !std::is_same<T, bool>::value)
	inline auto from_value(
		JSContext* context,
		JSValue value
	) -> T
	{
		return static_cast<T>(Converter::get_uint64(context, value));
	}

	template <typename T> requires (!std::is_integral_v<T> && std::is_floating_point<T>::value && !std::is_same<T, bool>::value)
	inline auto from_value(
		JSContext* context,
		JSValue value
	) -> T
	{
		return static_cast<T>(Converter::get_float64(context, value));
	}

	struct ArrayBuffer {
		uint8_t* value{ nullptr };
		size_t size{ 0 };
	};

	// ArrayBuffer
	template <>
	inline auto from_value<std::shared_ptr<ArrayBuffer>>(
		JSContext* context,
		JSValue value
	) -> std::shared_ptr<ArrayBuffer>
	{
		assert_conditional(static_cast<bool>(JS_IsArrayBuffer(value)), "Value must be ArrayBuffer, but it isn't", "from_value");
		auto destination = std::make_shared<ArrayBuffer>();
		destination->value = Converter::get_array_buffer(context, &destination->size, value);
		return destination;
	}


	template <>
	inline auto to_value<std::shared_ptr<ArrayBuffer>>(
		JSContext* context, 
		const std::shared_ptr<ArrayBuffer>& value
	) -> JSValue {
		return JS_NewArrayBufferCopy(context, value->value, value->size);
	}

	template <typename T> requires std::is_same<T, List<container_value_t<T>>>::value
	inline auto from_value(
		JSContext* context,
		JSValue value
	) -> T
	{
		assert_conditional(static_cast<bool>(JS_IsArray(context, value)), "Value must be Array, but it isn't", "from_value");
		auto destination = T{};
		auto atom = JS_NewAtomLen(context, "length", std::strlen("length"));
		auto length = Converter::get_property_uint32(context, value, atom);
		JS_FreeAtom(context, atom);
		destination.reserve(length);
		for (auto index : Range{length}) {
			auto atom = JS_NewAtomUInt32(context, index);
			auto current_value = JS_GetProperty(context, value, atom);
			destination.emplace_back(from_value<container_value_t<T>>(context, current_value));
			JS_FreeAtom(context, atom);
		}
		return destination;
	}

	template <typename T> requires is_array<T>::value
	inline auto from_value(
		JSContext* context,
		JSValue value
	) -> T
	{
		using ElementType = typename T::value_type;
		constexpr auto Size = std::tuple_size_v<T>;
		assert_conditional(static_cast<bool>(JS_IsArray(context, value)), "Value must be Array, but it isn't", "from_value");
		auto destination = T{};
		auto atom = JS_NewAtomLen(context, "length", std::strlen("length"));
		auto length = Converter::get_property_uint32(context, value, atom);
		JS_FreeAtom(context, atom);
		assert_conditional(length == Size, fmt::format("Expected array size: {} but got {}", Size, length), "from_value");
		for (auto index : Range{length}) {
			auto atom = JS_NewAtomUInt32(context, index);
			auto current_value = JS_GetProperty(context, value, atom);
			destination[index] = from_value<ElementType>(context, current_value);
			JS_FreeAtom(context, atom);
		}
		return destination;
	}

	struct ImageView {
		int width{};
		int height{};
		int bit_depth{};
		int color_type{};
		int interlace_type{};
		int channels{};
		int rowbytes{};
		ArrayBuffer data{};
	};

	struct VImageView : ImageView {
		int x{};
		int y{};
	};

	using Dimension = Kernel::Dimension<int32_t>;

	using ExtendedRectangle = Kernel::RectangleFileIO<int32_t>;

	template <>
	inline auto from_value<std::shared_ptr<Dimension>>(
		JSContext* context,
		JSValue value
	) -> std::shared_ptr<Dimension>
	{
		auto current_value = Value::as_new_reference(context, value);
		assert_conditional(current_value.is_object(), "Value must be object, but it isn't", "from_value");
		auto destination = std::make_shared<Dimension>();
		destination->width = current_value.get_property("width").template get_bigint<int>();
		destination->height = current_value.get_property("height").template get_bigint<int>();
		return destination;
	}

	template <>
	inline auto from_value<std::shared_ptr<ExtendedRectangle>>(
		JSContext* context,
		JSValue value
	) -> std::shared_ptr<ExtendedRectangle>
	{
		auto current_value = Value::as_new_reference(context, value);
		assert_conditional(current_value.is_object(), "Value must be object, but it isn't", "from_value");
		auto destination = std::make_shared<ExtendedRectangle>();
		destination->width = current_value.get_property("width").template get_bigint<int>();
		destination->height = current_value.get_property("height").template get_bigint<int>();
		destination->x = current_value.get_property("x").template get_bigint<int>();
		destination->y = current_value.get_property("y").template get_bigint<int>();
		destination->destination = current_value.get_property("destination").template get<std::string>();
		return destination;
	}

	template <>
	inline auto to_value<std::shared_ptr<ExtendedRectangle>>(
		JSContext* context,
		const std::shared_ptr<ExtendedRectangle>& rectangle
	) -> JSValue
	{
		auto destination = Value::as_new_instance(context, JS_UNINITIALIZED);
		destination.set_object();
		destination.define_property("width", to_value<int64_t>(context, static_cast<int64_t>(rectangle->width)));
		destination.define_property("height", to_value<int64_t>(context, static_cast<int64_t>(rectangle->height)));
		destination.define_property("x", to_value<int64_t>(context, static_cast<int64_t>(rectangle->x)));
		destination.define_property("y", to_value<int64_t>(context, static_cast<int64_t>(rectangle->y)));
		destination.define_property("destination", to_value<std::string>(context, rectangle->destination));
		return destination.release();
	}

	template <>
	inline auto to_value<std::shared_ptr<Dimension>>(
		JSContext* context,
		const std::shared_ptr<Dimension>& image
	) -> JSValue
	{
		auto destination = Value::as_new_instance(context, JS_UNINITIALIZED);
		destination.set_object();
		destination.define_property("width", to_value<int64_t>(context, static_cast<int64_t>(image->width)));
		destination.define_property("height", to_value<int64_t>(context, static_cast<int64_t>(image->height)));
		return destination.release();
	}

	template <>
	inline auto from_value<std::shared_ptr<ImageView>>(
		JSContext* context,
		JSValue value
	) -> std::shared_ptr<ImageView>
	{
		auto current_value = Value::as_new_reference(context, value);
		assert_conditional(current_value.is_object(), "Value must be object, but it isn't", "from_value");
		auto destination = std::make_shared<ImageView>();
		destination->width = current_value.get_property("width").template get_bigint<int>();
		destination->height = current_value.get_property("height").template get_bigint<int>();
		destination->bit_depth = current_value.get_property("bit_depth").template get_bigint<int>();
		destination->color_type = current_value.get_property("color_type").template get_bigint<int>();
		destination->interlace_type = current_value.get_property("interlace_type").template get_bigint<int>();
		destination->channels = current_value.get_property("channels").template get_bigint<int>();
		destination->rowbytes = current_value.get_property("rowbytes").template get_bigint<int>();
		auto data = current_value.get_property("data").value;
		assert_conditional(static_cast<bool>(JS_IsArrayBuffer(data)), "Value must be ArrayBuffer, but it isn't", "from_value");
		destination->data.value = JS_GetArrayBuffer(context, &destination->data.size, data);
		return destination;
	}

	template <>
	inline auto to_value<std::shared_ptr<ImageView>>(
		JSContext* context,
		const std::shared_ptr<ImageView>& image
	) -> JSValue
	{
		auto destination = Value::as_new_instance(context, JS_UNINITIALIZED);
		destination.set_object();
		destination.define_property("width", to_value<int64_t>(context, static_cast<int64_t>(image->width)));
		destination.define_property("height", to_value<int64_t>(context, static_cast<int64_t>(image->height)));
		destination.define_property("bit_depth", to_value<int64_t>(context, static_cast<int64_t>(image->bit_depth)));
		destination.define_property("color_type", to_value<int64_t>(context, static_cast<int64_t>(image->color_type)));
		destination.define_property("interlace_type", to_value<int64_t>(context, static_cast<int64_t>(image->interlace_type)));
		destination.define_property("channels", to_value<int64_t>(context, static_cast<int64_t>(image->channels)));
		destination.define_property("rowbytes", to_value<int64_t>(context, static_cast<int64_t>(image->rowbytes)));
		destination.define_property("data", JS_NewArrayBufferCopy(context, image->data.value, image->data.size));
		return destination.release();
	}

	template <>
	inline auto from_value<std::shared_ptr<VImageView>>(
		JSContext* context,
		JSValue value
	) -> std::shared_ptr<VImageView>
	{
		auto current_value = Value::as_new_reference(context, value);
		assert_conditional(current_value.is_object(), "Value must be object, but it isn't", "from_value");
		auto destination = std::make_shared<VImageView>();
		destination->width = current_value.get_property("width").template get_bigint<int>();
		destination->height = current_value.get_property("height").template get_bigint<int>();
		destination->bit_depth = current_value.get_property("bit_depth").template get_bigint<int>();
		destination->color_type = current_value.get_property("color_type").template get_bigint<int>();
		destination->interlace_type = current_value.get_property("interlace_type").template get_bigint<int>();
		destination->channels = current_value.get_property("channels").template get_bigint<int>();
		destination->rowbytes = current_value.get_property("rowbytes").template get_bigint<int>();
		destination->x = current_value.get_property("x").template get_bigint<int>();
		destination->y = current_value.get_property("y").template get_bigint<int>();
		auto data = current_value.get_property("data").value;
		assert_conditional(static_cast<bool>(JS_IsArrayBuffer(data)), "Value must be ArrayBuffer, but it isn't", "from_value");
		destination->data.value = JS_GetArrayBuffer(context, &destination->data.size, data);
		return destination;
	}

	template <>
	inline auto to_value<std::shared_ptr<VImageView>>(
		JSContext* context,
		const std::shared_ptr<VImageView>& image
	) -> JSValue
	{
		auto destination = Value::as_new_instance(context, JS_UNINITIALIZED);
		destination.set_object();
		destination.define_property("width", to_value<int64_t>(context, static_cast<int64_t>(image->width)));
		destination.define_property("height", to_value<int64_t>(context, static_cast<int64_t>(image->height)));
		destination.define_property("x", to_value<int64_t>(context, static_cast<int64_t>(image->x)));
		destination.define_property("y", to_value<int64_t>(context, static_cast<int64_t>(image->y)));
		destination.define_property("bit_depth", to_value<int64_t>(context, static_cast<int64_t>(image->bit_depth)));
		destination.define_property("color_type", to_value<int64_t>(context, static_cast<int64_t>(image->color_type)));
		destination.define_property("interlace_type", to_value<int64_t>(context, static_cast<int64_t>(image->interlace_type)));
		destination.define_property("channels", to_value<int64_t>(context, static_cast<int64_t>(image->channels)));
		destination.define_property("rowbytes", to_value<int64_t>(context, static_cast<int64_t>(image->rowbytes)));
		destination.define_property("data", JS_NewArrayBufferCopy(context, image->data.value, image->data.size));
		return destination.release();
	}

	using Rectangle = Kernel::Rectangle<int32_t>;

	template <>
	inline auto from_value<std::shared_ptr<Rectangle>>(
		JSContext* context,
		JSValue value
	) -> std::shared_ptr<Rectangle>
	{
		auto current_value = Value::as_new_reference(context, value);
		assert_conditional(current_value.is_object(), "Value must be object, but it isn't", "from_value");
		auto destination = std::make_shared<Rectangle>();
		destination->width = current_value.get_property("width").template get_bigint<int>();
		destination->height = current_value.get_property("height").template get_bigint<int>();
		destination->x = current_value.get_property("x").template get_bigint<int>();
		destination->y = current_value.get_property("y").template get_bigint<int>();
		return destination;
	}

	template <>
	inline auto from_value<std::shared_ptr<APNGMakerSetting>>(
		JSContext* context,
		JSValue value
	) -> std::shared_ptr<APNGMakerSetting>
	{
		auto current_value = Value::as_new_reference(context, value);
		assert_conditional(current_value.is_object(), "Value must be object, but it isn't", "from_value");
		auto destination = std::make_shared<APNGMakerSetting>();
		destination->delay_frames_list = JavaScript::from_value<List<uint32_t>>(context, current_value.get_property("delay_frames_list").value);
		destination->loop = JavaScript::from_value<uint32_t>(context, current_value.get_property("loop").value);
		destination->width = JavaScript::from_value<uint32_t>(context, current_value.get_property("width").value);
		destination->height = JavaScript::from_value<uint32_t>(context, current_value.get_property("height").value);
		destination->trim = JavaScript::from_value<bool>(context, current_value.get_property("trim").value);
		return destination;
	}

}