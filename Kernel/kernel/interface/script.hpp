#pragma once

#include "kernel/interface/shell.hpp"
#include "kernel/utility/javascript/builder.hpp"

namespace Sen::Kernel::Interface::Script
{

	namespace Localization = Kernel::Language;

	namespace JS = Kernel::JavaScript;

	inline auto constexpr get_property_string = JS::Converter::get_property_string;

	inline auto constexpr get_property_int32 = JS::Converter::get_property_int32;

	inline auto constexpr get_property_uint32 = JS::Converter::get_property_uint32;

	inline auto constexpr get_property_float64 = JS::Converter::get_property_float64;

	inline auto constexpr get_property_float32 = JS::Converter::get_property_float32;

	inline auto constexpr get_property_bigint64 = JS::Converter::get_property_bigint64;

	inline auto constexpr get_property_uint64 = JS::Converter::get_property_uint64;

	inline auto constexpr get_property_bool = JS::Converter::get_property_bool;

	template <typename T> requires std::is_class<T>::value
	inline auto constexpr initialize_constructor = JS::initialize_constructor<T>;

	template <typename T, int magic> requires (std::is_class<T>::value && !std::is_pointer<T>::value)
	inline auto constexpr generate_getter_setter = JS::generate_getter_setter<T, magic>;

	template <typename T, int length> requires (std::is_class<T>::value && !std::is_pointer<T>::value)
	inline auto constexpr generate_class_function = JS::generate_class_function<T, length>;

	template <typename Class, typename Constructor, std::size_t InstanceCount, std::size_t ProtoFunctionCount> requires (std::is_class<Class>::value && std::is_function<Constructor>::value)
	inline auto constexpr build_class = JS::build_class<Class, Constructor, InstanceCount, ProtoFunctionCount>;

	template <typename T> requires (std::is_class<T>::value && !std::is_pointer<T>::value)
	inline auto constexpr make_class_definition = JS::make_class_definition<T>;

	template <typename T> requires (std::is_class<T>::value && !std::is_pointer<T>::value)
	inline static auto get_opaque_value = JS::get_opaque_value<T>;

	template <typename T> requires (std::is_class<T>::value && !std::is_pointer<T>::value)
	inline auto constexpr make_deleter = JS::make_deleter<T>;

	inline auto constexpr make_instance_object = JS::make_instance_object;

	template <typename T> requires std::is_same<T, uint32_t>::value || std::is_same<T, std::string_view>::value
	inline auto constexpr get_value_from_object = JS::get_value_from_object<T>;

	inline auto constexpr get_prototype_from_object = JS::get_prototype_from_object;

	inline auto constexpr make_instance_of_class = JS::make_instance_of_class;

	inline auto constexpr get_array_buffer = JS::Converter::get_array_buffer;

	inline auto constexpr get_global_object = JS_GetGlobalObject;

	using Atom = JS::Atom;

	using ClassID = JS::ClassID;

	using Value = JS::Value;

	using JavaScriptNativeMethod = JSValue(*)(JSContext *, JSValue, int, JSValue *);

	template <typename Callable> requires std::is_invocable<Callable>::value
	inline static auto proxy_wrapper(
		JSContext* context,
		std::string_view func_name,
		Callable&& callable
	) -> JSValue
	{
		auto result = JS_UNDEFINED;
		try {
			result = JS::Builder<Callable>::make_function_declaration(context, std::forward<Callable>(callable));
		}
		catch (...) {
			auto exception = parse_exception();
			if (exception.function_name.empty()) {
				exception.function_name = func_name;
			}
			result = JS::throw_exception(context, exception.message(), exception.source, exception.function_name);
		}
		return result;
	}

	template <typename T> requires (std::is_class<T>::value && !std::is_pointer<T>::value)
	inline static auto make_handle(
		JSContext* context,
		JSValue value,
		int argc,
		JSValue* argv,
		std::string_view method_name,
		std::function<JSValue(T*)> method,
		JSClassID class_id
	) -> JSValue
	{
		auto normalized_lambda = [&](){
			auto class_pointer = get_opaque_value<T>(context, value, class_id);
			return method(class_pointer);
		};
		return proxy_wrapper<decltype(normalized_lambda)>(context, method_name, std::move(normalized_lambda));
	}

	namespace Class
	{

		namespace DataStreamView
		{

			inline static auto constexpr BooleanConstraint = true;

			template <auto T>
				requires BooleanConstraint
			using Data = Stream<T>;

			template <auto use_big_endian>
				requires BooleanConstraint
			inline static auto class_id = ClassID::temporary();

			template <auto T> requires BooleanConstraint
			inline static auto constructor(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return proxy_wrapper(context, "constructor", [&]() {
					return initialize_constructor<Data<T>>(
						context,
						value,
						argc,
						argv,
						[&](int argc, JSValue* argv) -> Data<T>* {
							if (argc == 1) {
								return new Data<T>(JS::Converter::get_string(context, argv[0]));
							} else if (argc == 0) {
								return new Data<T>();
							}
							return nullptr;
						},
						class_id<T>,
						Kernel::Language::get("data_stream_view_cannot_be_initialize")
					);
				});
			}

			template <auto T> requires BooleanConstraint
			inline static auto constexpr _class_name (

			) -> std::string_view
			{
				if constexpr (T) {
					return "DataStreamViewUseBigEndian";
				}
				else {
					return "DataStreamView";
				}
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto this_class = make_class_definition<Data<T>>(
				_class_name<T>(),
				class_id<T>.value
			);

			template <auto T>
			requires BooleanConstraint
			inline static auto handle(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv,
				std::string_view method_name,
				std::function<JSValue(Data<T>*)> method
			) -> JSValue {
				return make_handle<Data<T>>(context, value, argc, argv, method_name, method, class_id<T>.value);
			}

			template <auto T>
			requires BooleanConstraint
			inline static auto getter(
				JSContext *context,
				JSValue value,
				int magic
			) -> JSValue 
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(context, value, 0, nullptr, "getter", [&](Data<T>* s) {
					if (magic == 0) {
						return JS_NewBigInt64(context, s->read_pos);
					} else {
						return JS_NewBigInt64(context, s->write_pos);
					}
				});
			}

			template <auto T>
			requires BooleanConstraint
			inline static auto setter(
				JSContext *context,
				JSValue value,
				JSValue argv,
				int magic
			) -> JSValue 
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(context, value, 0, nullptr, "setter", [&](Data<T>* s) {
					auto result = JS::Converter::get_bigint64(context, argv);
					if (magic == 0) {
						s->read_pos = result;
					} else {
						s->write_pos = result;
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
			requires BooleanConstraint
			inline static auto size(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue {
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(context, value, argc, argv, "size", [&](Data<T>* s) {
					return JS::Converter::to_bigint<uint64_t>(context, s->size());
				});
			}

			template <auto T>
			requires BooleanConstraint
			inline static auto capacity(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue {
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(context, value, argc, argv, "capacity", [&](Data<T>* s) {
					return JS::Converter::to_bigint<uint64_t>(context, s->capacity());
				});
			}

			template <auto T>
			requires BooleanConstraint
			inline static auto fromString(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue {
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(context, value, argc, argv, "fromString", [&](Data<T>* s) {
					assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "fromString");
					s->fromString(JS::Converter::get_string(context, argv[0]));
					return JS_UNDEFINED;
				});
			}

			template <auto T>
			requires BooleanConstraint
			inline static auto reserve(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue {
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(context, value, argc, argv, "reserve", [&](Data<T>* s) {
					assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "reserve");
					s->reserve(static_cast<std::uint64_t>(JS::Converter::get_bigint64(context, argv[0])));
					return JS_UNDEFINED;
				});
			}

			#pragma region convert

			inline static auto to_arraybuffer(
				JSContext *context,
				const List<uint8_t>& vec
			) -> JSValue {
				return JS_NewArrayBufferCopy(context, vec.data(), vec.size());
			}

			#pragma endregion

			template <auto T>
			requires BooleanConstraint
			inline static auto toArrayBuffer(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue {
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(context, value, argc, argv, "toArrayBuffer", [&](Data<T>* s) {
					return to_arraybuffer(context, s->toBytes());
				});
			}

			template <auto T>
			requires BooleanConstraint
			inline static auto getArrayBuffer(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue {
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(context, value, argc, argv, "getArrayBuffer", [&](Data<T>* s) {
					assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "getArrayBuffer");
					auto from = JS::Converter::get_bigint64(context, argv[0]);
					auto to = JS::Converter::get_bigint64(context, argv[1]);
					return to_arraybuffer(context, s->get(from, to));
				});
			}


			template <auto T>
			requires BooleanConstraint
			inline static auto toString(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue 
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(context, value, argc, argv, "toString", [&](Data<T>* s) {
					return JS::Converter::to_string(context, s->toString());
				});
			}

			template <auto T>
			requires BooleanConstraint
			inline static auto out_file(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue {
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(context, value, argc, argv, "out_file", [&](Data<T>* s) {
					assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "out_file");
					s->out_file(JS::Converter::get_string(context, argv[0]));
					return JS_UNDEFINED;
				});
			}

			template <auto T>
			requires BooleanConstraint
			inline static auto allocate(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue {
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(context, value, argc, argv, "allocate", [&](Data<T>* s) {
					assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "allocate");
					s->allocate(static_cast<std::size_t>(JS::Converter::get_bigint64(context, argv[0])));
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeUint8(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(context, value, argc, argv, "writeUint8", [&](Data<T>* s) {
					assert_conditional(argc == 2 || argc == 1, fmt::format("argument expected 2 or 1, received: {}", argc), "writeUint8");
					if (argc == 1) {
						s->writeUint8(static_cast<uint8_t>(JS::Converter::get_bigint64(context, argv[0])));
					}
					else {
						s->writeUint8(static_cast<uint8_t>(JS::Converter::get_bigint64(context, argv[0])),static_cast<size_t>(JS::Converter::get_bigint64(context, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeUint16(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(context, value, argc, argv, "writeUint16", [&](Data<T>* s) {
					assert_conditional(argc == 2 || argc == 1, fmt::format("argument expected 2 or 1, received: {}", argc), "writeUint16");
					if (argc == 1) {
						s->writeUint16(static_cast<uint16_t>(JS::Converter::get_bigint64(context, argv[0])));
					}
					else {
						s->writeUint16(static_cast<uint16_t>(JS::Converter::get_bigint64(context, argv[0])),static_cast<size_t>(JS::Converter::get_bigint64(context, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeUint24(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(context, value, argc, argv, "writeUint24", [&](Data<T>* s) {
					assert_conditional(argc == 2 || argc == 1, fmt::format("argument expected 2 or 1, received: {}", argc), "writeUint24");
					if (argc == 1) {
						s->writeUint24(static_cast<uint32_t>(JS::Converter::get_bigint64(context, argv[0])));
					}
					else {
						s->writeUint24(static_cast<uint32_t>(JS::Converter::get_bigint64(context, argv[0])),static_cast<size_t>(JS::Converter::get_bigint64(context, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeUint32(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(context, value, argc, argv, "writeUint32", [&](Data<T>* s) {
					assert_conditional(argc == 2 || argc == 1, fmt::format("argument expected 2 or 1, received: {}", argc), "writeUint32");
					if (argc == 1) {
						s->writeUint32(static_cast<uint32_t>(JS::Converter::get_bigint64(context, argv[0])));
					}
					else {
						s->writeUint32(static_cast<uint32_t>(JS::Converter::get_bigint64(context, argv[0])),static_cast<size_t>(JS::Converter::get_bigint64(context, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeUint64(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(context, value, argc, argv, "writeUint64", [&](Data<T>* s) {
					assert_conditional(argc == 2 || argc == 1, fmt::format("argument expected 2 or 1, received: {}", argc), "writeUint64");
					if (argc == 1) {
						s->writeUint64(static_cast<uint64_t>(JS::Converter::get_bigint64(context, argv[0])));
					}
					else {
						s->writeUint64(static_cast<uint64_t>(JS::Converter::get_bigint64(context, argv[0])),static_cast<size_t>(JS::Converter::get_bigint64(context, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeInt8(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(context, value, argc, argv, "writeInt8", [&](Data<T>* s) {
					assert_conditional(argc == 2 || argc == 1, fmt::format("argument expected 2 or 1, received: {}", argc), "writeInt8");
					if (argc == 1) {
						s->writeInt8(static_cast<int8_t>(JS::Converter::get_bigint64(context, argv[0])));
					}
					else {
						s->writeInt8(static_cast<int8_t>(JS::Converter::get_bigint64(context, argv[0])),static_cast<size_t>(JS::Converter::get_bigint64(context, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeInt16(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(context, value, argc, argv, "writeInt16", [&](Data<T>* s) {
					assert_conditional(argc == 2 || argc == 1, fmt::format("argument expected 2 or 1, received: {}", argc), "writeInt16");
					if (argc == 1) {
						s->writeInt16(static_cast<int16_t>(JS::Converter::get_bigint64(context, argv[0])));
					}
					else {
						s->writeInt16(static_cast<int16_t>(JS::Converter::get_bigint64(context, argv[0])),static_cast<size_t>(JS::Converter::get_bigint64(context, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeInt24(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(context, value, argc, argv, "writeInt24", [&](Data<T>* s) {
					assert_conditional(argc == 2 || argc == 1, fmt::format("argument expected 2 or 1, received: {}", argc), "writeInt24");
					if (argc == 1) {
						s->writeInt24(static_cast<int32_t>(JS::Converter::get_bigint64(context, argv[0])));
					}
					else {
						s->writeInt24(static_cast<int32_t>(JS::Converter::get_bigint64(context, argv[0])),static_cast<size_t>(JS::Converter::get_bigint64(context, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeInt32(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(context, value, argc, argv, "writeInt32", [&](Data<T>* s) {
					assert_conditional(argc == 2 || argc == 1, fmt::format("argument expected 2 or 1, received: {}", argc), "writeInt32");
					if (argc == 1) {
						s->writeInt32(static_cast<int32_t>(JS::Converter::get_bigint64(context, argv[0])));
					}
					else {
						s->writeInt32(static_cast<int32_t>(JS::Converter::get_bigint64(context, argv[0])),static_cast<size_t>(JS::Converter::get_bigint64(context, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeInt64(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(context, value, argc, argv, "writeInt64", [&](Data<T>* s) {
					assert_conditional(argc == 2 || argc == 1, fmt::format("argument expected 2 or 1, received: {}", argc), "writeInt64");
					if (argc == 1) {
						s->writeInt64(static_cast<int64_t>(JS::Converter::get_bigint64(context, argv[0])));
					}
					else {
						s->writeInt64(static_cast<int64_t>(JS::Converter::get_bigint64(context, argv[0])),static_cast<size_t>(JS::Converter::get_bigint64(context, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			#pragma region convert

			inline static auto from_arraybuffer(
				JSContext *context,
				JSValue array_buffer
			) -> List<uint8_t>
			{
				auto size = size_t{};
				auto data = get_array_buffer(context, &size, array_buffer);
				return make_list(data, size);
			}

			inline static auto from_uint8array (
				JSContext *context,
				JSValue uint8array
			) -> List<uint8_t>
			{
				auto array_buffer = get_value_from_object<std::string_view>(context, uint8array, "buffer");
				auto vec = from_arraybuffer(context, array_buffer);
				JS_FreeValue(context, array_buffer);
				return vec;
			}

			#pragma endregion

			template <auto T>
				requires BooleanConstraint
			inline static auto writeArrayBuffer(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(context, value, argc, argv, "writeArrayBuffer", [&](Data<T>* s) {
					assert_conditional(argc == 1 || argc == 2, fmt::format("argument expected 1 or 2, received: {}", argc), "writeArrayBuffer");
					if (argc == 2) {
						s->writeBytes(from_arraybuffer(context, argv[0]), static_cast<std::size_t>(JS::Converter::get_bigint64(context, argv[1])));
					}
					else {
						s->writeBytes(from_arraybuffer(context, argv[0]));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeFloat(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(context, value, argc, argv, "writeFloat", [&](Data<T>* s) {
					assert_conditional(argc == 1 || argc == 2, fmt::format("argument expected 1 or 2, received: {}", argc), "writeFloat");
					if (argc == 1) {
						s->writeFloat(static_cast<float>(JS::Converter::get_float32(context, argv[0])));
					}
					else {
						s->writeFloat(static_cast<float>(JS::Converter::get_float32(context, argv[0])),static_cast<size_t>(JS::Converter::get_bigint64(context, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeDouble(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(context, value, argc, argv, "writeDouble", [&](Data<T>* s) {
					assert_conditional(argc == 1 || argc == 2, fmt::format("argument expected 1 or 2, received: {}", argc), "writeDouble");
					if (argc == 1) {
						s->writeDouble(static_cast<double>(JS::Converter::get_float64(context, argv[0])));
					}
					else {
						s->writeDouble(static_cast<double>(JS::Converter::get_float64(context, argv[0])),static_cast<size_t>(JS::Converter::get_bigint64(context, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeVarInt32(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(context, value, argc, argv, "writeVarInt32", [&](Data<T>* s) {
					assert_conditional(argc == 1 || argc == 2, fmt::format("argument expected 1 or 2, received: {}", argc), "writeVarInt32");
					if (argc == 1) {
						s->writeVarInt32(static_cast<int32_t>(JS::Converter::get_bigint64(context, argv[0])));
					}
					else {
						s->writeVarInt32(static_cast<int32_t>(JS::Converter::get_bigint64(context, argv[0])),static_cast<size_t>(JS::Converter::get_bigint64(context, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeVarInt64(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(context, value, argc, argv, "writeVarInt64", [&](Data<T>* s) {
					assert_conditional(argc == 1 || argc == 2, fmt::format("argument expected 1 or 2, received: {}", argc), "writeVarInt64");
					if (argc == 1) {
						s->writeVarInt64(static_cast<int64_t>(JS::Converter::get_bigint64(context, argv[0])));
					}
					else {
						s->writeVarInt64(static_cast<int64_t>(JS::Converter::get_bigint64(context, argv[0])),static_cast<size_t>(JS::Converter::get_bigint64(context, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeZigZag32(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(context, value, argc, argv, "writeZigZag32", [&](Data<T>* s) {
					assert_conditional(argc == 1 || argc == 2, fmt::format("argument expected 1 or 2, received: {}", argc), "writeZigZag32");
					if (argc == 1) {
						s->writeZigZag32(static_cast<int32_t>(JS::Converter::get_bigint64(context, argv[0])));
					}
					else {
						s->writeZigZag32(static_cast<int32_t>(JS::Converter::get_bigint64(context, argv[0])),static_cast<size_t>(JS::Converter::get_bigint64(context, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeZigZag64(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(context, value, argc, argv, "writeZigZag64", [&](Data<T>* s) {
					assert_conditional(argc == 1 || argc == 2, fmt::format("argument expected 1 or 2, received: {}", argc), "writeZigZag64");
					if (argc == 1) {
						s->writeZigZag64(static_cast<int64_t>(JS::Converter::get_bigint64(context, argv[0])));
					}
					else {
						s->writeZigZag64(static_cast<int64_t>(JS::Converter::get_bigint64(context, argv[0])),static_cast<size_t>(JS::Converter::get_bigint64(context, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeString(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(context, value, argc, argv, "writeString", [&](Data<T>* s) {
					assert_conditional(argc == 1 || argc == 2, fmt::format("argument expected 1 or 2, received: {}", argc), "writeString");
					if (argc == 1) {
						s->writeString(JS::Converter::get_string(context, argv[0]));
					}
					else {
						s->writeString(JS::Converter::get_string(context, argv[0]),static_cast<size_t>(JS::Converter::get_bigint64(context, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeStringFourByte(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(context, value, argc, argv, "writeStringFourByte", [&](Data<T>* s) {
					assert_conditional(argc == 2 || argc == 1, fmt::format("argument expected 2 or 1, received: {}", argc), "writeStringFourByte");
					if (argc == 1) {
						s->writeStringFourByte(JS::Converter::get_string(context, argv[0]));
					}
					else {
						s->writeStringFourByte(JS::Converter::get_string(context, argv[0]),static_cast<size_t>(JS::Converter::get_bigint64(context, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeNull(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(context, value, argc, argv, "writeNull", [&](Data<T>* s) {
					assert_conditional(argc == 2 || argc == 1, fmt::format("argument expected 2 or 1, received: {}", argc), "writeNull");
					if (argc == 1) {
						s->writeNull(static_cast<uint64_t>(JS::Converter::get_bigint64(context, argv[0])));
					}
					else {
						s->writeNull(static_cast<uint64_t>(JS::Converter::get_bigint64(context, argv[0])),static_cast<size_t>(JS::Converter::get_bigint64(context, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeBoolean(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(context, value, argc, argv, "writeBoolean", [&](Data<T>* s) {
					assert_conditional(argc == 2 || argc == 1, fmt::format("argument expected 2 or 1, received: {}", argc), "writeBoolean");
					if (argc == 1) {
						s->writeBoolean(JS::Converter::get_bool(context, argv[0]));
					}
					else {
						s->writeBoolean(JS::Converter::get_bool(context, argv[0]),static_cast<size_t>(JS::Converter::get_bigint64(context, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeStringByUint8(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(context, value, argc, argv, "writeStringByUint8", [&](Data<T>* s) {
					assert_conditional(argc == 2 || argc == 1, fmt::format("argument expected 2 or 1, received: {}", argc), "writeStringByUint8");
					if (argc == 1) {
						s->writeStringByUint8(JS::Converter::get_string(context, argv[0]));
					}
					else {
						s->writeStringByUint8(JS::Converter::get_string(context, argv[0]),static_cast<size_t>(JS::Converter::get_bigint64(context, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeStringByUint16(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(context, value, argc, argv, "writeStringByUint16", [&](Data<T>* s) {
					assert_conditional(argc == 2 || argc == 1, fmt::format("argument expected 2 or 1, received: {}", argc), "writeStringByUint16");
					if (argc == 1) {
						s->writeStringByUint16(JS::Converter::get_string(context, argv[0]));
					}
					else {
						s->writeStringByUint16(JS::Converter::get_string(context, argv[0]),static_cast<size_t>(JS::Converter::get_bigint64(context, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeStringByUint32(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(context, value, argc, argv, "writeStringByUint32", [&](Data<T>* s) {
					assert_conditional(argc == 2 || argc == 1, fmt::format("argument expected 2 or 1, received: {}", argc), "writeStringByUint32");
					if (argc == 1) {
						s->writeStringByUint32(JS::Converter::get_string(context, argv[0]));
					}
					else {
						s->writeStringByUint32(JS::Converter::get_string(context, argv[0]),static_cast<size_t>(JS::Converter::get_bigint64(context, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeStringByInt8(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(context, value, argc, argv, "writeStringByInt8", [&](Data<T>* s) {
					assert_conditional(argc == 2 || argc == 1, fmt::format("argument expected 2 or 1, received: {}", argc), "writeStringByInt8");
					if (argc == 1) {
						s->writeStringByInt8(JS::Converter::get_string(context, argv[0]));
					}
					else {
						s->writeStringByInt8(JS::Converter::get_string(context, argv[0]),static_cast<size_t>(JS::Converter::get_bigint64(context, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeStringByInt16(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(context, value, argc, argv, "writeStringByInt16", [&](Data<T>* s) {
					assert_conditional(argc == 2 || argc == 1, fmt::format("argument expected 2 or 1, received: {}", argc), "writeStringByInt16");
					if (argc == 1) {
						s->writeStringByInt16(JS::Converter::get_string(context, argv[0]));
					}
					else {
						s->writeStringByInt16(JS::Converter::get_string(context, argv[0]),static_cast<size_t>(JS::Converter::get_bigint64(context, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeStringByInt32(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(context, value, argc, argv, "writeStringByInt32", [&](Data<T>* s) {
					assert_conditional(argc == 2 || argc == 1, fmt::format("argument expected 2 or 1, received: {}", argc), "writeStringByInt32");
					if (argc == 1) {
						s->writeStringByInt32(JS::Converter::get_string(context, argv[0]));
					}
					else {
						s->writeStringByInt32(JS::Converter::get_string(context, argv[0]),static_cast<size_t>(JS::Converter::get_bigint64(context, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeStringByEmpty(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(context, value, argc, argv, "writeStringByEmpty", [&](Data<T>* s) {
					assert_conditional(argc == 2 || argc == 1, fmt::format("argument expected 2 or 1, received: {}", argc), "writeStringByEmpty");
					if (argc == 1) {
						s->writeStringByEmpty(JS::Converter::get_string(context, argv[0]));
					}
					else {
						s->writeStringByEmpty(JS::Converter::get_string(context, argv[0]),static_cast<size_t>(JS::Converter::get_bigint64(context, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readUint8(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(context, value, argc, argv, "readUint8", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readUint8");
					auto value = uint8_t{};
					if (argc == 1) {
						value = s->readUint8(static_cast<size_t>(JS::Converter::get_bigint64(context, argv[0])));
					}
					else {
						value = s->readUint8();
					}
					return JS::Converter::to_bigint<uint8_t>(context, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readUint16(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(context, value, argc, argv, "readUint16", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readUint16");
					auto value = uint16_t{};
					if (argc == 1) {
						value = s->readUint16(static_cast<size_t>(JS::Converter::get_bigint64(context, argv[0])));
					}
					else {
						value = s->readUint16();
					}
					return JS::Converter::to_bigint<uint16_t>(context, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readUint24(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(context, value, argc, argv, "readUint24", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readUint24");
					auto value = uint32_t{};
					if (argc == 1) {
						value = s->readUint24(static_cast<size_t>(JS::Converter::get_bigint64(context, argv[0])));
					}
					else {
						value = s->readUint24();
					}
					return JS::Converter::to_bigint<uint32_t>(context, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readUint32(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(context, value, argc, argv, "readUint32", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readUint32");
					auto value = uint32_t{};
					if (argc == 1) {
						value = s->readUint32(static_cast<size_t>(JS::Converter::get_bigint64(context, argv[0])));
					}
					else {
						value = s->readUint32();
					}
					return JS::Converter::to_bigint<uint32_t>(context, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readUint64(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(context, value, argc, argv, "readUint64", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readUint64");
					auto value = uint64_t{};
					if (argc == 1) {
						value = s->readUint64(static_cast<size_t>(JS::Converter::get_bigint64(context, argv[0])));
					}
					else {
						value = s->readUint64();
					}
					return JS::Converter::to_bigint<uint64_t>(context, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readInt8(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(context, value, argc, argv, "readInt8", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readInt8");
					auto value = int8_t{};
					if (argc == 1) {
						value = s->readInt8(static_cast<size_t>(JS::Converter::get_bigint64(context, argv[0])));
					}
					else {
						value = s->readInt8();
					}
					return JS::Converter::to_bigint<int8_t>(context, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readInt16(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(context, value, argc, argv, "readInt16", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readInt16");
					auto value = int16_t{};
					if (argc == 1) {
						value = s->readInt16(static_cast<size_t>(JS::Converter::get_bigint64(context, argv[0])));
					}
					else {
						value = s->readInt16();
					}
					return JS::Converter::to_bigint<int16_t>(context, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readInt24(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(context, value, argc, argv, "readInt24", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readInt24");
					auto value = int32_t{};
					if (argc == 1) {
						value = s->readInt24(static_cast<size_t>(JS::Converter::get_bigint64(context, argv[0])));
					}
					else {
						value = s->readInt24();
					}
					return JS::Converter::to_bigint<int32_t>(context, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readInt32(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(context, value, argc, argv, "readInt32", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readInt32");
					auto value = int32_t{};
					if (argc == 1) {
						value = s->readInt32(static_cast<size_t>(JS::Converter::get_bigint64(context, argv[0])));
					}
					else {
						value = s->readInt32();
					}
					return JS::Converter::to_bigint<int32_t>(context, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readInt64(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(context, value, argc, argv, "readInt64", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readInt64");
					auto value = int64_t{};
					if (argc == 1) {
						value = s->readInt64(static_cast<size_t>(JS::Converter::get_bigint64(context, argv[0])));
					}
					else {
						value = s->readInt64();
					}
					return JS::Converter::to_bigint<int64_t>(context, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readString(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(context, value, argc, argv, "readString", [&](Data<T>* s) {
					assert_conditional(argc == 1 || argc == 2, fmt::format("argument expected 1 or 2, received: {}", argc), "readString");
					auto value = std::string{};
					if (argc == 1) {
						value = s->readString(static_cast<size_t>(JS::Converter::get_bigint64(context, argv[0])));
					}
					else {
						value = s->readString(static_cast<size_t>(JS::Converter::get_bigint64(context, argv[0])),static_cast<size_t>(JS::Converter::get_bigint64(context, argv[1])));
					}
					return JS::Converter::to_string(context, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readStringByUint8(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(context, value, argc, argv, "readStringByUint8", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readStringByUint8");
					auto value = std::string{};
					if (argc == 1) {
						value = s->readStringByUint8(static_cast<size_t>(JS::Converter::get_bigint64(context, argv[0])));
					}
					else {
						value = s->readStringByUint8();
					}
					return JS::Converter::to_string(context, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readStringByUint16(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(context, value, argc, argv, "readStringByUint16", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readStringByUint16");
					auto value = std::string{};
					if (argc == 1) {
						value = s->readStringByUint16(static_cast<size_t>(JS::Converter::get_bigint64(context, argv[0])));
					}
					else {
						value = s->readStringByUint16();
					}
					return JS::Converter::to_string(context, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readStringByUint32(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(context, value, argc, argv, "readStringByUint32", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readStringByUint32");
					auto value = std::string{};
					if (argc == 1) {
						value = s->readStringByUint32(static_cast<size_t>(JS::Converter::get_bigint64(context, argv[0])));
					}
					else {
						value = s->readStringByUint32();
					}
					return JS::Converter::to_string(context, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readStringByInt8(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(context, value, argc, argv, "readStringByInt8", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readStringByInt8");
					auto value = std::string{};
					if (argc == 1) {
						value = s->readStringByInt8(static_cast<size_t>(JS::Converter::get_bigint64(context, argv[0])));
					}
					else {
						value = s->readStringByInt8();
					}
					return JS::Converter::to_string(context, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readStringByInt16(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(context, value, argc, argv, "readStringByInt16", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readStringByInt16");
					auto value = std::string{};
					if (argc == 1) {
						value = s->readStringByInt16(static_cast<size_t>(JS::Converter::get_bigint64(context, argv[0])));
					}
					else {
						value = s->readStringByInt16();
					}
					return JS::Converter::to_string(context, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readStringByInt32(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(context, value, argc, argv, "readStringByInt32", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readStringByInt32");
					auto value = std::string{};
					if (argc == 1) {
						value = s->readStringByInt32(static_cast<size_t>(JS::Converter::get_bigint64(context, argv[0])));
					}
					else {
						value = s->readStringByInt32();
					}
					return JS::Converter::to_string(context, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readStringByVarInt32(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(context, value, argc, argv, "readStringByVarInt32", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readStringByVarInt32");
					auto value = std::string{};
					if (argc == 1) {
						value = s->readStringByVarInt32(static_cast<size_t>(JS::Converter::get_bigint64(context, argv[0])));
					}
					else {
						value = s->readStringByVarInt32();
					}
					return JS::Converter::to_string(context, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readStringByEmpty(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(context, value, argc, argv, "readStringByEmpty", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readStringByEmpty");
					auto value = std::string{};
					if (argc == 1) {
						value = s->readStringByEmpty(static_cast<size_t>(JS::Converter::get_bigint64(context, argv[0])));
					}
					else {
						value = s->readStringByEmpty();
					}
					return JS::Converter::to_string(context, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readVarInt32(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(context, value, argc, argv, "readVarInt32", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readVarInt32");
					auto value = int32_t{};
					if (argc == 1) {
						value = s->readVarInt32(static_cast<size_t>(JS::Converter::get_bigint64(context, argv[0])));
					}
					else {
						value = s->readVarInt32();
					}
					return JS::Converter::to_bigint<int32_t>(context, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readVarInt64(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(context, value, argc, argv, "readVarInt64", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readVarInt64");
					auto value = int64_t{};
					if (argc == 1) {
						value = s->readVarInt64(static_cast<size_t>(JS::Converter::get_bigint64(context, argv[0])));
					}
					else {
						value = s->readVarInt64();
					}
					return JS::Converter::to_bigint<int64_t>(context, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readVarUint32(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(context, value, argc, argv, "readVarUint32", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readVarUint32");
					auto value = uint32_t{};
					if (argc == 1) {
						value = s->readVarUint32(static_cast<size_t>(JS::Converter::get_bigint64(context, argv[0])));
					}
					else {
						value = s->readVarUint32();
					}
					return JS::Converter::to_bigint<uint32_t>(context, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readVarUint64(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(context, value, argc, argv, "readVarUint64", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readVarUint64");
					auto value = uint64_t{};
					if (argc == 1) {
						value = s->readVarUint64(static_cast<size_t>(JS::Converter::get_bigint64(context, argv[0])));
					}
					else {
						value = s->readVarUint64();
					}
					return JS::Converter::to_bigint<uint64_t>(context, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readZigZag32(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(context, value, argc, argv, "readZigZag32", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readZigZag32");
					auto value = int32_t{};
					if (argc == 1) {
						value = s->readZigZag32(static_cast<size_t>(JS::Converter::get_bigint64(context, argv[0])));
					}
					else {
						value = s->readZigZag32();
					}
					return JS::Converter::to_bigint<int32_t>(context, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readZigZag64(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(context, value, argc, argv, "readZigZag64", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readZigZag64");
					auto value = int64_t{};
					if (argc == 1) {
						value = s->readZigZag64(static_cast<size_t>(JS::Converter::get_bigint64(context, argv[0])));
					}
					else {
						value = s->readZigZag64();
					}
					return JS::Converter::to_bigint<int64_t>(context, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readFloat(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(context, value, argc, argv, "readFloat", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readFloat");
					auto value = float{};
					if (argc == 1) {
						value = s->readFloat(static_cast<size_t>(JS::Converter::get_bigint64(context, argv[0])));
					}
					else {
						value = s->readFloat();
					}
					return JS::Converter::to_number(context, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readDouble(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(context, value, argc, argv, "readDouble", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readDouble");
					auto value = double{};
					if (argc == 1) {
						value = s->readDouble(static_cast<size_t>(JS::Converter::get_bigint64(context, argv[0])));
					}
					else {
						value = s->readDouble();
					}
					return JS::Converter::to_number(context, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto close(
				JSContext *context,
				JSValue value,
				int argc,
				JSValue* argv
			) -> JSValue
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(context, value, argc, argv, "readDouble", [&](Data<T>* s) {
					s->close();
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto proto_functions = std::to_array<JSCFunctionListEntry>({
				generate_getter_setter<Data<T>, 0>("read_position", getter<T>, setter<T>),
				generate_getter_setter<Data<T>, 1>("write_position", getter<T>, setter<T>),
				generate_class_function<Data<T>, 0>("size", size<T>),
				generate_class_function<Data<T>, 1>("fromString", fromString<T>),
				generate_class_function<Data<T>, 0>("capacity", capacity<T>),
				generate_class_function<Data<T>, 1>("reserve", reserve<T>),
				generate_class_function<Data<T>, 0>("toArrayBuffer", toArrayBuffer<T>),
				generate_class_function<Data<T>, 2>("getArrayBuffer", getArrayBuffer<T>),
				generate_class_function<Data<T>, 0>("toString", toString<T>),
				generate_class_function<Data<T>, 1>("out_file", out_file<T>),
				generate_class_function<Data<T>, 2>("writeUint8", writeUint8<T>),
				generate_class_function<Data<T>, 2>("writeUint16", writeUint16<T>),
				generate_class_function<Data<T>, 2>("writeUint24", writeUint24<T>),
				generate_class_function<Data<T>, 2>("writeUint32", writeUint32<T>),
				generate_class_function<Data<T>, 2>("writeUint64", writeUint64<T>),
				generate_class_function<Data<T>, 2>("writeInt8", writeInt8<T>),
				generate_class_function<Data<T>, 2>("writeInt16", writeInt16<T>),
				generate_class_function<Data<T>, 2>("writeInt24", writeInt24<T>),
				generate_class_function<Data<T>, 2>("writeInt32", writeInt32<T>),
				generate_class_function<Data<T>, 2>("writeInt64", writeInt64<T>),
				generate_class_function<Data<T>, 2>("writeArrayBuffer", writeArrayBuffer<T>),
				generate_class_function<Data<T>, 2>("writeFloat", writeFloat<T>),
				generate_class_function<Data<T>, 2>("writeDouble", writeDouble<T>),
				generate_class_function<Data<T>, 2>("writeVarInt32", writeVarInt32<T>),
				generate_class_function<Data<T>, 2>("writeVarInt64", writeVarInt64<T>),
				generate_class_function<Data<T>, 2>("writeZigZag32", writeZigZag32<T>),
				generate_class_function<Data<T>, 2>("writeZigZag64", writeZigZag64<T>),
				generate_class_function<Data<T>, 2>("writeString", writeString<T>),
				generate_class_function<Data<T>, 2>("writeStringFourByte", writeStringFourByte<T>),
				generate_class_function<Data<T>, 2>("writeNull", writeNull<T>),
				generate_class_function<Data<T>, 2>("writeBoolean", writeBoolean<T>),
				generate_class_function<Data<T>, 2>("writeStringByUint8", writeStringByUint8<T>),
				generate_class_function<Data<T>, 2>("writeStringByUint16", writeStringByUint16<T>),
				generate_class_function<Data<T>, 2>("writeStringByUint32", writeStringByUint32<T>),
				generate_class_function<Data<T>, 2>("writeStringByInt8", writeStringByInt8<T>),
				generate_class_function<Data<T>, 2>("writeStringByInt16", writeStringByInt16<T>),
				generate_class_function<Data<T>, 2>("writeStringByInt32", writeStringByInt32<T>),
				generate_class_function<Data<T>, 2>("writeStringByEmpty", writeStringByEmpty<T>),
				generate_class_function<Data<T>, 1>("readUint8", readUint8<T>),
				generate_class_function<Data<T>, 1>("readUint16", readUint16<T>),
				generate_class_function<Data<T>, 1>("readUint24", readUint24<T>),
				generate_class_function<Data<T>, 1>("readUint32", readUint32<T>),
				generate_class_function<Data<T>, 1>("readUint64", readUint64<T>),
				generate_class_function<Data<T>, 1>("readInt8", readInt8<T>),
				generate_class_function<Data<T>, 1>("readInt16", readInt16<T>),
				generate_class_function<Data<T>, 1>("readInt24", readInt24<T>),
				generate_class_function<Data<T>, 1>("readInt32", readInt32<T>),
				generate_class_function<Data<T>, 1>("readInt64", readInt64<T>),
				generate_class_function<Data<T>, 2>("readString", readString<T>),
				generate_class_function<Data<T>, 1>("readStringByUint8", readStringByUint8<T>),
				generate_class_function<Data<T>, 1>("readStringByUint16", readStringByUint16<T>),
				generate_class_function<Data<T>, 1>("readStringByUint32", readStringByUint32<T>),
				generate_class_function<Data<T>, 1>("readStringByInt8", readStringByInt8<T>),
				generate_class_function<Data<T>, 1>("readStringByInt16", readStringByInt16<T>),
				generate_class_function<Data<T>, 1>("readStringByInt32", readStringByInt32<T>),
				generate_class_function<Data<T>, 1>("readStringByVarInt32", readStringByVarInt32<T>),
				generate_class_function<Data<T>, 1>("readStringByEmpty", readStringByEmpty<T>),
				generate_class_function<Data<T>, 1>("readVarInt32", readVarInt32<T>),
				generate_class_function<Data<T>, 1>("readVarInt64", readVarInt64<T>),
				generate_class_function<Data<T>, 1>("readVarUint32", readVarUint32<T>),
				generate_class_function<Data<T>, 1>("readVarUint64", readVarUint64<T>),
				generate_class_function<Data<T>, 1>("readZigZag32", readZigZag32<T>),
				generate_class_function<Data<T>, 1>("readZigZag64", readZigZag64<T>),
				generate_class_function<Data<T>, 1>("readFloat", readFloat<T>),
				generate_class_function<Data<T>, 1>("readDouble", readDouble<T>),
				generate_class_function<Data<T>, 0>("close", close<T>),
				generate_class_function<Data<T>, 1>("allocate", allocate<T>),
			});

			template <auto use_big_endian>
				requires BooleanConstraint
			inline static auto register_class (
				JSContext *context
			) -> void
			{
				static_assert(use_big_endian == true || use_big_endian == false, "use_big_endian can only be true or false");
				static_assert(sizeof(use_big_endian) == sizeof(bool));
				return build_class<decltype(class_id<use_big_endian>), decltype(constructor<use_big_endian>), 2, 67>(context, class_id<use_big_endian>, constructor<use_big_endian>, _class_name<use_big_endian>(), proto_functions<use_big_endian>, this_class<use_big_endian>, std::to_array<std::string_view>({"Sen", "Kernel"}));
			}

		}
	}

	namespace XML
	{

		using XMLDocument = tinyxml2::XMLDocument;

		inline static auto xml2json(
			const tinyxml2::XMLNode *node) -> nlohmann::ordered_json
		{
			auto j = nlohmann::ordered_json{};
			auto element = node->ToElement();
			if (element != nullptr)
			{
				for (auto attr = element->FirstAttribute(); attr; attr = attr->Next())
				{
					j["@attributes"][attr->Name()] = attr->Value();
				}
			}
			if (node->ToText())
			{
				j["@text"] = {
					{"value", node->Value()},
					{"is_cdata", node->ToText()->CData()},
				};
			}
			else
			{
				for (auto child = node->FirstChild(); child; child = child->NextSibling())
				{
					auto child_json = xml2json(child);
					if (j.contains(child->Value()))
					{
						if (j[child->Value()].is_array())
						{
							j[child->Value()].push_back(child_json);
						}
						else
						{
							j[child->Value()] = {j[child->Value()], child_json};
						}
					}
					else
					{
						if (child_json.find("@text") != child_json.end())
						{
							if (std::strcmp(child_json["@text"]["value"].get<std::string>().data(),
											child->Value()) == 0)
							{
								j = child_json;
							}
							else
							{
								j[child->Value()] = child_json;
							}
						}
						else
						{
							j[child->Value()] = child_json;
						}
					}
				}
			}
			return j;
		}


		inline static auto json2xml(
			const nlohmann::ordered_json &j,
			tinyxml2::XMLNode *node,
			tinyxml2::XMLDocument &doc
		) -> void
		{
			if (j.is_object())
			{
				for (auto &[key, value] : j.items())
				{
					switch (hash_string(key)) {
						case hash_string("@attributes"_sv): {
							for (auto &[attribute_key, attribute_value] : value.items())
							{
								dynamic_cast<tinyxml2::XMLElement *>(node)->SetAttribute(attribute_key.data(), attribute_value.get<std::string>().data());
							}
							break;
						}
						case hash_string("@text"_sv): {
							if (!value["is_cdata"].is_null() and value["is_cdata"])
							{
								auto cdata = doc.NewText(value["value"].get<std::string>().data());
								cdata->SetCData(true);
								node->InsertEndChild(cdata);
							}
							else
							{
								dynamic_cast<tinyxml2::XMLElement *>(node)->SetText(value["value"].get<std::string>().data());
							}
							break;
						}
						default: {
							if (value.is_object())
							{
								auto child = doc.NewElement(key.data());
								node->InsertEndChild(child);
								json2xml(value, child, doc);
							}
							else if (value.is_array())
							{
								for (auto &element : value)
								{
									auto child = doc.NewElement(key.data());
									node->InsertEndChild(child);
									json2xml(element, child, doc);
								}
							}
							else if (value.is_null()) {
								auto child = doc.NewElement(key.data());
								node->InsertEndChild(child);
							}
							else
							{
								auto child = doc.NewElement(node->Value());
								node->Parent()->InsertEndChild(child);
							}
							break;
						}

					}
				}
			}
			else if (j.is_array())
			{
				for (auto &element : j)
				{
					auto child = doc.NewElement(node->Value());
					node->Parent()->InsertEndChild(child);
					json2xml(element, child, doc);
				}
			}
			else if (j.is_string())
			{
				node->InsertEndChild(doc.NewText(j.get<std::string>().data()));
			}
			else
			{
				auto child = doc.NewElement(node->Value());
				node->Parent()->InsertEndChild(child);
			}
			return;
		}

		inline static auto convert(
			const nlohmann::ordered_json &j,
			tinyxml2::XMLDocument &doc
		) -> void
		{
			auto &root_name = j.begin().key();
			auto root = doc.NewElement(root_name.data());
			doc.InsertEndChild(root);
			json2xml(j.begin().value(), root, doc);
			return;
		}

		inline static auto deserialize(
			JSContext *context,
			JSValue value,
			int argc,
			JSValue* argv
		) -> JSValue
		{
			return proxy_wrapper(context, "deserialize"_sv, [&]() {
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "deserialize");
				auto doc = tinyxml2::XMLDocument{};
				auto source = JS::Converter::get_string(context, argv[0]);
				auto eResult = doc.Parse(source.data(), source.size());
				if (eResult != tinyxml2::XML_SUCCESS) {
					throw Exception(fmt::format("XML cannot be parsed, data: {}", source), std::source_location::current(), "deserialize");
				}
				auto value = nlohmann::ordered_json{};
				value[doc.RootElement()->Value()] = xml2json(doc.RootElement());
				auto destination = JS::to(context, value);
				return destination; 
			});
		}

		inline static auto deserialize_fs(
			JSContext *context,
			JSValue value,
			int argc,
			JSValue* argv
		) -> JSValue
		{
			return proxy_wrapper(context, "deserialize_fs"_sv, [&]() {
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "deserialize_fs");
				auto doc = tinyxml2::XMLDocument{};
				auto source = JS::Converter::get_string(context, argv[0]);
				auto view = Kernel::FileSystem::read_file(source);
				auto eResult = doc.Parse(view.data(), view.size());
				if (eResult != tinyxml2::XML_SUCCESS) {
					throw Exception(fmt::format("XML cannot be loaded, data: {}", source), std::source_location::current(), "deserialize");
				}
				auto value = nlohmann::ordered_json{};
				value[doc.RootElement()->Value()] = xml2json(doc.RootElement());
				auto destination = JS::to(context, value); 
				return destination;
			});
		}

		inline static auto serialize(
			JSContext *context,
			JSValue value,
			int argc,
			JSValue* argv
		) -> JSValue
		{
			return proxy_wrapper(context, "serialize"_sv, [&]() {
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "serialize");
				auto doc = tinyxml2::XMLDocument{};
				auto value = Value::as_new_reference(context, argv[0]);
				auto j = JS::from(value);
				convert(j, doc);
				auto printer = tinyxml2::XMLPrinter{};
				doc.Print(&printer);
				return std::string{printer.CStr(), static_cast<std::size_t>(printer.CStrSize() - 1)}; 
			});
		}

		inline static auto serialize_fs(
			JSContext *context,
			JSValue valueue,
			int argc,
			JSValue* argv
		) -> JSValue
		{
			return proxy_wrapper(context, "serialize_fs"_sv, [&]() {
				assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "serialize_fs");
				auto doc = XMLDocument{};
				auto value = Value::as_new_reference(context, argv[0]);
				auto source = JS::from(value);
				convert(source, doc);
				auto printer = tinyxml2::XMLPrinter{};
				doc.Print(&printer);
				auto destination = JS::Converter::get_string(context, argv[1]);
				Kernel::FileSystem::write_file(destination, std::string{printer.CStr(), static_cast<std::size_t>(printer.CStrSize() - 1)});
			});
		}

	}

}