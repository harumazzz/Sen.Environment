#pragma once

#include "kernel/interface/shell.hpp"

namespace Sen::Kernel::Interface::Script
{

	/**
	 * To make sure Language is ambigious
	 */

	namespace Localization = Sen::Kernel::Language;

	/**
	 * ----------------------------------------
	 * JS namespace is used
	 * ----------------------------------------
	 */

	namespace JS = Sen::Kernel::Definition::JavaScript;

	inline auto constexpr get_property_string = JS::Converter::get_property_string;

	inline auto constexpr get_property_int32 = JS::Converter::get_property_int32;

	inline auto constexpr get_property_float64 = JS::Converter::get_property_float64;

	inline auto constexpr get_property_float32 = JS::Converter::get_property_float32;

	inline auto constexpr get_property_int64 = JS::Converter::get_property_int64;

	inline auto constexpr get_property_bigint64 = JS::Converter::get_property_bigint64;

	inline auto constexpr get_property_uint64 = JS::Converter::get_property_uint64;

	inline auto constexpr get_property_bool = JS::Converter::get_property_bool;
	
	/**
	 * ----------------------------------------
	 * JS Atom is used
	 * ----------------------------------------
	 */

	using Atom = JS::Atom;

	/**
	 * ----------------------------------------
	 * JS Method Declaration
	 * ----------------------------------------
	 */

	typedef JSValue (*JavaScriptNativeMethod)(JSContext *, JSValueConst, int, JSValueConst *);

	// JS Type

	namespace JSElement
	{

		using undefined = JSValue;

		using null = JSValue;

		using bigint = JSValue;

		using number = JSValue;

		using string = JSValue;

		using boolean = JSValue;

		template <typename T>
		using Array = JSValue;

		using Object = JSValue;

		using ArrayBuffer = JSValue;

		using Uint8Array = JSValue;

		using Prototype = JSValue;

		using any = JSValue;

		using ParameterList = JSValue *;

		using ParameterCount = int;

		using Context = JSContext;

	};

	namespace JSDefine
	{

		using DataStreamView = JSValue;

		using Boolean = JSValue;

		using Character = JSValue;

		using String = JSValue;

		using Size = JSValue;

		using BinaryView = JSValue;

		using ImageView = JSValue;

		using Canvas = JSValue;

		using JsonWriter = JSValue;

		using APNGMakerSetting = JSValue;
	}

	inline static auto throw_exception(
		JSContext *context, 
		const std::string& error, 
		const std::string& source, 
		std::string_view function_name
	) -> JSValue
	{
		auto evaluate_context = fmt::format("function {}() {{", function_name);
		evaluate_context += fmt::format("let e = new Error(`{}`);", error);
		evaluate_context += fmt::format("e.source = `{}`;", source);
		evaluate_context += "throw e;";
		evaluate_context += "}";
		evaluate_context += fmt::format("\n{}();", function_name);
		JS_Eval(context, evaluate_context.c_str(), evaluate_context.length(), source.c_str(), JS_EVAL_TYPE_GLOBAL);
		return JS_EXCEPTION;
	}

	inline static auto proxy_wrapper(
		JSContext *context, 
		std::string_view func_name, 
		std::function<JSValue ()> code
	) -> JSValue 
	{
		auto result = JS_UNDEFINED;
		try {
			result = code();
		} catch (...) {
			auto exception = parse_exception();
			if (exception.function_name.empty()) {
				exception.function_name = func_name;
			}
			result = throw_exception(context, exception.message(), exception.source, exception.function_name);
		}
		return result;  
	}

	inline static auto callback(
		JSContext *context,
		JSValueConst this_val,
		int argc,
		JSValueConst *argv
	) -> JSElement::undefined
	{
		return proxy_wrapper(context, "callback", [&]() {
			assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "callback");
			auto destination = std::make_unique<CStringView>();
			Shell::callback(construct_string_list(JS::Converter::get_vector<std::string>(context, argv[0])).get(), destination.get());
			return JS::Converter::to_string(context, construct_standard_string(*destination));
		});
	}

	namespace JSON
	{

		inline static auto json_to_js(
			JSContext *context,
			const nlohmann::ordered_json &json
		) -> JSElement::Object
		{
			switch (json.type())
			{
				case nlohmann::ordered_json::value_t::null:
				{
					return JS::Converter::get_null();
				}
				case nlohmann::ordered_json::value_t::object:
				{
					auto js_obj = JS_NewObject(context);
					for (auto &[key, value] : json.items())
					{
						auto atom = Atom{context, key.data(), key.size()};
						JS_DefinePropertyValue(context, js_obj, atom.value, json_to_js(context, value), JS_PROP_C_W_E);
					}
					return js_obj;
				}
				case nlohmann::ordered_json::value_t::array:
				{
					auto js_arr = JS_NewArray(context);
					for (auto i : Range<size_t>(json.size()))
					{
						JS_DefinePropertyValueUint32(context, js_arr, i, json_to_js(context, json[i]), JS_PROP_C_W_E);
					}
					return js_arr;
				}
				case nlohmann::ordered_json::value_t::string:
				{
					return JS_NewStringLen(context, json.get<std::string>().data(), json.get<std::string>().size());
				}
				case nlohmann::ordered_json::value_t::boolean:
				{
					return JS_NewBool(context, json.get<bool>());
				}
				case nlohmann::ordered_json::value_t::number_integer:
				{
					return JS_NewBigInt64(context, json.get<int64_t>());
				}
				case nlohmann::ordered_json::value_t::number_unsigned:
				{
					return JS_NewBigInt64(context, json.get<uint64_t>());
				}
				case nlohmann::ordered_json::value_t::number_float:
				{
					return JS_NewFloat64(context, json.get<double>());
				}
				default:
				{
					return JS_UNDEFINED;
				}
			}
		}

		inline static auto deserialize(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::Object {
			return proxy_wrapper(context, "deserialize", [&]() {
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "deserialize");
				auto source = JS::Converter::get_string(context, argv[0]);
				auto json = nlohmann::ordered_json::parse(source);
				auto js_obj = json_to_js(context, json);
				return js_obj;
			});
		}

		inline static auto deserialize_fs(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::Object {
			return proxy_wrapper(context, "deserialize_fs", [&]() {
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "deserialize_fs");
				auto source = JS::Converter::get_string(context, argv[0]);
				auto json = Sen::Kernel::FileSystem::read_json(source);
				auto js_obj = json_to_js(context, *json);
				return js_obj;
			});
		}

		inline static auto object_to_json(
			JSContext *context,
			JSValueConst value
		) -> nlohmann::ordered_json
		{
			switch (JS_VALUE_GET_TAG(value))
			{
			case JS_TAG_UNDEFINED:
			{
				return nullptr;
			}
			case JS_TAG_NULL:
			{
				return nullptr;
			}
			case JS_TAG_OBJECT:
			{
				if (JS_IsArray(context, value))
				{
					auto json = nlohmann::ordered_json::array();
					auto length = uint32_t{};
					auto atom = Atom{context, "length"};
					auto len = JS_GetProperty(context, value, atom.value);
					JS_ToUint32(context, &length, len);
					JS_FreeValue(context, len);
					for (auto i : Range<uint32_t>(length))
					{
						auto val = JS_GetPropertyUint32(context, value, i);
						json.push_back(object_to_json(context, val));
						JS_FreeValue(context, val);
					}
					return json;
				}
				else if (JS_IsObject(value))
				{
					auto json = nlohmann::ordered_json::object();
					auto *tab = static_cast<JSPropertyEnum *>(nullptr);
					auto tab_size = uint32_t{};
					if (JS_GetOwnPropertyNames(context, &tab, &tab_size, value, JS_GPN_STRING_MASK) == 0)
					{
						for (auto i : Range<uint32_t>(tab_size))
						{
							auto key = JS_AtomToCString(context, tab[i].atom);
							if (key == nullptr) {
								JS_FreeAtom(context, tab[i].atom);
								continue;
							}
							auto val = JS_GetProperty(context, value, tab[i].atom);
							if (JS_VALUE_GET_TAG(val) != JS_TAG_UNDEFINED)
							{
								json[key] = object_to_json(context, val);
							}
							JS_FreeAtom(context, tab[i].atom);
							JS_FreeValue(context, val);
							JS_FreeCString(context, key);
						}
						js_free(context, tab);
					}
					else {
						if (tab != nullptr) {
							js_free(context, tab);
						}
						throw Exception("Failed to get property names", std::source_location::current(), "object_to_json");
					}
					return json;
				}
				else
				{
					throw Exception("Unknown type", std::source_location::current(), "object_to_json");
				}
			}
			case JS_TAG_STRING:
			{
				auto str = JS::Converter::get_string(context, value);
				auto json = nlohmann::ordered_json(str);
				return json;
			}
			case JS_TAG_BOOL:
			{
				return nlohmann::ordered_json(JS_VALUE_GET_BOOL(value) == 0 ? false : true);
			}
			case JS_TAG_INT:
			{
				return nlohmann::ordered_json(static_cast<double>(JS_VALUE_GET_INT(value)));
			}
			case JS_TAG_FLOAT64:
			{
				return nlohmann::ordered_json(JS_VALUE_GET_FLOAT64(value));
			}
			case JS_TAG_BIG_INT:
			{
				auto val = int64_t{};
				JS_ToBigInt64(context, &val, value);
				return nlohmann::ordered_json(val);
			}
			}
		}

		inline static auto serialize(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::string {
			return proxy_wrapper(context, "serialize", [&]() {
				assert_conditional(argc == 3, fmt::format("{} 3, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "serialize");
				auto json = object_to_json(context, argv[0]);
				auto indent = JS::Converter::get_int32(context, argv[1]);
				auto ensure_ascii = JS::Converter::get_bool(context, argv[2]);
				auto source = json.dump(indent, '\t', ensure_ascii);
				return JS_NewStringLen(context, source.data(), source.size());
			});
		}

		inline static auto serialize_fs(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::undefined
		{
			return proxy_wrapper(context, "serialize_fs", [&]() {
				assert_conditional(argc == 4, fmt::format("{} 4, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "serialize_fs");
				auto destination = JS::Converter::get_string(context, argv[0]);
				auto json = object_to_json(context, argv[1]);
				auto indent = JS::Converter::get_int32(context, argv[2]);
				auto ensure_ascii = JS::Converter::get_bool(context, argv[3]);
				auto result = json.dump(indent, '\t', ensure_ascii);
				Sen::Kernel::FileSystem::write_file(destination, result);
				return JS_UNDEFINED;
			});
		}
	}

	/**
	 * Language Support
	 */

	namespace Language
	{

		inline static auto load_language(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::undefined
		{
			return proxy_wrapper(context, "load_language", [&]() {
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "load_language");
				assert_conditional(JS_IsString(argv[0]), fmt::format("{} {} {} {}", Kernel::Language::get("kernel.expected_argument"), 0, Kernel::Language::get("is"), Kernel::Language::get("kernel.tuple.js_string")), "load_language");
				Sen::Kernel::Language::read_language(JS::Converter::get_string(context, argv[0]));
				return JS_UNDEFINED;
			});
		}


		inline static auto get(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::string
		{
			return proxy_wrapper(context, "get", [&]() {
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "get");
				assert_conditional(JS_IsString(argv[0]), fmt::format("{} {} {} {}", Kernel::Language::get("kernel.expected_argument"), 0, Kernel::Language::get("is"), Kernel::Language::get("kernel.tuple.js_string")), "get");
				auto result = Sen::Kernel::Language::get(JS::Converter::get_string(context, argv[0]));
				return JS::Converter::to_string(context, result.data());
			});
		}


	}

	namespace Class
	{

	#define JS_CPPGETSET_MAGIC_DEF(c_name, fgetter, fsetter, _magic)                                                                                                                                      \
		{                                                                                                                                                                                                 \
			.name = c_name, .prop_flags = JS_PROP_CONFIGURABLE, .def_type = JS_DEF_CGETSET_MAGIC, .magic = _magic, .u = {.getset = {.get = {.getter_magic = fgetter}, .set = {.setter_magic = fsetter}} } \
		}

	#define JS_CPPFUNC_DEF(c_name, length, func1)                                                                                                                                       \
		{                                                                                                                                                                               \
			.name = c_name, .prop_flags = JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE, .def_type = JS_DEF_CFUNC, .magic = 0, .u = {.func = {length, JS_CFUNC_generic, {.generic = func1}} } \
		}

			using CString = const char*;

			template <typename T> requires (std::is_class<T>::value && !std::is_pointer<T>::value)
			inline auto make_finalizer (
				JSClassID class_id
			) -> std::function<void (JSRuntime*, JSValue)>
			{
				return [class_id](JSRuntime *rt, JSValue value) {
					auto object_class = static_cast<T*>(JS_GetOpaque(value, class_id));
					if (object_class != nullptr) {
						delete object_class;
					}
				};
			}

			template <typename T> requires (std::is_class<T>::value && !std::is_pointer<T>::value)
			inline auto constexpr make_class_definition (
				std::string_view class_name,
				JSClassID class_id
			) -> JSClassDef
			{
				return JSClassDef {
					.class_name = class_name.data(),
					.finalizer = make_finalizer<T>(class_id).template target<JSClassFinalizer>(),
					.gc_mark = nullptr,
					.call = nullptr,
					.exotic = nullptr,
				};
			}

			inline static auto make_instance_object (
				JSContext* ctx, 
				JSValue parent, 
				std::string_view name
			) -> JSValue
			{
				auto atom = Atom{ctx, name};
				auto obj = JS_GetProperty(ctx, parent, atom.value);
				if (JS_IsUndefined(obj)) {
					obj = JS_NewObject(ctx);
					JS_DefinePropertyValue(ctx, parent, atom.value, obj, int{JS_PROP_C_W_E});
				}
				return obj;
			}

			template <typename T> requires (std::is_class<T>::value && !std::is_pointer<T>::value)
			inline auto constexpr make_deleter() {
				return [](T* obj) {
					delete obj;
				};
			}



		namespace DataStreamView
		{

			inline static auto constexpr BooleanConstraint = true;

			template <auto T>
				requires BooleanConstraint
			using Data = Definition::Buffer::Stream<T>;

			template <auto use_big_endian>
				requires BooleanConstraint
			struct ClassID
			{
				static_assert(use_big_endian == true or use_big_endian == false, "use_big_endian must be true or false");
				static_assert(sizeof(use_big_endian) == sizeof(bool));
				inline static JSClassID value = 0;
			};

			template <auto T>
				requires BooleanConstraint
			inline static auto constructor(
				JSContext *ctx,
				JSValueConst new_target,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return proxy_wrapper(ctx, "constructor", [&]() {
					auto s = std::unique_ptr<Data<T>, decltype(make_deleter<Data<T>>())>(nullptr, make_deleter<Data<T>>());
					auto obj = JS_UNDEFINED;
					auto proto = JSElement::Prototype{};
					if (argc == 1) {
						s.reset(new Data<T>{ JS::Converter::get_string(ctx, argv[0])});
					} else if (argc == 0) {
						s.reset(new Data<T>{});
					} else {
						JS_ThrowInternalError(ctx, fmt::format("{}", Kernel::Language::get("data_stream_view_cannot_be_initialize")).data());
						return JS_EXCEPTION;
					}
					auto atom = Atom{ctx, "prototype"};
					proto = JS_GetProperty(ctx, new_target, atom.value);
					if (JS_IsException(proto)) {
						goto fail;
					}
					obj = JS_NewObjectProtoClass(ctx, proto, ClassID<T>::value);
					JS_FreeValue(ctx, proto);
					if (JS_IsException(obj)) {
						goto fail;
					}
					JS_SetOpaque(obj, s.release());
					return obj;

				fail:
					JS_FreeValue(ctx, obj);
					if constexpr (T) {
						JS_ThrowInternalError(ctx, "Failed to initialize DataStreamViewUseBigEndian");
					} else {
						JS_ThrowInternalError(ctx, "Failed to initialize DataStreamView");
					}
					return JS_EXCEPTION;
				});
			}

			template <auto T> requires BooleanConstraint
			inline static auto constexpr _class_name (

			) -> std::string_view
			{
				if constexpr (T) {
					return "DataStreamView";
				}
				else {
					return "DataStreamViewUseBigEndian";
				}
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto this_class = make_class_definition<Data<T>>(
				_class_name<T>(),
				ClassID<T>::value
			);

			template <auto T>
			requires BooleanConstraint
			inline static auto handle(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv,
				std::string_view method_name,
				std::function<JSValue(Data<T>*)> method
			) -> JSValue {
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return proxy_wrapper(ctx, method_name, [&](){
					auto s = static_cast<Data<T>*>(JS_GetOpaque2(ctx, this_val, ClassID<T>::value));
					if (s == nullptr) {
						return JS_EXCEPTION;
					}
					return method(s);
				});
			}

			template <auto T>
			requires BooleanConstraint
			inline static auto getter(
				JSContext *ctx,
				JSValueConst this_val,
				int magic
			) -> JSElement::bigint 
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(ctx, this_val, 0, nullptr, "getter", [&](Data<T>* s) {
					if (magic == 0) {
						return JS_NewBigInt64(ctx, s->read_pos);
					} else {
						return JS_NewBigInt64(ctx, s->write_pos);
					}
				});
			}

			template <auto T>
			requires BooleanConstraint
			inline static auto setter(
				JSContext *ctx,
				JSValueConst this_val,
				JSValueConst val,
				int magic
			) -> JSElement::bigint 
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(ctx, this_val, 0, nullptr, "setter", [&](Data<T>* s) {
					auto v = std::int64_t{};
					if (JS_ToBigInt64(ctx, &v, val)) {
						return JS_EXCEPTION;
					}
					if (magic == 0) {
						s->read_pos = v;
					} else {
						s->write_pos = v;
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
			requires BooleanConstraint
			inline static auto size(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::bigint {
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(ctx, this_val, argc, argv, "size", [&](Data<T>* s) {
					return JS::Converter::to_bigint<uint64_t>(ctx, s->size());
				});
			}

			template <auto T>
			requires BooleanConstraint
			inline static auto capacity(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::bigint {
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(ctx, this_val, argc, argv, "capacity", [&](Data<T>* s) {
					return JS::Converter::to_bigint<uint64_t>(ctx, s->capacity());
				});
			}

			template <auto T>
			requires BooleanConstraint
			inline static auto fromString(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined {
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(ctx, this_val, argc, argv, "fromString", [&](Data<T>* s) {
					assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "fromString");
					s->fromString(JS::Converter::get_string(ctx, argv[0]));
					return JS_UNDEFINED;
				});
			}

			template <auto T>
			requires BooleanConstraint
			inline static auto reserve(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined {
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(ctx, this_val, argc, argv, "reserve", [&](Data<T>* s) {
					assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "reserve");
					s->reserve(static_cast<std::uint64_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					return JS_UNDEFINED;
				});
			}

			#pragma region convert

			inline static auto to_arraybuffer(
				JSContext *ctx,
				const List<uint8_t>& vec
			) -> JSElement::ArrayBuffer {
				return JS_NewArrayBufferCopy(ctx, vec.data(), vec.size());
			}

			inline static auto to_uint8array(
				JSContext *ctx,
				const List<uint8_t>& vec
			) -> JSElement::Uint8Array {
				auto array_buffer = JS_NewArrayBufferCopy(ctx, vec.data(), vec.size());
				auto global_obj = JS_GetGlobalObject(ctx);
				auto atom = Atom{ctx, "Uint8Array"};
				auto uint8array_ctor = JS_GetProperty(ctx, global_obj, atom.value);
				JSValue args[] = {array_buffer};
				auto uint8array = JS_CallConstructor(ctx, uint8array_ctor, 1, args);
				JS_FreeValue(ctx, global_obj);
				JS_FreeValue(ctx, uint8array_ctor);
				return uint8array;
			}

			#pragma endregion

			template <auto T>
			requires BooleanConstraint
			inline static auto toUint8Array(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::Uint8Array {
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(ctx, this_val, argc, argv, "toUint8Array", [&](Data<T>* s) {
					return to_uint8array(ctx, s->toBytes());
				});
			}

			template <auto T>
			requires BooleanConstraint
			inline static auto toArrayBuffer(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::ArrayBuffer {
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(ctx, this_val, argc, argv, "toArrayBuffer", [&](Data<T>* s) {
					return to_arraybuffer(ctx, s->toBytes());
				});
			}

			template <auto T>
			requires BooleanConstraint
			inline static auto getArrayBuffer(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::ArrayBuffer {
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(ctx, this_val, argc, argv, "getArrayBuffer", [&](Data<T>* s) {
					assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "getArrayBuffer");
					auto from = JS::Converter::get_bigint64(ctx, argv[0]);
					auto to = JS::Converter::get_bigint64(ctx, argv[1]);
					return to_arraybuffer(ctx, s->get(from, to));
				});
			}

			template <auto T>
			requires BooleanConstraint
			inline static auto getUint8Array(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::Uint8Array 
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(ctx, this_val, argc, argv, "getUint8Array", [&](Data<T>* s) {
					assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "getUint8Array");
					auto from = JS::Converter::get_bigint64(ctx, argv[0]);
					auto to = JS::Converter::get_bigint64(ctx, argv[1]);
					return to_uint8array(ctx, s->get(from, to));
				});
			}

			template <auto T>
			requires BooleanConstraint
			inline static auto toString(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::string 
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(ctx, this_val, argc, argv, "toString", [&](Data<T>* s) {
					return JS::Converter::to_string(ctx, s->toString());
				});
			}

			template <auto T>
			requires BooleanConstraint
			inline static auto out_file(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined {
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(ctx, this_val, argc, argv, "out_file", [&](Data<T>* s) {
					assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "out_file");
					s->out_file(JS::Converter::get_string(ctx, argv[0]));
					return JS_UNDEFINED;
				});
			}

			template <auto T>
			requires BooleanConstraint
			inline static auto allocate(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined {
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(ctx, this_val, argc, argv, "allocate", [&](Data<T>* s) {
					assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "allocate");
					s->allocate(static_cast<std::size_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeUint8(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(ctx, this_val, argc, argv, "writeUint8", [&](Data<T>* s) {
					assert_conditional(argc == 2 || argc == 1, fmt::format("argument expected 2 or 1, received: {}", argc), "writeUint8");
					if (argc == 1) {
						s->writeUint8(static_cast<uint8_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					}
					else {
						s->writeUint8(static_cast<uint8_t>(JS::Converter::get_bigint64(ctx, argv[0])),static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeUint16(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(ctx, this_val, argc, argv, "writeUint16", [&](Data<T>* s) {
					assert_conditional(argc == 2 || argc == 1, fmt::format("argument expected 2 or 1, received: {}", argc), "writeUint16");
					if (argc == 1) {
						s->writeUint16(static_cast<uint16_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					}
					else {
						s->writeUint16(static_cast<uint16_t>(JS::Converter::get_bigint64(ctx, argv[0])),static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeUint24(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(ctx, this_val, argc, argv, "writeUint24", [&](Data<T>* s) {
					assert_conditional(argc == 2 || argc == 1, fmt::format("argument expected 2 or 1, received: {}", argc), "writeUint24");
					if (argc == 1) {
						s->writeUint24(static_cast<uint32_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					}
					else {
						s->writeUint24(static_cast<uint32_t>(JS::Converter::get_bigint64(ctx, argv[0])),static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeUint32(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(ctx, this_val, argc, argv, "writeUint32", [&](Data<T>* s) {
					assert_conditional(argc == 2 || argc == 1, fmt::format("argument expected 2 or 1, received: {}", argc), "writeUint32");
					if (argc == 1) {
						s->writeUint32(static_cast<uint32_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					}
					else {
						s->writeUint32(static_cast<uint32_t>(JS::Converter::get_bigint64(ctx, argv[0])),static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeUint64(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(ctx, this_val, argc, argv, "writeUint64", [&](Data<T>* s) {
					assert_conditional(argc == 2 || argc == 1, fmt::format("argument expected 2 or 1, received: {}", argc), "writeUint64");
					if (argc == 1) {
						s->writeUint64(static_cast<uint64_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					}
					else {
						s->writeUint64(static_cast<uint64_t>(JS::Converter::get_bigint64(ctx, argv[0])),static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeInt8(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(ctx, this_val, argc, argv, "writeInt8", [&](Data<T>* s) {
					assert_conditional(argc == 2 || argc == 1, fmt::format("argument expected 2 or 1, received: {}", argc), "writeInt8");
					if (argc == 1) {
						s->writeInt8(static_cast<int8_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					}
					else {
						s->writeInt8(static_cast<int8_t>(JS::Converter::get_bigint64(ctx, argv[0])),static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeInt16(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(ctx, this_val, argc, argv, "writeInt16", [&](Data<T>* s) {
					assert_conditional(argc == 2 || argc == 1, fmt::format("argument expected 2 or 1, received: {}", argc), "writeInt16");
					if (argc == 1) {
						s->writeInt16(static_cast<int16_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					}
					else {
						s->writeInt16(static_cast<int16_t>(JS::Converter::get_bigint64(ctx, argv[0])),static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeInt24(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(ctx, this_val, argc, argv, "writeInt24", [&](Data<T>* s) {
					assert_conditional(argc == 2 || argc == 1, fmt::format("argument expected 2 or 1, received: {}", argc), "writeInt24");
					if (argc == 1) {
						s->writeInt24(static_cast<int32_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					}
					else {
						s->writeInt24(static_cast<int32_t>(JS::Converter::get_bigint64(ctx, argv[0])),static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeInt32(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(ctx, this_val, argc, argv, "writeInt32", [&](Data<T>* s) {
					assert_conditional(argc == 2 || argc == 1, fmt::format("argument expected 2 or 1, received: {}", argc), "writeInt32");
					if (argc == 1) {
						s->writeInt32(static_cast<int32_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					}
					else {
						s->writeInt32(static_cast<int32_t>(JS::Converter::get_bigint64(ctx, argv[0])),static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeInt64(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(ctx, this_val, argc, argv, "writeInt64", [&](Data<T>* s) {
					assert_conditional(argc == 2 || argc == 1, fmt::format("argument expected 2 or 1, received: {}", argc), "writeInt64");
					if (argc == 1) {
						s->writeInt64(static_cast<int64_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					}
					else {
						s->writeInt64(static_cast<int64_t>(JS::Converter::get_bigint64(ctx, argv[0])),static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			#pragma region convert

			inline static auto from_arraybuffer(
				JSContext *ctx,
				JSValue array_buffer
			) -> List<uint8_t>
			{
				auto size = size_t{};
				auto data = JS_GetArrayBuffer(ctx, &size, array_buffer);
				assert_conditional(size != 0 && data != nullptr, fmt::format("{}", Kernel::Language::get("js.array_buffer_is_empty")), "from_arraybuffer");
				return make_list(data, size);
			}

			inline static auto from_uint8array (
				JSContext *ctx,
				JSValue uint8array
			) -> List<uint8_t>
			{
				auto atom = Atom{ctx, "buffer"};
				auto array_buffer = JS_GetProperty(ctx, uint8array, atom.value);
				auto vec = from_arraybuffer(ctx, array_buffer);
				JS_FreeValue(ctx, array_buffer);
				return vec;
			}

			#pragma endregion

			template <auto T>
				requires BooleanConstraint
			inline static auto writeArrayBuffer(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(ctx, this_val, argc, argv, "writeArrayBuffer", [&](Data<T>* s) {
					assert_conditional(argc == 1 || argc == 2, fmt::format("argument expected 1 or 2, received: {}", argc), "writeArrayBuffer");
					if (argc == 2) {
						s->writeBytes(from_arraybuffer(ctx, argv[0]), static_cast<std::size_t>(JS::Converter::get_bigint64(ctx, argv[1])));
					}
					else {
						s->writeBytes(from_arraybuffer(ctx, argv[0]));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeUint8Array(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(ctx, this_val, argc, argv, "writeUint8Array", [&](Data<T>* s) {
					assert_conditional(argc == 1 || argc == 2, fmt::format("argument expected 1 or 2, received: {}", argc), "writeUint8Array");
					if (argc == 2) {
						s->writeBytes(from_uint8array(ctx, argv[0]), static_cast<std::size_t>(JS::Converter::get_bigint64(ctx, argv[1])));
					}
					else {
						s->writeBytes(from_uint8array(ctx, argv[0]));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeFloat(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(ctx, this_val, argc, argv, "writeFloat", [&](Data<T>* s) {
					assert_conditional(argc == 1 || argc == 2, fmt::format("argument expected 1 or 2, received: {}", argc), "writeFloat");
					if (argc == 1) {
						s->writeFloat(static_cast<float>(JS::Converter::get_float32(ctx, argv[0])));
					}
					else {
						s->writeFloat(static_cast<float>(JS::Converter::get_float32(ctx, argv[0])),static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeDouble(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(ctx, this_val, argc, argv, "writeDouble", [&](Data<T>* s) {
					assert_conditional(argc == 1 || argc == 2, fmt::format("argument expected 1 or 2, received: {}", argc), "writeDouble");
					if (argc == 1) {
						s->writeDouble(static_cast<double>(JS::Converter::get_float64(ctx, argv[0])));
					}
					else {
						s->writeDouble(static_cast<double>(JS::Converter::get_float64(ctx, argv[0])),static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeVarInt32(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(ctx, this_val, argc, argv, "writeVarInt32", [&](Data<T>* s) {
					assert_conditional(argc == 1 || argc == 2, fmt::format("argument expected 1 or 2, received: {}", argc), "writeVarInt32");
					if (argc == 1) {
						s->writeVarInt32(static_cast<int32_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					}
					else {
						s->writeVarInt32(static_cast<int32_t>(JS::Converter::get_bigint64(ctx, argv[0])),static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeVarInt64(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(ctx, this_val, argc, argv, "writeVarInt64", [&](Data<T>* s) {
					assert_conditional(argc == 1 || argc == 2, fmt::format("argument expected 1 or 2, received: {}", argc), "writeVarInt64");
					if (argc == 1) {
						s->writeVarInt64(static_cast<int64_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					}
					else {
						s->writeVarInt64(static_cast<int64_t>(JS::Converter::get_bigint64(ctx, argv[0])),static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeZigZag32(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(ctx, this_val, argc, argv, "writeZigZag32", [&](Data<T>* s) {
					assert_conditional(argc == 1 || argc == 2, fmt::format("argument expected 1 or 2, received: {}", argc), "writeZigZag32");
					if (argc == 1) {
						s->writeZigZag32(static_cast<int32_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					}
					else {
						s->writeZigZag32(static_cast<int32_t>(JS::Converter::get_bigint64(ctx, argv[0])),static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeZigZag64(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(ctx, this_val, argc, argv, "writeZigZag64", [&](Data<T>* s) {
					assert_conditional(argc == 1 || argc == 2, fmt::format("argument expected 1 or 2, received: {}", argc), "writeZigZag64");
					if (argc == 1) {
						s->writeZigZag64(static_cast<int64_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					}
					else {
						s->writeZigZag64(static_cast<int64_t>(JS::Converter::get_bigint64(ctx, argv[0])),static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeString(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(ctx, this_val, argc, argv, "writeString", [&](Data<T>* s) {
					assert_conditional(argc == 1 || argc == 2, fmt::format("argument expected 1 or 2, received: {}", argc), "writeString");
					if (argc == 1) {
						s->writeString(JS::Converter::get_string(ctx, argv[0]));
					}
					else {
						s->writeString(JS::Converter::get_string(ctx, argv[0]),static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeStringFourByte(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(ctx, this_val, argc, argv, "writeStringFourByte", [&](Data<T>* s) {
					assert_conditional(argc == 2 || argc == 1, fmt::format("argument expected 2 or 1, received: {}", argc), "writeStringFourByte");
					if (argc == 1) {
						s->writeStringFourByte(JS::Converter::get_string(ctx, argv[0]));
					}
					else {
						s->writeStringFourByte(JS::Converter::get_string(ctx, argv[0]),static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeNull(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(ctx, this_val, argc, argv, "writeNull", [&](Data<T>* s) {
					assert_conditional(argc == 2 || argc == 1, fmt::format("argument expected 2 or 1, received: {}", argc), "writeNull");
					if (argc == 1) {
						s->writeNull(static_cast<uint64_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					}
					else {
						s->writeNull(static_cast<uint64_t>(JS::Converter::get_bigint64(ctx, argv[0])),static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeBoolean(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(ctx, this_val, argc, argv, "writeBoolean", [&](Data<T>* s) {
					assert_conditional(argc == 2 || argc == 1, fmt::format("argument expected 2 or 1, received: {}", argc), "writeBoolean");
					if (argc == 1) {
						s->writeBoolean(JS::Converter::get_bool(ctx, argv[0]));
					}
					else {
						s->writeBoolean(JS::Converter::get_bool(ctx, argv[0]),static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeStringByUint8(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(ctx, this_val, argc, argv, "writeStringByUint8", [&](Data<T>* s) {
					assert_conditional(argc == 2 || argc == 1, fmt::format("argument expected 2 or 1, received: {}", argc), "writeStringByUint8");
					if (argc == 1) {
						s->writeStringByUint8(JS::Converter::get_string(ctx, argv[0]));
					}
					else {
						s->writeStringByUint8(JS::Converter::get_string(ctx, argv[0]),static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeStringByUint16(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(ctx, this_val, argc, argv, "writeStringByUint16", [&](Data<T>* s) {
					assert_conditional(argc == 2 || argc == 1, fmt::format("argument expected 2 or 1, received: {}", argc), "writeStringByUint16");
					if (argc == 1) {
						s->writeStringByUint16(JS::Converter::get_string(ctx, argv[0]));
					}
					else {
						s->writeStringByUint16(JS::Converter::get_string(ctx, argv[0]),static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeStringByUint32(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(ctx, this_val, argc, argv, "writeStringByUint32", [&](Data<T>* s) {
					assert_conditional(argc == 2 || argc == 1, fmt::format("argument expected 2 or 1, received: {}", argc), "writeStringByUint32");
					if (argc == 1) {
						s->writeStringByUint32(JS::Converter::get_string(ctx, argv[0]));
					}
					else {
						s->writeStringByUint32(JS::Converter::get_string(ctx, argv[0]),static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeStringByInt8(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(ctx, this_val, argc, argv, "writeStringByInt8", [&](Data<T>* s) {
					assert_conditional(argc == 2 || argc == 1, fmt::format("argument expected 2 or 1, received: {}", argc), "writeStringByInt8");
					if (argc == 1) {
						s->writeStringByInt8(JS::Converter::get_string(ctx, argv[0]));
					}
					else {
						s->writeStringByInt8(JS::Converter::get_string(ctx, argv[0]),static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeStringByInt16(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(ctx, this_val, argc, argv, "writeStringByInt16", [&](Data<T>* s) {
					assert_conditional(argc == 2 || argc == 1, fmt::format("argument expected 2 or 1, received: {}", argc), "writeStringByInt16");
					if (argc == 1) {
						s->writeStringByInt16(JS::Converter::get_string(ctx, argv[0]));
					}
					else {
						s->writeStringByInt16(JS::Converter::get_string(ctx, argv[0]),static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeStringByInt32(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(ctx, this_val, argc, argv, "writeStringByInt32", [&](Data<T>* s) {
					assert_conditional(argc == 2 || argc == 1, fmt::format("argument expected 2 or 1, received: {}", argc), "writeStringByInt32");
					if (argc == 1) {
						s->writeStringByInt32(JS::Converter::get_string(ctx, argv[0]));
					}
					else {
						s->writeStringByInt32(JS::Converter::get_string(ctx, argv[0]),static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto writeStringByEmpty(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool));
				return handle<T>(ctx, this_val, argc, argv, "writeStringByEmpty", [&](Data<T>* s) {
					assert_conditional(argc == 2 || argc == 1, fmt::format("argument expected 2 or 1, received: {}", argc), "writeStringByEmpty");
					if (argc == 1) {
						s->writeStringByEmpty(JS::Converter::get_string(ctx, argv[0]));
					}
					else {
						s->writeStringByEmpty(JS::Converter::get_string(ctx, argv[0]),static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[1])));
					}
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readUint8(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::bigint
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(ctx, this_val, argc, argv, "readUint8", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readUint8");
					auto value = uint8_t{};
					if (argc == 1) {
						value = s->readUint8(static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					}
					else {
						value = s->readUint8();
					}
					return JS::Converter::to_bigint<uint8_t>(ctx, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readUint16(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::bigint
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(ctx, this_val, argc, argv, "readUint16", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readUint16");
					auto value = uint16_t{};
					if (argc == 1) {
						value = s->readUint16(static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					}
					else {
						value = s->readUint16();
					}
					return JS::Converter::to_bigint<uint16_t>(ctx, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readUint24(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::bigint
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(ctx, this_val, argc, argv, "readUint24", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readUint24");
					auto value = uint32_t{};
					if (argc == 1) {
						value = s->readUint24(static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					}
					else {
						value = s->readUint24();
					}
					return JS::Converter::to_bigint<uint32_t>(ctx, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readUint32(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::bigint
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(ctx, this_val, argc, argv, "readUint32", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readUint32");
					auto value = uint32_t{};
					if (argc == 1) {
						value = s->readUint32(static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					}
					else {
						value = s->readUint32();
					}
					return JS::Converter::to_bigint<uint32_t>(ctx, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readUint64(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::bigint
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(ctx, this_val, argc, argv, "readUint64", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readUint64");
					auto value = uint64_t{};
					if (argc == 1) {
						value = s->readUint64(static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					}
					else {
						value = s->readUint64();
					}
					return JS::Converter::to_bigint<uint64_t>(ctx, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readInt8(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::bigint
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(ctx, this_val, argc, argv, "readInt8", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readInt8");
					auto value = int8_t{};
					if (argc == 1) {
						value = s->readInt8(static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					}
					else {
						value = s->readInt8();
					}
					return JS::Converter::to_bigint<int8_t>(ctx, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readInt16(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::bigint
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(ctx, this_val, argc, argv, "readInt16", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readInt16");
					auto value = int16_t{};
					if (argc == 1) {
						value = s->readInt16(static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					}
					else {
						value = s->readInt16();
					}
					return JS::Converter::to_bigint<int16_t>(ctx, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readInt24(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::bigint
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(ctx, this_val, argc, argv, "readInt24", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readInt24");
					auto value = int32_t{};
					if (argc == 1) {
						value = s->readInt24(static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					}
					else {
						value = s->readInt24();
					}
					return JS::Converter::to_bigint<int32_t>(ctx, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readInt32(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::bigint
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(ctx, this_val, argc, argv, "readInt32", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readInt32");
					auto value = int32_t{};
					if (argc == 1) {
						value = s->readInt32(static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					}
					else {
						value = s->readInt32();
					}
					return JS::Converter::to_bigint<int32_t>(ctx, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readInt64(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::bigint
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(ctx, this_val, argc, argv, "readInt64", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readInt64");
					auto value = int64_t{};
					if (argc == 1) {
						value = s->readInt64(static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					}
					else {
						value = s->readInt64();
					}
					return JS::Converter::to_bigint<int64_t>(ctx, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readString(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::string
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(ctx, this_val, argc, argv, "readString", [&](Data<T>* s) {
					assert_conditional(argc == 1 || argc == 2, fmt::format("argument expected 1 or 2, received: {}", argc), "readString");
					auto value = std::string{};
					if (argc == 1) {
						value = s->readString(static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					}
					else {
						value = s->readString(static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[0])),static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[1])));
					}
					return JS::Converter::to_string(ctx, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readStringByUint8(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::string
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(ctx, this_val, argc, argv, "readStringByUint8", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readStringByUint8");
					auto value = std::string{};
					if (argc == 1) {
						value = s->readStringByUint8(static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					}
					else {
						value = s->readStringByUint8();
					}
					return JS::Converter::to_string(ctx, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readStringByUint16(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::string
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(ctx, this_val, argc, argv, "readStringByUint16", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readStringByUint16");
					auto value = std::string{};
					if (argc == 1) {
						value = s->readStringByUint16(static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					}
					else {
						value = s->readStringByUint16();
					}
					return JS::Converter::to_string(ctx, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readStringByUint32(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::string
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(ctx, this_val, argc, argv, "readStringByUint32", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readStringByUint32");
					auto value = std::string{};
					if (argc == 1) {
						value = s->readStringByUint32(static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					}
					else {
						value = s->readStringByUint32();
					}
					return JS::Converter::to_string(ctx, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readStringByInt8(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::string
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(ctx, this_val, argc, argv, "readStringByInt8", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readStringByInt8");
					auto value = std::string{};
					if (argc == 1) {
						value = s->readStringByInt8(static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					}
					else {
						value = s->readStringByInt8();
					}
					return JS::Converter::to_string(ctx, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readStringByInt16(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::string
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(ctx, this_val, argc, argv, "readStringByInt16", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readStringByInt16");
					auto value = std::string{};
					if (argc == 1) {
						value = s->readStringByInt16(static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					}
					else {
						value = s->readStringByInt16();
					}
					return JS::Converter::to_string(ctx, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readStringByInt32(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::string
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(ctx, this_val, argc, argv, "readStringByInt32", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readStringByInt32");
					auto value = std::string{};
					if (argc == 1) {
						value = s->readStringByInt32(static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					}
					else {
						value = s->readStringByInt32();
					}
					return JS::Converter::to_string(ctx, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readStringByVarInt32(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::string
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(ctx, this_val, argc, argv, "readStringByVarInt32", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readStringByVarInt32");
					auto value = std::string{};
					if (argc == 1) {
						value = s->readStringByVarInt32(static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					}
					else {
						value = s->readStringByVarInt32();
					}
					return JS::Converter::to_string(ctx, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readStringByEmpty(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::string
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(ctx, this_val, argc, argv, "readStringByEmpty", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readStringByEmpty");
					auto value = std::string{};
					if (argc == 1) {
						value = s->readStringByEmpty(static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					}
					else {
						value = s->readStringByEmpty();
					}
					return JS::Converter::to_string(ctx, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readVarInt32(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::bigint
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(ctx, this_val, argc, argv, "readVarInt32", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readVarInt32");
					auto value = int32_t{};
					if (argc == 1) {
						value = s->readVarInt32(static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					}
					else {
						value = s->readVarInt32();
					}
					return JS::Converter::to_bigint<int32_t>(ctx, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readVarInt64(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::bigint
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(ctx, this_val, argc, argv, "readVarInt64", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readVarInt64");
					auto value = int64_t{};
					if (argc == 1) {
						value = s->readVarInt64(static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					}
					else {
						value = s->readVarInt64();
					}
					return JS::Converter::to_bigint<int64_t>(ctx, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readVarUint32(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::bigint
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(ctx, this_val, argc, argv, "readVarUint32", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readVarUint32");
					auto value = uint32_t{};
					if (argc == 1) {
						value = s->readVarUint32(static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					}
					else {
						value = s->readVarUint32();
					}
					return JS::Converter::to_bigint<uint32_t>(ctx, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readVarUint64(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::bigint
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(ctx, this_val, argc, argv, "readVarUint64", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readVarUint64");
					auto value = uint64_t{};
					if (argc == 1) {
						value = s->readVarUint64(static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					}
					else {
						value = s->readVarUint64();
					}
					return JS::Converter::to_bigint<uint64_t>(ctx, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readZigZag32(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::bigint
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(ctx, this_val, argc, argv, "readZigZag32", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readZigZag32");
					auto value = int32_t{};
					if (argc == 1) {
						value = s->readZigZag32(static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					}
					else {
						value = s->readZigZag32();
					}
					return JS::Converter::to_bigint<int32_t>(ctx, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readZigZag64(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::bigint
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(ctx, this_val, argc, argv, "readZigZag64", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readZigZag64");
					auto value = int64_t{};
					if (argc == 1) {
						value = s->readZigZag64(static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					}
					else {
						value = s->readZigZag64();
					}
					return JS::Converter::to_bigint<int64_t>(ctx, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readFloat(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::number
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(ctx, this_val, argc, argv, "readFloat", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readFloat");
					auto value = float{};
					if (argc == 1) {
						value = s->readFloat(static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					}
					else {
						value = s->readFloat();
					}
					return JS::Converter::to_number(ctx, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto readDouble(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::number
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(ctx, this_val, argc, argv, "readDouble", [&](Data<T>* s) {
					assert_conditional(argc == 0 || argc == 1, fmt::format("argument expected 0 or 1, received: {}", argc), "readDouble");
					auto value = double{};
					if (argc == 1) {
						value = s->readDouble(static_cast<size_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					}
					else {
						value = s->readDouble();
					}
					return JS::Converter::to_number(ctx, value);
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto close(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(bool), "T must be bool");
				return handle<T>(ctx, this_val, argc, argv, "readDouble", [&](Data<T>* s) {
					s->close();
					return JS_UNDEFINED;
				});
			}

			template <auto T>
				requires BooleanConstraint
			inline static auto constexpr proto_functions = std::to_array<JSCFunctionListEntry>({
				JS_CPPGETSET_MAGIC_DEF("read_position", getter<T>, setter<T>, 0),
				JS_CPPGETSET_MAGIC_DEF("write_position", getter<T>, setter<T>, 1),
				JS_CPPFUNC_DEF("size", 0, size<T>),
				JS_CPPFUNC_DEF("fromString", 1, fromString<T>),
				JS_CPPFUNC_DEF("capacity", 0, capacity<T>),
				JS_CPPFUNC_DEF("reserve", 1, reserve<T>),
				JS_CPPFUNC_DEF("toArrayBuffer", 0, toArrayBuffer<T>),
				JS_CPPFUNC_DEF("toUint8Array", 0, toUint8Array<T>),
				JS_CPPFUNC_DEF("getArrayBuffer", 2, getArrayBuffer<T>),
				JS_CPPFUNC_DEF("getUint8Array", 2, getUint8Array<T>),
				JS_CPPFUNC_DEF("toString", 0, toString<T>),
				JS_CPPFUNC_DEF("out_file", 1, out_file<T>),
				JS_CPPFUNC_DEF("writeUint8", 2, writeUint8<T>),
				JS_CPPFUNC_DEF("writeUint16", 2, writeUint16<T>),
				JS_CPPFUNC_DEF("writeUint24", 2, writeUint24<T>),
				JS_CPPFUNC_DEF("writeUint32", 2, writeUint32<T>),
				JS_CPPFUNC_DEF("writeUint64", 2, writeUint64<T>),
				JS_CPPFUNC_DEF("writeInt8", 2, writeInt8<T>),
				JS_CPPFUNC_DEF("writeInt16", 2, writeInt16<T>),
				JS_CPPFUNC_DEF("writeInt24", 2, writeInt24<T>),
				JS_CPPFUNC_DEF("writeInt32", 2, writeInt32<T>),
				JS_CPPFUNC_DEF("writeInt64", 2, writeInt64<T>),
				JS_CPPFUNC_DEF("writeArrayBuffer", 2, writeArrayBuffer<T>),
				JS_CPPFUNC_DEF("writeUint8Array", 2, writeUint8Array<T>),
				JS_CPPFUNC_DEF("writeFloat", 2, writeFloat<T>),
				JS_CPPFUNC_DEF("writeDouble", 2, writeDouble<T>),
				JS_CPPFUNC_DEF("writeVarInt32", 2, writeVarInt32<T>),
				JS_CPPFUNC_DEF("writeVarInt64", 2, writeVarInt64<T>),
				JS_CPPFUNC_DEF("writeZigZag32", 2, writeZigZag32<T>),
				JS_CPPFUNC_DEF("writeZigZag64", 2, writeZigZag64<T>),
				JS_CPPFUNC_DEF("writeString", 2, writeString<T>),
				JS_CPPFUNC_DEF("writeStringFourByte", 2, writeStringFourByte<T>),
				JS_CPPFUNC_DEF("writeNull", 2, writeNull<T>),
				JS_CPPFUNC_DEF("writeBoolean", 2, writeBoolean<T>),
				JS_CPPFUNC_DEF("writeStringByUint8", 2, writeStringByUint8<T>),
				JS_CPPFUNC_DEF("writeStringByUint16", 2, writeStringByUint16<T>),
				JS_CPPFUNC_DEF("writeStringByUint32", 2, writeStringByUint32<T>),
				JS_CPPFUNC_DEF("writeStringByInt8", 2, writeStringByInt8<T>),
				JS_CPPFUNC_DEF("writeStringByInt16", 2, writeStringByInt16<T>),
				JS_CPPFUNC_DEF("writeStringByInt32", 2, writeStringByInt32<T>),
				JS_CPPFUNC_DEF("writeStringByEmpty", 2, writeStringByEmpty<T>),
				JS_CPPFUNC_DEF("readUint8", 1, readUint8<T>),
				JS_CPPFUNC_DEF("readUint16", 1, readUint16<T>),
				JS_CPPFUNC_DEF("readUint24", 1, readUint24<T>),
				JS_CPPFUNC_DEF("readUint32", 1, readUint32<T>),
				JS_CPPFUNC_DEF("readUint64", 1, readUint64<T>),
				JS_CPPFUNC_DEF("readInt8", 1, readInt8<T>),
				JS_CPPFUNC_DEF("readInt16", 1, readInt16<T>),
				JS_CPPFUNC_DEF("readInt24", 1, readInt24<T>),
				JS_CPPFUNC_DEF("readInt32", 1, readInt32<T>),
				JS_CPPFUNC_DEF("readInt64", 1, readInt64<T>),
				JS_CPPFUNC_DEF("readString", 2, readString<T>),
				JS_CPPFUNC_DEF("readStringByUint8", 1, readStringByUint8<T>),
				JS_CPPFUNC_DEF("readStringByUint16", 1, readStringByUint16<T>),
				JS_CPPFUNC_DEF("readStringByUint32", 1, readStringByUint32<T>),
				JS_CPPFUNC_DEF("readStringByInt8", 1, readStringByInt8<T>),
				JS_CPPFUNC_DEF("readStringByInt16", 1, readStringByInt16<T>),
				JS_CPPFUNC_DEF("readStringByInt32", 1, readStringByInt32<T>),
				JS_CPPFUNC_DEF("readStringByVarInt32", 1, readStringByVarInt32<T>),
				JS_CPPFUNC_DEF("readStringByEmpty", 1, readStringByEmpty<T>),
				JS_CPPFUNC_DEF("readVarInt32", 1, readVarInt32<T>),
				JS_CPPFUNC_DEF("readVarInt64", 1, readVarInt64<T>),
				JS_CPPFUNC_DEF("readVarUint32", 1, readVarUint32<T>),
				JS_CPPFUNC_DEF("readVarUint64", 1, readVarUint64<T>),
				JS_CPPFUNC_DEF("readZigZag32", 1, readZigZag32<T>),
				JS_CPPFUNC_DEF("readZigZag64", 1, readZigZag64<T>),
				JS_CPPFUNC_DEF("readFloat", 1, readFloat<T>),
				JS_CPPFUNC_DEF("readDouble", 1, readDouble<T>),
				JS_CPPFUNC_DEF("close", 0, close<T>),
				JS_CPPFUNC_DEF("allocate", 1, allocate<T>),
			});

			template <auto use_big_endian>
				requires BooleanConstraint
			inline static auto register_class(
				JSContext *ctx
			) -> void
			{
				static_assert(use_big_endian == true || use_big_endian == false, "use_big_endian can only be true or false");
				static_assert(sizeof(use_big_endian) == sizeof(bool));
				JS_NewClassID(JS_GetRuntime(ctx), &ClassID<use_big_endian>::value);
				assert_conditional(JS_NewClass(JS_GetRuntime(ctx), ClassID<use_big_endian>::value, &this_class<use_big_endian>) == 0, fmt::format("{} class register failed", _class_name<use_big_endian>()), "register_class");
				auto class_name = _class_name<use_big_endian>();
				auto make_constructor = JS_NewCFunction2(ctx, constructor<use_big_endian>, class_name.data(), 2, JS_CFUNC_constructor, 0);
				auto proto = JS_NewObject(ctx);
				JS_SetPropertyFunctionList(ctx, proto, proto_functions<use_big_endian>.data(), proto_functions<use_big_endian>.size());
				JS_SetConstructor(ctx, make_constructor, proto);
				auto global_obj = JS_GetGlobalObject(ctx);
				auto obj1 = make_instance_object(ctx, global_obj, "Sen"_sv);
				auto obj2 = make_instance_object(ctx, obj1, "Kernel"_sv);
				auto atom = Atom{ctx, class_name};
				JS_DefinePropertyValue(ctx, obj2, atom.value, make_constructor, int{JS_PROP_C_W_E});
				JS_FreeValue(ctx, global_obj);
				JS_FreeValue(ctx, obj1);
				JS_FreeValue(ctx, obj2);
				JS_FreeValue(ctx, proto);
				return;
			}

		}

		namespace Boolean
		{

			struct Data
			{
				bool value;

				Data() = default;

				Data(
					bool value) : value(value)
				{
				}

				~Data() = default;
			};

			inline static JSClassID class_id;

			inline static auto constructor(
				JSContext *ctx,
				JSValueConst new_target,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return proxy_wrapper(ctx, "constructor", [&]() {
					auto s = std::unique_ptr<Data, decltype(make_deleter<Data>())>(nullptr, make_deleter<Data>());
					auto obj = JS_UNDEFINED;
					auto proto = JSElement::Prototype{};
					if (argc == 1) {
						s.reset(new Data(JS_VALUE_GET_BOOL(argv[0]) == 0 ? false : true));
					}
					else {
						JS_ThrowInternalError(ctx, "Constructor for Boolean class does not match, expected 1 argument");
						return JS_EXCEPTION;
					}
					auto atom = Atom{ctx, "prototype"};
					proto = JS_GetProperty(ctx, new_target, atom.value);
					if (JS_IsException(proto)) {
						goto fail;
					}
					obj = JS_NewObjectProtoClass(ctx, proto, class_id);
					JS_FreeValue(ctx, proto);
					if (JS_IsException(obj)) {
						goto fail;
					}
					JS_SetOpaque(obj, s.release());
					return obj;
				fail:
					JS_FreeValue(ctx, obj);
					return JS_EXCEPTION;
				});
			}

			inline static auto constexpr _class_name(

			) -> std::string_view
			{
				return "Boolean";
			}

			inline static auto this_class = make_class_definition<Data>(
				_class_name(),
				class_id
			);

			inline static auto handle(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv,
				std::string_view method_name,
				std::function<JSValue(Data*)> method
			) -> JSValue {
				return proxy_wrapper(ctx, method_name, [&](){
					auto s = static_cast<Data*>(JS_GetOpaque2(ctx, this_val, class_id));
					if (s == nullptr) {
						return JS_EXCEPTION;
					}
					return method(s);
				});
			}

			inline static auto getter(
				JSContext *ctx,
				JSValueConst this_val,
				int magic
			) -> JSElement::boolean
			{
				return handle(ctx, this_val, 0, nullptr, "getter", [&](Data* s) {
					return JS::Converter::to_bool(ctx, s->value);
				});
			}

			inline static auto setter(
				JSContext *ctx,
				JSValueConst this_val,
				JSValueConst val,
				int magic
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, 0, nullptr, "getter", [&](Data* s) {
					s->value = JS_VALUE_GET_BOOL(val) == 0 ? false : true;
					return JS_UNDEFINED;
				});
			}

			inline static auto constexpr proto_functions = std::to_array<JSCFunctionListEntry>({
				JS_CPPGETSET_MAGIC_DEF("value", getter, setter, 0),
			});

			template <auto T>
			inline static auto make_instance (
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSDefine::Boolean
			{
				static_assert(T == true or T == false, "T must be true or false");
				static_assert(sizeof(T) == sizeof(Data));
				auto s = std::make_unique<Data>(T);
				auto atom = Atom{ctx, "prototype"};
				auto proto = JS_GetProperty(ctx, this_val, atom.value);
				if (JS_IsException(proto)) {
					return JS_EXCEPTION;
				}
				auto obj = JS_NewObjectProtoClass(ctx, proto, class_id);
				JS_FreeValue(ctx, proto);
				if (JS_IsException(obj)) {
					return JS_EXCEPTION;
				}
				JS_SetOpaque(obj, s.release());
				return obj;
			}

			inline static auto register_class(
				JSContext *ctx
			) -> void
			{
				JS_NewClassID(JS_GetRuntime(ctx), &class_id);
				assert_conditional(JS_NewClass(JS_GetRuntime(ctx), class_id, &this_class) == 0, "Boolean class register failed", "register_class");
				auto class_name = _class_name();
				auto point_ctor = JS_NewCFunction2(ctx, constructor, class_name.data(), 2, JS_CFUNC_constructor, 0);
				auto proto = JS_NewObject(ctx);
				auto default_true_func_val = JS_NewCFunction2(ctx, make_instance<true>, "true", 0, JS_CFUNC_generic, 0);
				auto default_false_func_val = JS_NewCFunction2(ctx, make_instance<false>, "false", 0, JS_CFUNC_generic, 0);
				auto trueAtom = Atom{ctx, "true"};
				JS_DefinePropertyValue(ctx, point_ctor, trueAtom.value, default_true_func_val, int{JS_PROP_C_W_E});
				auto falseAtom = Atom{ctx, "false"};
				JS_DefinePropertyValue(ctx, point_ctor, falseAtom.value, default_false_func_val, int{JS_PROP_C_W_E});
				JS_SetPropertyFunctionList(ctx, proto, proto_functions.data(), proto_functions.size());
				JS_SetConstructor(ctx, point_ctor, proto);
				auto global_obj = JS_GetGlobalObject(ctx);
				auto obj1 = make_instance_object(ctx, global_obj, "Sen"_sv);
				auto obj2 = make_instance_object(ctx, obj1, "Kernel"_sv);
				auto atom = Atom(ctx, class_name);
				JS_DefinePropertyValue(ctx, obj2, atom.value, point_ctor, int{JS_PROP_C_W_E});
				JS_FreeValue(ctx, global_obj);
				JS_FreeValue(ctx, proto);
				JS_FreeValue(ctx, obj2);
				JS_FreeValue(ctx, obj1);
				return;
			}

		}

		namespace Image
		{

			using Data = Kernel::Support::PopCap::Animation::Miscellaneous::Image;

			inline static JSClassID class_id;

			using Matrix = std::array<double, 6>;

			auto constexpr matrix_from_transform(Matrix &matrix, List<double> &transform) -> void
			{
				matrix = {transform[0], transform[1], transform[2], transform[3], transform[4], transform[5]};
			};

			inline static auto constructor(
				JSContext *ctx,
				JSValueConst new_target,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return proxy_wrapper(ctx, "constructor", [&]() {
					auto s = std::unique_ptr<Data, decltype(make_deleter<Data>())>(nullptr, make_deleter<Data>());
					auto obj = JS_UNDEFINED;
					auto proto = JSElement::Prototype{};
					if (argc == 3) {
						auto name = JS::Converter::get_string(ctx, argv[0]);
						auto id = JS::Converter::get_string(ctx, argv[1]);
						auto transform = JS::Converter::get_vector<double>(ctx, argv[2]);
						auto matrix = Matrix{};
						matrix_from_transform(matrix, transform);
						s.reset(new Data(name, id, matrix));
					}
					else {
						JS_ThrowInternalError(ctx, "Constructor for Image class does not match, expected 3 argument, got: %d", argc);
						return JS_EXCEPTION;
					}
					auto atom = Atom{ctx, "prototype"};
					proto = JS_GetProperty(ctx, new_target, atom.value);
					if (JS_IsException(proto)) {
						goto fail;
					}
					obj = JS_NewObjectProtoClass(ctx, proto, class_id);
					JS_FreeValue(ctx, proto);
					if (JS_IsException(obj)) {
						goto fail;
					}
					JS_SetOpaque(obj, s.release());
					return obj;
				fail:
					JS_FreeValue(ctx, obj);
					return JS_EXCEPTION;
				});
			}

			inline static auto constexpr _class_name(

			) -> std::string_view
			{
				return "Image";
			}

			inline static auto this_class = make_class_definition<Data>(
				_class_name(),
				class_id
			);

			enum class Value : uint8_t
			{
				name,
				id,
				transform,
			};

			inline static auto handle(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv,
				std::string_view method_name,
				std::function<JSValue(Data*)> method
			) -> JSValue {
				return proxy_wrapper(ctx, method_name, [&](){
					auto s = static_cast<Data*>(JS_GetOpaque2(ctx, this_val, class_id));
					if (s == nullptr) {
						return JS_EXCEPTION;
					}
					return method(s);
				});
			}

			inline static auto getter(
				JSContext *ctx,
				JSValueConst this_val,
				int magic
			) -> JSElement::any
			{
				return handle(ctx, this_val, 0, nullptr, "getter", [&](Data* s) {
					switch (static_cast<Value>(magic))
					{
						case Value::name:
						{
							return JS::Converter::to_string(ctx, s->name);
						}
						case Value::id:
						{
							return JS::Converter::to_string(ctx, s->id);
						}
						case Value::transform:
						{
							return JS::Converter::to_array(ctx, List<double>{s->transform.begin(), s->transform.end()});
						}
						default: {
							JS_ThrowInternalError(ctx, "Cannot find any getter to magic %d", magic);
							return JS_EXCEPTION;
						}
					}
				});
			}

			inline static auto setter(
				JSContext *ctx,
				JSValueConst this_val,
				JSValueConst val,
				int magic
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, 0, nullptr, "setter", [&](Data* s) {
					switch (static_cast<Value>(magic))
					{
						case Value::name:
						{
							s->name = JS::Converter::get_string(ctx, val);
							break;
						}
						case Value::id:
						{
							s->id = JS::Converter::get_string(ctx, val);
							break;
						}
						case Value::transform:
						{
							auto transform = JS::Converter::get_array<double>(ctx, val);
							matrix_from_transform(s->transform, transform);
							break;
						}
						default: {
							JS_ThrowInternalError(ctx, "Cannot find any setter to magic %d", magic);
							return JS_EXCEPTION;
						}
					}
					return JS_UNDEFINED;
				});
			}

			inline static auto constexpr proto_functions = std::to_array<JSCFunctionListEntry>({
				JS_CPPGETSET_MAGIC_DEF("name", getter, setter, static_cast<int>(Value::name)),
				JS_CPPGETSET_MAGIC_DEF("id", getter, setter, static_cast<int>(Value::id)),
				JS_CPPGETSET_MAGIC_DEF("transform", getter, setter, static_cast<int>(Value::transform)),
			});

			inline static auto register_class(
				JSContext *ctx
			) -> void
			{
				JS_NewClassID(JS_GetRuntime(ctx), &class_id);
				assert_conditional(JS_NewClass(JS_GetRuntime(ctx), class_id, &this_class) == 0, "Image class register failed", "register_class");
				auto class_name = _class_name();
				auto point_ctor = JS_NewCFunction2(ctx, constructor, class_name.data(), 2, JS_CFUNC_constructor, 0);
				auto proto = JS_NewObject(ctx);
				JS_SetPropertyFunctionList(ctx, proto, proto_functions.data(), proto_functions.size());
				JS_SetConstructor(ctx, point_ctor, proto);
				auto global_obj = JS_GetGlobalObject(ctx);
				auto obj1 = make_instance_object(ctx, global_obj, "Sen");
				auto obj2 = make_instance_object(ctx, obj1, "Kernel");
				auto obj3 = make_instance_object(ctx, obj2, "Support");
				auto obj4 = make_instance_object(ctx, obj3, "PopCap");
				auto obj5 = make_instance_object(ctx, obj4, "Animation");
				auto obj6 = make_instance_object(ctx, obj5, "Miscellaneous");
				auto atom = Atom{ctx, class_name};
				JS_DefinePropertyValue(ctx, obj6, atom.value, point_ctor, int{JS_PROP_C_W_E});
				JS_FreeValue(ctx, global_obj);
				JS_FreeValue(ctx, obj1);
				JS_FreeValue(ctx, obj2);
				JS_FreeValue(ctx, obj3);
				JS_FreeValue(ctx, obj4);
				JS_FreeValue(ctx, obj5);
				JS_FreeValue(ctx, obj6);
				JS_FreeValue(ctx, proto);
				return;
			}

		}

		namespace Sprite
		{

			using Data = Kernel::Support::PopCap::Animation::Miscellaneous::Sprite;

			inline static JSClassID class_id;

			using Matrix = std::array<double, 6>;

			using Color = std::array<double, 4>;

			auto constexpr matrix_from_transform(Matrix &matrix, List<double> &transform) -> void
			{
				matrix = {transform.at(0), transform.at(1), transform.at(2), transform.at(3), transform.at(4), transform.at(5)};
			};

			auto constexpr color_from_transform(Color &matrix, List<double> &transform) -> void
			{
				matrix = {transform.at(0), transform.at(1), transform.at(2), transform.at(3)};
			};

			inline static auto constructor(
				JSContext *ctx,
				JSValueConst new_target,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return proxy_wrapper(ctx, "constructor", [&]() {
					auto s = std::unique_ptr<Data, decltype(make_deleter<Data>())>(nullptr, make_deleter<Data>());
					auto obj = JS_UNDEFINED;
					auto proto = JSElement::Prototype{};
					if (argc == 4) {
						auto name = JS::Converter::get_string(ctx, argv[0]);
						auto link = JS::Converter::get_string(ctx, argv[1]);
						auto transform = JS::Converter::get_vector<double>(ctx, argv[2]);
						auto color = JS::Converter::get_vector<double>(ctx, argv[3]);
						auto matrix = Matrix{};
						auto basic_color = Color{};
						matrix_from_transform(matrix, transform);
						color_from_transform(basic_color, color);
						s.reset(new Data(name, link, matrix, basic_color));
					}
					else {
						JS_ThrowInternalError(ctx, "Constructor for Sprite class does not match, expected 4 argument, got: %d", argc);
						return JS_EXCEPTION;
					}
					auto atom = Atom{ctx, "prototype"};
					proto = JS_GetProperty(ctx, new_target, atom.value);
					if (JS_IsException(proto)) {
						goto fail;
					}
					obj = JS_NewObjectProtoClass(ctx, proto, class_id);
					JS_FreeValue(ctx, proto);
					if (JS_IsException(obj)) {
						goto fail;
					}
					JS_SetOpaque(obj, s.release());
					return obj;
				fail:
					JS_FreeValue(ctx, obj);
					return JS_EXCEPTION; 
				});
			}

			inline static auto handle(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv,
				std::string_view method_name,
				std::function<JSValue(Data*)> method
			) -> JSValue {
				return proxy_wrapper(ctx, method_name, [&](){
					auto s = static_cast<Data*>(JS_GetOpaque2(ctx, this_val, class_id));
					if (s == nullptr) {
						return JS_EXCEPTION;
					}
					return method(s);
				});
			}

			inline static auto _class_name(

			) -> std::string_view
			{
				return "Sprite";
			}

			inline static auto this_class = make_class_definition<Data>(
				_class_name(),
				class_id
			);

			enum class Value : uint8_t
			{
				name,
				link,
				transform,
				color,
			};

			inline static auto getter(
				JSContext *ctx,
				JSValueConst this_val,
				int magic
			) -> JSElement::any
			{
				return handle(ctx, this_val, 0, nullptr, "getter", [&](Data* s) {
					switch (static_cast<Value>(magic))
					{
						case Value::name:
						{
							return JS::Converter::to_string(ctx, s->name);
						}
						case Value::link:
						{
							return JS::Converter::to_string(ctx, s->link);
						}
						case Value::transform:
						{
							return JS::Converter::to_array(ctx, List<double>{s->transform.begin(), s->transform.end()});
						}
						case Value::color:
						{
							return JS::Converter::to_array(ctx, List<double>{s->color.begin(), s->color.end()});
						}
						default: {
							JS_ThrowInternalError(ctx, "Cannot call getter on magic: %d", magic);
							return JS_EXCEPTION;
						}
					}
				});
			}

			inline static auto setter(
				JSContext *ctx,
				JSValueConst this_val,
				JSValueConst val,
				int magic
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, 0, nullptr, "setter", [&](Data* s) {
					switch (static_cast<Value>(magic))
					{
						case Value::name:
						{
							s->name = JS::Converter::get_string(ctx, val);
						}
						case Value::link:
						{
							s->link = JS::Converter::get_string(ctx, val);
						}
						case Value::transform:
						{
							auto transform = JS::Converter::get_array<double>(ctx, val);
							matrix_from_transform(s->transform, transform);
						}
						case Value::color:
						{
							auto color = JS::Converter::get_array<double>(ctx, val);
							color_from_transform(s->color, color);
						}
						default: {
							JS_ThrowInternalError(ctx, "Cannot call setter on magic: %d", magic);
							return JS_EXCEPTION;
						}
					}
					return JS_UNDEFINED;
				});
			}

			inline static auto constexpr proto_functions = std::to_array<JSCFunctionListEntry>({
				JS_CPPGETSET_MAGIC_DEF("name", getter, setter, static_cast<int>(Value::name)),
				JS_CPPGETSET_MAGIC_DEF("link", getter, setter, static_cast<int>(Value::link)),
				JS_CPPGETSET_MAGIC_DEF("transform", getter, setter, static_cast<int>(Value::transform)),
				JS_CPPGETSET_MAGIC_DEF("color", getter, setter, static_cast<int>(Value::color)),
			});

			inline static auto register_class(
				JSContext *ctx
			) -> void
			{
				JS_NewClassID(JS_GetRuntime(ctx), &class_id);
				assert_conditional(JS_NewClass(JS_GetRuntime(ctx), class_id, &this_class) == 0, "Sprite class register failed", "register_class");
				auto class_name = _class_name();
				auto point_ctor = JS_NewCFunction2(ctx, constructor, class_name.data(), 2, JS_CFUNC_constructor, 0);
				auto proto = JS_NewObject(ctx);
				JS_SetPropertyFunctionList(ctx, proto, proto_functions.data(), proto_functions.size());
				JS_SetConstructor(ctx, point_ctor, proto);
				auto global_obj = JS_GetGlobalObject(ctx);
				auto obj1 = make_instance_object(ctx, global_obj, "Sen");
				auto obj2 = make_instance_object(ctx, obj1, "Kernel");
				auto obj3 = make_instance_object(ctx, obj2, "Support");
				auto obj4 = make_instance_object(ctx, obj3, "PopCap");
				auto obj5 = make_instance_object(ctx, obj4, "Animation");
				auto obj6 = make_instance_object(ctx, obj5, "Miscellaneous");
				auto atom = Atom{ctx, class_name};
				JS_DefinePropertyValue(ctx, obj6, atom.value, point_ctor, int{JS_PROP_C_W_E});
				JS_FreeValue(ctx, global_obj);
				JS_FreeValue(ctx, obj1);
				JS_FreeValue(ctx, obj2);
				JS_FreeValue(ctx, obj3);
				JS_FreeValue(ctx, obj4);
				JS_FreeValue(ctx, obj5);
				JS_FreeValue(ctx, obj6);
				JS_FreeValue(ctx, proto);
				return;
			}

		}

		namespace FileInputStream
		{

			using Data = Kernel::FileSystem::FileHandler;

			inline static JSClassID class_id;

			inline static auto constructor(
				JSContext *ctx,
				JSValueConst new_target,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return proxy_wrapper(ctx, "constructor", [&]() {
					auto s = std::unique_ptr<Data, decltype(make_deleter<Data>())>(nullptr, make_deleter<Data>());
					auto obj = JS_UNDEFINED;
					auto proto = JSElement::Prototype{};
					if (argc == 1) {
						auto source = JS::Converter::get_string(ctx, argv[0]);
						s.reset(new Data(source, "rb"));
					}
					else {
						JS_ThrowInternalError(ctx, "FileInputStream cannot be initialized because argument does not satisfy constructor");
						return JS_EXCEPTION;
					}
					auto atom = Atom{ctx, "prototype"};
					proto = JS_GetProperty(ctx, new_target, atom.value);
					if (JS_IsException(proto)) {
						goto fail;
					}
					obj = JS_NewObjectProtoClass(ctx, proto, class_id);
					JS_FreeValue(ctx, proto);
					if (JS_IsException(obj)) {
						goto fail;
					}
					JS_SetOpaque(obj, s.release());
					return obj;
				fail:
					JS_FreeValue(ctx, obj);
					return JS_EXCEPTION;
				});
			}

			inline static auto constexpr _class_name(

			) -> std::string_view
			{
				return "FileInputStream";
			}

			inline static auto this_class = make_class_definition<Data>(
				_class_name(),
				class_id
			);

			inline static auto handle(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv,
				std::string_view method_name,
				std::function<JSValue(Data*)> method
			) -> JSValue {
				return proxy_wrapper(ctx, method_name, [&](){
					auto s = static_cast<Data*>(JS_GetOpaque2(ctx, this_val, class_id));
					if (s == nullptr) {
						return JS_EXCEPTION;
					}
					return method(s);
				});
			}

			inline static auto close(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "close", [&](Data* s) {
					s->close();
					return JS_UNDEFINED;
				});
			}

			inline static auto size(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::bigint
			{
				return handle(ctx, this_val, argc, argv, "size", [&](Data* s) {
					return JS::Converter::to_bigint<std::uint64_t>(ctx, s->size());
				});
			}

			inline static auto read(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::bigint
			{
				return handle(ctx, this_val, argc, argv, "read", [&](Data* s) {
					return JS::Converter::to_bigint(ctx, static_cast<std::int64_t>(s->read()));
				});
			}

			inline static auto read_all(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::ArrayBuffer
			{
				return handle(ctx, this_val, argc, argv, "read_all", [&](Data* s) {
					auto data = s->read_all();
					return JS_NewArrayBufferCopy(ctx, data.data(), data.size());
				});
			}

			inline static auto getter(
				JSContext *ctx,
				JSValueConst this_val,
				int magic
			) -> JSElement::bigint
			{
				return handle(ctx, this_val, 0, nullptr, "getter", [&](Data* s) {
					return JS::Converter::to_bigint(ctx, s->position());
				});
			}

			inline static auto setter(
				JSContext *ctx,
				JSValueConst this_val,
				JSValueConst val,
				int magic
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, 0, nullptr, "setter", [&](Data* s) {
					s->position(static_cast<std::size_t>(JS::Converter::get_bigint64(ctx, val)));
					return JS_UNDEFINED;
				});
			}

			inline static auto constexpr proto_functions = std::to_array<JSCFunctionListEntry>({
				JS_CPPGETSET_MAGIC_DEF("position", getter, setter, 0),
				JS_CPPFUNC_DEF("close", 0, close),
				JS_CPPFUNC_DEF("size", 0, size),
				JS_CPPFUNC_DEF("read", 0, read),
				JS_CPPFUNC_DEF("read_all", 0, read_all),
			});

			inline static auto register_class(
				JSContext *ctx
			) -> void
			{
				JS_NewClassID(JS_GetRuntime(ctx), &class_id);
				assert_conditional(JS_NewClass(JS_GetRuntime(ctx), class_id, &this_class) == 0, "FileInputStream class register failed", "register_class");
				auto class_name = _class_name();
				auto point_ctor = JS_NewCFunction2(ctx, constructor, class_name.data(), 2, JS_CFUNC_constructor, 0);
				auto proto = JS_NewObject(ctx);
				JS_SetPropertyFunctionList(ctx, proto, proto_functions.data(), proto_functions.size());
				JS_SetConstructor(ctx, point_ctor, proto);
				auto global_obj = JS_GetGlobalObject(ctx);
				auto obj1 = make_instance_object(ctx, global_obj, "Sen");
				auto obj2 = make_instance_object(ctx, obj1, "Kernel");
				auto atom = Atom{ctx, class_name};
				JS_DefinePropertyValue(ctx, obj2, atom.value, point_ctor, int{JS_PROP_C_W_E});
				JS_FreeValue(ctx, global_obj);
				JS_FreeValue(ctx, obj1);
				JS_FreeValue(ctx, obj2);
				JS_FreeValue(ctx, proto);
				return;
			}

		}

		namespace FileOutputStream
		{

			using Data = Kernel::FileSystem::FileHandler;

			inline static JSClassID class_id;

			inline static auto constructor(
				JSContext *ctx,
				JSValueConst new_target,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return proxy_wrapper(ctx, "constructor", [&]() {
					auto s = std::unique_ptr<Data, decltype(make_deleter<Data>())>(nullptr, make_deleter<Data>());
					auto obj = JS_UNDEFINED;
					auto proto = JSElement::Prototype{};
					if (argc == 1) {
						auto source = JS::Converter::get_string(ctx, argv[0]);
						s.reset(new Data(source, "wb"));
					}
					else {
						JS_ThrowInternalError(ctx, "FileOutputStream cannot be initialized because the constructor does not satisfy the argument count");
						return JS_EXCEPTION;
					}
					auto atom = Atom{ctx, "prototype"};
					proto = JS_GetProperty(ctx, new_target, atom.value);
					if (JS_IsException(proto)) {
						goto fail;
					}
					obj = JS_NewObjectProtoClass(ctx, proto, class_id);
					JS_FreeValue(ctx, proto);
					if (JS_IsException(obj)) {
						goto fail;
					}
					JS_SetOpaque(obj, s.release());
					return obj;
				fail:
					JS_FreeValue(ctx, obj);
					return JS_EXCEPTION; 
				});
			}

			inline static auto constexpr _class_name(

			) -> std::string_view
			{
				return "FileOutputStream";
			}

			inline static auto this_class = make_class_definition<Data>(
				_class_name(),
				class_id
			);

			inline static auto handle(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv,
				std::string_view method_name,
				std::function<JSValue(Data*)> method
			) -> JSValue {
				return proxy_wrapper(ctx, method_name, [&](){
					auto s = static_cast<Data*>(JS_GetOpaque2(ctx, this_val, class_id));
					if (s == nullptr) {
						return JS_EXCEPTION;
					}
					return method(s);
				});
			}

			inline static auto close(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "close", [&](Data* s) {
					s->close();
					return JS_UNDEFINED;
				});
			}

			inline static auto size(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::bigint
			{
				return handle(ctx, this_val, argc, argv, "size", [&](Data* s) {
					return JS::Converter::to_bigint(ctx, s->size());
				});
			}

			inline static auto write(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "write", [&](Data* s) {
					assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "write");
					s->write(static_cast<char>(JS::Converter::get_bigint64(ctx, argv[0])));
					return JS_UNDEFINED;
				});
			}

			inline static auto write_all(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "write_all", [&](Data* s) {
					assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "write_all");
					s->write_all(JS::Converter::to_binary_list(ctx, argv[0]));
					return JS_UNDEFINED;
				});
			}

			inline static auto getter(
				JSContext *ctx,
				JSValueConst this_val,
				int magic
			) -> JSElement::bigint
			{
				return handle(ctx, this_val, 0, nullptr, "getter", [&](Data* s) {
					return JS::Converter::to_bigint(ctx, s->position());
				});
			}

			inline static auto setter(
				JSContext *ctx,
				JSValueConst this_val,
				JSValueConst val,
				int magic
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, 0, nullptr, "getter", [&](Data* s) {
					s->position(static_cast<std::size_t>(JS::Converter::get_bigint64(ctx, val)));
					return JS_UNDEFINED;
				});
			}

			inline static auto constexpr proto_functions = std::to_array<JSCFunctionListEntry>({
				JS_CPPGETSET_MAGIC_DEF("position", getter, setter, 0),
				JS_CPPFUNC_DEF("close", 0, close),
				JS_CPPFUNC_DEF("size", 0, size),
				JS_CPPFUNC_DEF("write", 0, write),
				JS_CPPFUNC_DEF("write_all", 0, write_all),
			});

			inline static auto register_class(
				JSContext *ctx
			) -> void
			{
				JS_NewClassID(JS_GetRuntime(ctx), &class_id);
				assert_conditional(JS_NewClass(JS_GetRuntime(ctx), class_id, &this_class) == 0, "FileOutputStream class register failed", "register_class");
				auto class_name = _class_name();
				auto point_ctor = JS_NewCFunction2(ctx, constructor, class_name.data(), 2, JS_CFUNC_constructor, 0);
				auto proto = JS_NewObject(ctx);
				JS_SetPropertyFunctionList(ctx, proto, proto_functions.data(), proto_functions.size());
				JS_SetConstructor(ctx, point_ctor, proto);
				auto global_obj = JS_GetGlobalObject(ctx);
				auto obj1 = make_instance_object(ctx, global_obj, "Sen");
				auto obj2 = make_instance_object(ctx, obj1, "Kernel");
				auto atom = Atom{ctx, class_name};
				JS_DefinePropertyValue(ctx, obj2, atom.value, point_ctor, int{JS_PROP_C_W_E});
				JS_FreeValue(ctx, global_obj);
				JS_FreeValue(ctx, obj1);
				JS_FreeValue(ctx, obj2);
				JS_FreeValue(ctx, proto);
				return;
			}

		}

		namespace FileStream
		{

			using Data = Kernel::FileSystem::FileHandler;

			inline static JSClassID class_id;

			inline static auto constructor(
				JSContext *ctx,
				JSValueConst new_target,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return proxy_wrapper(ctx, "constructor", [&](){
					auto s = std::unique_ptr<Data, decltype(make_deleter<Data>())>(nullptr, make_deleter<Data>());
					auto obj = JS_UNDEFINED;
					auto proto = JSElement::Prototype{};
					if (argc == 1) {
						auto source = JS::Converter::get_string(ctx, argv[0]);
						s.reset(new Data(source, "w+b"));
					}
					else {
						JS_ThrowInternalError(ctx, "FileStream cannot be initialized because expected argument count = 1 but got: %d", argc);
						return JS_EXCEPTION;
					}
					auto atom = Atom{ctx, "prototype"};
					proto = JS_GetProperty(ctx, new_target, atom.value);
					if (JS_IsException(proto)) {
						goto fail;
					}
					obj = JS_NewObjectProtoClass(ctx, proto, class_id);
					JS_FreeValue(ctx, proto);
					if (JS_IsException(obj)) {
						goto fail;
					}
					JS_SetOpaque(obj, s.release());
					return obj;
				fail:
					JS_FreeValue(ctx, obj);
					return JS_EXCEPTION;
				});
			}

			inline static auto constexpr _class_name(

			) -> std::string_view
			{
				return "FileStream";
			}

			inline static auto this_class = make_class_definition<Data>(
				_class_name(),
				class_id
			);

			inline static auto handle(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv,
				std::string_view method_name,
				std::function<JSValue(Data*)> method
			) -> JSValue {
				return proxy_wrapper(ctx, method_name, [&](){
					auto s = static_cast<Data*>(JS_GetOpaque2(ctx, this_val, class_id));
					if (s == nullptr) {
						return JS_EXCEPTION;
					}
					return method(s);
				});
			}

			inline static auto close(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "close", [&](Data* s){
					s->close();
					return JS_UNDEFINED;
				});
			}

			inline static auto size(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::bigint
			{
				return handle(ctx, this_val, argc, argv, "size", [&](Data* s){
					return JS::Converter::to_bigint(ctx, s->size());
				});
			}

			inline static auto write(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "write", [&](Data* s){
					assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "write");
					s->write(static_cast<char>(JS::Converter::get_bigint64(ctx, argv[0])));
					return JS_UNDEFINED;
				});
			}

			inline static auto write_all(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "write_all", [&](Data* s){
					assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "write_all");
					s->write_all(JS::Converter::to_binary_list(ctx, argv[0]));
					return JS_UNDEFINED;
				});
			}

			inline static auto read(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::bigint
			{
				return handle(ctx, this_val, argc, argv, "read", [&](Data* s){
					return JS::Converter::to_bigint(ctx, static_cast<std::int64_t>(s->read()));
				});
			}

			inline static auto read_all(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::ArrayBuffer
			{
				return handle(ctx, this_val, argc, argv, "read_all", [&](Data* s){
					auto data = s->read_all();
					return JS_NewArrayBufferCopy(ctx, data.data(), data.size());
				});
			}

			inline static auto getter(
				JSContext *ctx,
				JSValueConst this_val,
				int magic
			) -> JSElement::bigint
			{
				return handle(ctx, this_val, 0, nullptr, "getter", [&](Data* s){
					return JS::Converter::to_bigint(ctx, s->position());
				});
			}

			inline static auto setter(
				JSContext *ctx,
				JSValueConst this_val,
				JSValueConst val,
				int magic
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, 0, nullptr, "setter", [&](Data* s){
					s->position(static_cast<std::size_t>(JS::Converter::get_bigint64(ctx, val)));
					return JS_UNDEFINED;
				});
			}

			inline static auto constexpr proto_functions = std::to_array<JSCFunctionListEntry>({
				JS_CPPGETSET_MAGIC_DEF("position", getter, setter, 0),
				JS_CPPFUNC_DEF("close", 0, close),
				JS_CPPFUNC_DEF("size", 0, size),
				JS_CPPFUNC_DEF("write", 0, write),
				JS_CPPFUNC_DEF("write_all", 0, write_all),
				JS_CPPFUNC_DEF("read", 0, read),
				JS_CPPFUNC_DEF("read_all", 0, read_all),
			});

			inline static auto register_class(
				JSContext *ctx
			) -> void
			{
				JS_NewClassID(JS_GetRuntime(ctx), &class_id);
				assert_conditional(JS_NewClass(JS_GetRuntime(ctx), class_id, &this_class) == 0, "FileStream class register failed", "register_class");
				auto class_name = _class_name();
				auto point_ctor = JS_NewCFunction2(ctx, constructor, class_name.data(), 2, JS_CFUNC_constructor, 0);
				auto proto = JS_NewObject(ctx);
				JS_SetPropertyFunctionList(ctx, proto, proto_functions.data(), proto_functions.size());
				JS_SetConstructor(ctx, point_ctor, proto);
				auto global_obj = JS_GetGlobalObject(ctx);
				auto obj1 = make_instance_object(ctx, global_obj, "Sen");
				auto obj2 = make_instance_object(ctx, obj1, "Kernel");
				auto atom = Atom{ctx, class_name};
				JS_DefinePropertyValue(ctx, obj2, atom.value, point_ctor, int{JS_PROP_C_W_E});
				JS_FreeValue(ctx, global_obj);
				JS_FreeValue(ctx, obj1);
				JS_FreeValue(ctx, obj2);
				JS_FreeValue(ctx, proto);
				return;
			}

		}

		namespace JsonWriter
		{

			using Data = Kernel::Definition::JsonWriter;

			inline static JSClassID class_id;

			inline static auto constructor(
				JSContext *ctx,
				JSValueConst new_target,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return proxy_wrapper(ctx, "constructor", [&](){
					auto s = std::make_unique<Data>();
					auto obj = JS_UNDEFINED;
					auto proto = JSElement::Prototype{};
					auto atom = Atom{ctx, "prototype"};
					proto = JS_GetProperty(ctx, new_target, atom.value);
					if (JS_IsException(proto)) {
						goto fail;
					}
					obj = JS_NewObjectProtoClass(ctx, proto, class_id);
					JS_FreeValue(ctx, proto);
					if (JS_IsException(obj)) {
						goto fail;
					}
					JS_SetOpaque(obj, s.release());
					return obj;
				fail:
					JS_FreeValue(ctx, obj);
					return JS_EXCEPTION;					
				});
			}

			inline static auto constexpr _class_name(

			) -> std::string_view
			{
				return "JsonWriter";
			}

			inline static auto this_class = make_class_definition<Data>(
				_class_name(),
				class_id
			);

			inline static auto handle(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv,
				std::string_view method_name,
				std::function<JSValue(Data*)> method
			) -> JSValue {
				return proxy_wrapper(ctx, method_name, [&](){
					auto s = static_cast<Data*>(JS_GetOpaque2(ctx, this_val, class_id));
					if (s == nullptr) {
						return JS_EXCEPTION;
					}
					return method(s);
				});
			}

			inline static auto clear(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "clear", [&](Data* s) {
					s->Clear();
					return JS_UNDEFINED;
				});
			}

			inline static auto toString(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::string
			{
				return handle(ctx, this_val, argc, argv, "toString", [&](Data* s) {
					return JS::Converter::to_string(ctx, s->ToString());
				});
			}

			inline static auto writeStartArray(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "writeStartArray", [&](Data* s) {
					s->WriteStartArray();
					return JS_UNDEFINED;
				});
			}

			inline static auto writeEndArray(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "writeEndArray", [&](Data* s) {
					s->WriteEndArray();
					return JS_UNDEFINED;
				});
			}

			inline static auto writeStartObject(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "writeStartObject", [&](Data* s) {
					s->WriteStartObject();
					return JS_UNDEFINED;
				});
			}

			inline static auto writeEndObject(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "writeEndObject", [&](Data* s) {
					s->WriteEndObject();
					return JS_UNDEFINED;
				});
			}

			inline static auto writeBoolean(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "writeBoolean", [&](Data* s) {
					assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "writeBoolean");
					s->WriteBoolean(JS::Converter::get_bool(ctx, argv[0]));
					return JS_UNDEFINED;
				});
			}

			inline static auto writeNull(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "writeNull", [&](Data* s) {
					s->WriteNull();
					return JS_UNDEFINED;
				});
			}

			inline static auto writePropertyName(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "writePropertyName", [&](Data* s) {
					assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "writePropertyName");
					s->WritePropertyName(JS::Converter::get_string(ctx, argv[0]));
					return JS_UNDEFINED;
				});
			}

			template <typename T>
				requires (std::is_arithmetic<T>::value or std::is_same<T, std::string>::value && !std::is_pointer<T>::value)
			inline static auto writeValue(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				auto function_name = std::string{};
				if constexpr (std::is_same<T, std::string>::value)
				{
					function_name = "writeString";
				}
				if constexpr (std::is_integral<T>::value)
				{
					function_name = "writeBigInt";
				}
				if constexpr (std::is_floating_point<T>::value)
				{
					function_name = "writeNumber";
				}
				return handle(ctx, this_val, argc, argv, function_name, [&](Data* s) {
					assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), function_name);
					if constexpr (std::is_same<T, std::string>::value) {
						s->WriteValue(JS::Converter::get_string(ctx, argv[0]));
					}
					if constexpr (std::is_integral<T>::value) {
						s->WriteValue(JS::Converter::get_bigint64(ctx, argv[0]));
					}
					if constexpr (std::is_floating_point<T>::value) {
						s->WriteValue(JS::Converter::get_float64(ctx, argv[0]));
					}
					return JS_UNDEFINED;
				});
			}

			inline static auto getter(
				JSContext *ctx,
				JSValueConst this_val,
				int magic
			) -> JSElement::boolean
			{
				return handle(ctx, this_val, 0, nullptr, "getter", [&](Data* s) {
					return JS::Converter::to_bool(ctx, s->WriteIndent);
				});
			}

			inline static auto setter(
				JSContext *ctx,
				JSValueConst this_val,
				JSValueConst val,
				int magic
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, 0, nullptr, "setter", [&](Data* s) {
					s->WriteIndent = JS_VALUE_GET_BOOL(val) == 0 ? false : true;
					return JS_UNDEFINED;
				});
			}

			inline static auto constexpr proto_functions = std::to_array<JSCFunctionListEntry>({
				JS_CPPGETSET_MAGIC_DEF("write_indent", getter, setter, 0),
				JS_CPPFUNC_DEF("clear", 0, clear),
				JS_CPPFUNC_DEF("toString", 0, toString),
				JS_CPPFUNC_DEF("writeStartArray", 0, writeStartArray),
				JS_CPPFUNC_DEF("writeEndArray", 0, writeEndArray),
				JS_CPPFUNC_DEF("writeStartObject", 0, writeStartObject),
				JS_CPPFUNC_DEF("writeEndObject", 0, writeEndObject),
				JS_CPPFUNC_DEF("writeBoolean", 1, writeBoolean),
				JS_CPPFUNC_DEF("writeNull", 0, writeNull),
				JS_CPPFUNC_DEF("writePropertyName", 1, writePropertyName),
				JS_CPPFUNC_DEF("writeString", 1, writeValue<std::string>),
				JS_CPPFUNC_DEF("writeNumber", 1, writeValue<double>),
				JS_CPPFUNC_DEF("writeBigInt", 1, writeValue<std::int64_t>),
			});

			inline static auto register_class(
				JSContext *ctx
			) -> void
			{
				JS_NewClassID(JS_GetRuntime(ctx), &class_id);
				assert_conditional(JS_NewClass(JS_GetRuntime(ctx), class_id, &this_class) == 0, "JsonWriter class register failed", "register_class");
				auto class_name = _class_name();
				auto point_ctor = JS_NewCFunction2(ctx, constructor, class_name.data(), 2, JS_CFUNC_constructor, 0);
				auto proto = JS_NewObject(ctx);
				JS_SetPropertyFunctionList(ctx, proto, proto_functions.data(), proto_functions.size());
				JS_SetConstructor(ctx, point_ctor, proto);
				auto global_obj = JS_GetGlobalObject(ctx);
				auto obj1 = make_instance_object(ctx, global_obj, "Sen");
				auto obj2 = make_instance_object(ctx, obj1, "Kernel");
				auto atom = Atom{ctx, class_name};
				JS_DefinePropertyValue(ctx, obj2, atom.value, point_ctor, int{JS_PROP_C_W_E});
				JS_FreeValue(ctx, global_obj);
				JS_FreeValue(ctx, obj1);
				JS_FreeValue(ctx, obj2);
				JS_FreeValue(ctx, proto);
				return;
			}

		}

		namespace Number
		{

			template <typename T>
				requires (std::is_integral<T>::value or std::is_floating_point<T>::value) && (!std::is_pointer<T>::value && !std::is_class<T>::value)
			struct Data
			{
				T value{};

				Data() = default;

				Data(
					T value
				) : value(value)
				{
				}

				~Data() = default;
			};

			template <typename T>
				requires (std::is_integral<T>::value or std::is_floating_point<T>::value) && (!std::is_pointer<T>::value && !std::is_class<T>::value)
			struct NumberID
			{
				inline static JSClassID class_id = 0;
			};

			template <typename T>
				requires (std::is_integral<T>::value or std::is_floating_point<T>::value) && (!std::is_pointer<T>::value && !std::is_class<T>::value)
			inline static auto constructor(
				JSContext *ctx,
				JSValueConst new_target,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return proxy_wrapper(ctx, "constructor", [&]() {
					auto s = std::unique_ptr<Data<T>, decltype(make_deleter<Data<T>>())>(nullptr, make_deleter<Data<T>>());
					auto obj = JS_UNDEFINED;
					auto proto = JSElement::Prototype{};
					if (argc == 1) {
						if constexpr (std::is_integral<T>::value) {
							s.reset(new Data<T>(static_cast<T>(JS::Converter::get_bigint64(ctx, argv[0]))));
						}
						else {
							s.reset(new Data<T>(static_cast<T>(JS::Converter::get_float64(ctx, argv[0]))));
						}
					}
					else {
						s.reset(new Data<T>());
					}
					auto atom = Atom{ctx, "prototype"};
					proto = JS_GetProperty(ctx, new_target, atom.value);
					if (JS_IsException(proto)) {
						goto fail;
					}
					obj = JS_NewObjectProtoClass(ctx, proto, NumberID<T>::class_id);
					JS_FreeValue(ctx, proto);
					if (JS_IsException(obj)) {
						goto fail;
					}
					JS_SetOpaque(obj, s.release());
					return obj;
				fail:
					JS_FreeValue(ctx, obj);
					return JS_EXCEPTION;
				});
			}

			template <typename T> requires (std::is_integral<T>::value or std::is_floating_point<T>::value) && (!std::is_pointer<T>::value && !std::is_class<T>::value)
			inline static auto this_class = make_class_definition<Data<T>>(
				"Number",
				NumberID<T>::class_id
			);

			template <>
			inline static auto this_class<int8_t> = make_class_definition<Data<int8_t>>(
				"Integer8",
				NumberID<int8_t>::class_id
			);

			template <>
			inline static auto this_class<int16_t> = make_class_definition<Data<int16_t>>(
				"Integer16",
				NumberID<int16_t>::class_id
			);

			template <>
			inline static auto this_class<int32_t> = make_class_definition<Data<int32_t>>(
				"Integer32",
				NumberID<int32_t>::class_id
			);

			template <>
			inline static auto this_class<int64_t> = make_class_definition<Data<int64_t>>(
				"Integer64",
				NumberID<int64_t>::class_id
			);

			template <>
			inline static auto this_class<uint8_t> = make_class_definition<Data<uint8_t>>(
				"UInteger8",
				NumberID<uint8_t>::class_id
			);

			template <>
			inline static auto this_class<uint16_t> = make_class_definition<Data<uint16_t>>(
				"UInteger16",
				NumberID<uint16_t>::class_id
			);

			template <>
			inline static auto this_class<uint32_t> = make_class_definition<Data<uint32_t>>(
				"UInteger32",
				NumberID<uint32_t>::class_id
			);

			template <>
			inline static auto this_class<uint64_t> = make_class_definition<Data<uint64_t>>(
				"UInteger64",
				NumberID<uint32_t>::class_id
			);

			template <>
			inline static auto this_class<float> = make_class_definition<Data<float>>(
				"Float",
				NumberID<float>::class_id
			);

			template <>
			inline static auto this_class<double> = make_class_definition<Data<double>>(
				"Double",
				NumberID<double>::class_id
			);

			template <typename T>
				requires (std::is_integral<T>::value or std::is_floating_point<T>::value) && (!std::is_pointer<T>::value && !std::is_class<T>::value)
			inline static auto handle(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv,
				std::string_view method_name,
				std::function<JSValue(Data<T>*)> method
			) -> JSValue {
				return proxy_wrapper(ctx, method_name, [&](){
					auto s = static_cast<Data<T>*>(JS_GetOpaque2(ctx, this_val, NumberID<T>::class_id));
					if (s == nullptr) {
						return JS_EXCEPTION;
					}
					return method(s);
				});
			}

			template <typename T>
				requires (std::is_integral<T>::value or std::is_floating_point<T>::value) && (!std::is_pointer<T>::value && !std::is_class<T>::value)
			inline static auto getter(
				JSContext *ctx,
				JSValueConst this_val,
				int magic
			) -> JSElement::any
			{
				return handle<T>(ctx, this_val, 0, nullptr, "getter", [&](Data<T>* s){
					if constexpr (std::is_integral<T>::value)
					{
						return JS::Converter::to_bigint(ctx, s->value);
					}
					else
					{
						return JS::Converter::to_number(ctx, s->value);
					}
				});
			}

			template <typename T> requires (std::is_integral<T>::value or std::is_floating_point<T>::value) && (!std::is_pointer<T>::value && !std::is_class<T>::value)
			inline static auto setter(
				JSContext *ctx,
				JSValueConst this_val,
				JSValueConst val,
				int magic
			) -> JSElement::undefined
			{
				return handle<T>(ctx, this_val, 0, nullptr, "setter", [&](Data<T>* s){
					if constexpr (std::is_integral<T>::value)
					{
						s->value = static_cast<T>(JS::Converter::get_bigint64(ctx, val));
					}
					else
					{
						s->value = static_cast<T>(JS::Converter::get_float64(ctx, val));
					}
					return JS_UNDEFINED;
				});
			}

			template <typename T> requires (std::is_integral<T>::value or std::is_floating_point<T>::value) && (!std::is_pointer<T>::value && !std::is_class<T>::value)
			inline static auto constexpr proto_functions = std::to_array<JSCFunctionListEntry>({
				JS_CPPGETSET_MAGIC_DEF("value", getter<T>, setter<T>, 0),
			});

			template <typename T> requires (std::is_integral<T>::value or std::is_floating_point<T>::value) && (!std::is_pointer<T>::value && !std::is_class<T>::value)
			inline static auto register_class(
				JSContext *ctx
			) -> void
			{
				JS_NewClassID(JS_GetRuntime(ctx), &(NumberID<T>::class_id));
				auto class_name = fmt::format("{}", this_class<T>.class_name);
				assert_conditional(JS_NewClass(JS_GetRuntime(ctx), NumberID<T>::class_id, &this_class<T>) == 0, fmt::format("{} class register failed", class_name), "register_class");
				auto point_ctor = JS_NewCFunction2(ctx, constructor<T>, class_name.data(), 2, JS_CFUNC_constructor, 0);
				auto proto = JS_NewObject(ctx);
				JS_SetPropertyFunctionList(ctx, proto, proto_functions<T>.data(), proto_functions<T>.size());
				JS_SetConstructor(ctx, point_ctor, proto);
				auto global_obj = JS_GetGlobalObject(ctx);
				auto obj1 = make_instance_object(ctx, global_obj, "Sen");
				auto obj2 = make_instance_object(ctx, obj1, "Kernel");
				auto atom = Atom{ctx, class_name};
				JS_DefinePropertyValue(ctx, obj2, atom.value, point_ctor, int{JS_PROP_C_W_E});
				JS_FreeValue(ctx, global_obj);
				JS_FreeValue(ctx, obj1);
				JS_FreeValue(ctx, obj2);
				JS_FreeValue(ctx, proto);
				return;
			}

		}


		namespace APNGMakerSetting
		{

			using Data = Definition::APNGMakerSetting;

			inline static JSClassID class_id;

			enum GetterSetter
			{
				delay_frames_list,
				loop,
				width,
				height,
				trim,
			};

			inline static auto constructor(
				JSContext *ctx,
				JSValueConst new_target,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return proxy_wrapper(ctx, "constructor", [&]() {
					using Uinteger32 = Number::NumberID<std::uint32_t>;
					using Uinteger32C = Number::Data<std::uint32_t>;
					auto s = std::unique_ptr<Data, decltype(make_deleter<Data>())>(nullptr, make_deleter<Data>());
					auto obj = JS_UNDEFINED;
					auto proto = JSElement::Prototype{};
					if (argc == 5) {
						auto delay_frames_list = JS::Converter::get_array<std::uint32_t>(ctx, argv[0]);
						auto loop = static_cast<Uinteger32C*>(JS_GetOpaque2(ctx, argv[1], Uinteger32::class_id));
						if (loop == nullptr) {
							return JS_EXCEPTION;
						}
						auto width = static_cast<Uinteger32C*>(JS_GetOpaque2(ctx, argv[2], Uinteger32::class_id));
						if (width == nullptr) {
							return JS_EXCEPTION;
						}
						auto height = static_cast<Uinteger32C*>(JS_GetOpaque2(ctx, argv[3], Uinteger32::class_id));
						if (height == nullptr) {
							return JS_EXCEPTION;
						}
						auto trim = static_cast<Boolean::Data*>(JS_GetOpaque2(ctx, argv[4], Boolean::class_id));
						if (trim == nullptr) {
							return JS_EXCEPTION;
						}
						s.reset(new Data(delay_frames_list, loop->value, width->value, height->value, trim->value));
						}
					else {
						s.reset(new Data());
					}
					auto atom = Atom{ctx, "prototype"};
					proto = JS_GetProperty(ctx, new_target, atom.value);
					if (JS_IsException(proto)) {
						goto fail;
					}
					obj = JS_NewObjectProtoClass(ctx, proto, class_id);
					JS_FreeValue(ctx, proto);
					if (JS_IsException(obj)) {
						goto fail;
					}
					JS_SetOpaque(obj, s.release());
					return obj;
					fail:
						JS_FreeValue(ctx, obj);
						return JS_EXCEPTION; 
				});
			}

			inline auto constexpr _class_name( 

			) -> std::string_view
			{
				return "APNGMakerSetting";
			}

			inline static auto this_class = make_class_definition<Data>(
				_class_name(),
				class_id
			);

			inline static auto handle(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv,
				std::string_view method_name,
				std::function<JSValue(Data*)> method
			) -> JSValue {
				return proxy_wrapper(ctx, method_name, [&](){
					auto s = static_cast<Data*>(JS_GetOpaque2(ctx, this_val, class_id));
					if (s == nullptr) {
						return JS_EXCEPTION;
					}
					return method(s);
				});
			}

			inline static auto getter(
				JSContext *ctx,
				JSValueConst this_val,
				int magic
			) -> JSElement::any
			{
				return handle(ctx, this_val, 0, nullptr, "getter", [&](Data* s) {
					switch (static_cast<GetterSetter>(magic))
					{
						case GetterSetter::delay_frames_list:
						{
							return JS::Converter::to_array(ctx, s->delay_frames_list);
						}
						case GetterSetter::loop:
						{
							return JS::Converter::to_bigint(ctx, s->loop);
						}
						case GetterSetter::width:
						{
							return JS::Converter::to_bigint(ctx, s->width);
						}
						case GetterSetter::height:
						{
							return JS::Converter::to_bigint(ctx, s->height);
						}
						case GetterSetter::trim:
						{
							return JS::Converter::to_bool(ctx, s->trim);
						}
						default: {
							JS_ThrowInternalError(ctx, "Cannot call getter from magic %d", magic);
							return JS_EXCEPTION;
						}
					}
				});
			}

			inline static auto setter(
				JSContext *ctx,
				JSValueConst this_val,
				JSValueConst val,
				int magic
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, 0, nullptr, "getter", [&](Data* s) {
					switch (static_cast<GetterSetter>(magic))
					{
						case GetterSetter::delay_frames_list:
						{
							s->delay_frames_list.clear();
							auto delay_frames_list = JS::Converter::get_array<uint32_t>(ctx, val);
							s->delay_frames_list.assign(delay_frames_list.begin(), delay_frames_list.end());
							break;
						}
						case GetterSetter::loop:
						{
							s->loop = static_cast<std::uint32_t>(JS::Converter::get_bigint64(ctx, val));
							break;
						}
						case GetterSetter::width:
						{
							s->width = static_cast<std::uint32_t>(JS::Converter::get_bigint64(ctx, val));
							break;
						}
						case GetterSetter::height:
						{
							s->height = static_cast<std::uint32_t>(JS::Converter::get_bigint64(ctx, val));
							break;
						}
						case GetterSetter::trim:
						{
							s->trim = static_cast<bool>(JS::Converter::get_bool(ctx, val));
							break;
						}
						default: {
							JS_ThrowInternalError(ctx, "Cannot call setter from magic %d", magic);
							return JS_EXCEPTION;
						}
					}
					return JS_UNDEFINED;
				});
			}

			inline static auto constexpr proto_functions = std::to_array<JSCFunctionListEntry>({
				JS_CPPGETSET_MAGIC_DEF("delay_frames_list", getter, setter, GetterSetter::delay_frames_list),
				JS_CPPGETSET_MAGIC_DEF("loop", getter, setter, GetterSetter::loop),
				JS_CPPGETSET_MAGIC_DEF("width", getter, setter, GetterSetter::width),
				JS_CPPGETSET_MAGIC_DEF("height", getter, setter, GetterSetter::height),
				JS_CPPGETSET_MAGIC_DEF("trim", getter, setter, GetterSetter::trim),
			});

			inline static auto register_class(
				JSContext *ctx) -> void
			{
				JS_NewClassID(JS_GetRuntime(ctx), &class_id);
				assert_conditional(JS_NewClass(JS_GetRuntime(ctx), class_id, &this_class) == 0, "APNGMakerSetting class failed register", "register_class");
				auto class_name = _class_name();
				auto point_ctor = JS_NewCFunction2(ctx, constructor, class_name.data(), 2, JS_CFUNC_constructor, 0);
				auto proto = JS_NewObject(ctx);
				JS_SetPropertyFunctionList(ctx, proto, proto_functions.data(), proto_functions.size());
				JS_SetConstructor(ctx, point_ctor, proto);
				auto global_obj = JS_GetGlobalObject(ctx);
				auto obj1 = make_instance_object(ctx, global_obj, "Sen");
				auto obj2 = make_instance_object(ctx, obj1, "Kernel");
				auto atom = Atom{ctx, class_name};
				JS_DefinePropertyValue(ctx, obj2, atom.value, point_ctor, int{JS_PROP_C_W_E});
				JS_FreeValue(ctx, global_obj);
				JS_FreeValue(ctx, obj1);
				JS_FreeValue(ctx, obj2);
				JS_FreeValue(ctx, proto);
				return;
			}

		}

		namespace Size
		{

			struct Data
			{

				std::size_t value;

				Data() = default;

				Data(
					std::size_t value
				) : value(value)
				{
				}

				~Data() = default;
			};

			inline static JSClassID class_id;

			inline static auto constructor(
				JSContext *ctx,
				JSValueConst new_target,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return proxy_wrapper(ctx, "constructor", [&](){
					auto s = std::unique_ptr<Data, decltype(make_deleter<Data>())>(nullptr, make_deleter<Data>());
					auto obj = JS_UNDEFINED;
					auto proto = JSElement::Prototype{};
					if (argc == 1) {
						s.reset(new Data{static_cast<std::size_t>(JS::Converter::get_bigint64(ctx, argv[0]))});
					}
					else {
						JS_ThrowInternalError(ctx, "Size class cannot be initialized because number of argument does not match. Expected: %d, got: %d", 1, argc);
						return JS_EXCEPTION;
					}
					auto atom = Atom{ctx, "prototype"};
					proto = JS_GetProperty(ctx, new_target, atom.value);
					if (JS_IsException(proto)) {
						goto fail;
					}
					obj = JS_NewObjectProtoClass(ctx, proto, class_id);
					JS_FreeValue(ctx, proto);
					if (JS_IsException(obj)) {
						goto fail;
					}
					JS_SetOpaque(obj, s.release());
					return obj;
				fail:
					JS_FreeValue(ctx, obj);
					return JS_EXCEPTION;
				});
			}

			inline auto constexpr _class_name( 

			) -> std::string_view
			{
				return "Size";
			}

			inline static auto this_class = make_class_definition<Data>(
				_class_name(),
				class_id
			);

			inline static auto handle(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv,
				std::string_view method_name,
				std::function<JSValue(Data*)> method
			) -> JSValue {
				return proxy_wrapper(ctx, method_name, [&](){
					auto s = static_cast<Data*>(JS_GetOpaque2(ctx, this_val, class_id));
					if (s == nullptr) {
						return JS_EXCEPTION;
					}
					return method(s);
				});
			}

			inline static auto getter(
				JSContext *ctx,
				JSValueConst this_val,
				int magic
			) -> JSElement::bigint
			{
				return handle(ctx, this_val, 0, nullptr, "getter", [&](Data* s){
					return JS::Converter::to_bigint(ctx, s->value);
				});
			}

			inline static auto setter(
				JSContext *ctx,
				JSValueConst this_val,
				JSValueConst val,
				int magic
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, 0, nullptr, "setter", [&](Data* s){
					s->value = JS::Converter::get_bigint64(ctx, val);
					return JS_UNDEFINED;
				});
			}

			inline static auto constexpr proto_functions = std::to_array<JSCFunctionListEntry>({
				JS_CPPGETSET_MAGIC_DEF("value", getter, setter, 0),
			});

			inline static auto instance(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSDefine::Size
			{
				return proxy_wrapper(ctx, "instance", [&](){
					assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "instance");
					auto s = std::make_unique<Data>(static_cast<std::size_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					auto atom = Atom{ctx, "prototype"};
					auto proto = JS_GetProperty(ctx, this_val, atom.value);
					if (JS_IsException(proto)) {
						return JS_EXCEPTION;
					}
					auto obj = JS_NewObjectProtoClass(ctx, proto, class_id);
					JS_FreeValue(ctx, proto);
					if (JS_IsException(obj)) {
						return JS_EXCEPTION;
					}
					JS_SetOpaque(obj, s.release());
					return obj;
				});
			}

			inline static auto register_class(
				JSContext *ctx
			) -> void
			{
				JS_NewClassID(JS_GetRuntime(ctx), &class_id);
				assert_conditional(JS_NewClass(JS_GetRuntime(ctx), class_id, &this_class) == 0, "Size class failed register", "register_class");
				auto class_name = _class_name();
				auto point_ctor = JS_NewCFunction2(ctx, constructor, class_name.data(), 2, JS_CFUNC_constructor, 0);
				auto proto = JS_NewObject(ctx);
				auto instance_c = JS_NewCFunction2(ctx, instance, "instance", 0, JS_CFUNC_generic, 0);
				auto atom = Atom{ctx, "instance"};
				JS_DefinePropertyValue(ctx, point_ctor, atom.value, instance_c, int{JS_PROP_C_W_E});
				JS_SetPropertyFunctionList(ctx, proto, proto_functions.data(), proto_functions.size());
				JS_SetConstructor(ctx, point_ctor, proto);
				auto global_obj = JS_GetGlobalObject(ctx);
				auto obj1 = make_instance_object(ctx, global_obj, "Sen");
				auto obj2 = make_instance_object(ctx, obj1, "Kernel");
				auto atomData = Atom{ctx, class_name};
				JS_DefinePropertyValue(ctx, obj2, atomData.value, point_ctor, int{JS_PROP_C_W_E});
				JS_FreeValue(ctx, global_obj);
				JS_FreeValue(ctx, obj1);
				JS_FreeValue(ctx, obj2);
				JS_FreeValue(ctx, proto);
				return;
			}

		}


		namespace Character
		{

			template <typename T>
				requires std::is_same<T, char>::value or std::is_same<T, unsigned char>::value or std::is_same<T, wchar_t>::value && (!std::is_class<T>::value && !std::is_pointer<T>::value)
			struct Data
			{

				static_assert(sizeof(T) == sizeof(char) || sizeof(T) == sizeof(unsigned char) || sizeof(T) == sizeof(wchar_t), "invalid size");

				T value;

				Data() = default;

				Data(
					T value) : value(value)
				{
				}

				~Data(

					) = default;
			};

			template <typename T>
				requires std::is_same<T, char>::value or std::is_same<T, unsigned char>::value or std::is_same<T, wchar_t>::value && (!std::is_class<T>::value && !std::is_pointer<T>::value)
			struct ClassID
			{
				inline static JSClassID value;
			};

			template <typename T>
				requires std::is_same<T, char>::value or std::is_same<T, unsigned char>::value or std::is_same<T, wchar_t>::value && (!std::is_class<T>::value && !std::is_pointer<T>::value)
			inline static auto constructor(
				JSContext *ctx,
				JSValueConst new_target,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return proxy_wrapper(ctx, "constructor", [&](){
					auto s = std::unique_ptr<Data<T>, decltype(make_deleter<Data<T>>())>(nullptr, make_deleter<Data<T>>());
					auto obj = JS_UNDEFINED;
					auto proto = JSDefine::Character{};
					if (argc == 1) {
						s.reset(new Data<T>{static_cast<T>(JS::Converter::get_bigint64(ctx, argv[0]))});
					}
					else {
						s.reset(new Data<T>());
					}
					auto atom = Atom{ctx, "prototype"};
					proto = JS_GetProperty(ctx, new_target, atom.value);
					if (JS_IsException(proto)) {
						goto fail;
					}
					obj = JS_NewObjectProtoClass(ctx, proto, ClassID<T>::value);
					JS_FreeValue(ctx, proto);
					if (JS_IsException(obj)) {
						goto fail;
					}
					JS_SetOpaque(obj, s.release());
					return obj;
				fail:
					JS_FreeValue(ctx, obj);
					return JS_EXCEPTION;
				});
			}

			template <typename T>
				requires std::is_same<T, char>::value or std::is_same<T, unsigned char>::value or std::is_same<T, wchar_t>::value && (!std::is_class<T>::value && !std::is_pointer<T>::value)
			inline static auto this_class = make_class_definition<Data<T>>(
				"Character",
				ClassID<T>::value
			);

			template <>
			inline static auto this_class<unsigned char> = make_class_definition<Data<unsigned char>>(
				"UCharacter",
				ClassID<unsigned char>::value
			); 

			template <>
			inline static auto this_class<wchar_t> = make_class_definition<Data<wchar_t>>(
				"WideCharacter",
				ClassID<wchar_t>::value
			);

			template <typename T> 
				requires std::is_same<T, char>::value or std::is_same<T, unsigned char>::value or std::is_same<T, wchar_t>::value && (!std::is_class<T>::value && !std::is_pointer<T>::value)
			inline static auto handle(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv,
				std::string_view method_name,
				std::function<JSValue(Data<T>*)> method
			) -> JSValue {
				return proxy_wrapper(ctx, method_name, [&](){
					auto s = static_cast<Data<T>*>(JS_GetOpaque2(ctx, this_val, ClassID<T>::value));
					if (s == nullptr) {
						return JS_EXCEPTION;
					}
					return method(s);
				});
			}

			template <typename T> 
				requires std::is_same<T, char>::value or std::is_same<T, unsigned char>::value or std::is_same<T, wchar_t>::value && (!std::is_class<T>::value && !std::is_pointer<T>::value)
			inline static auto getter(
				JSContext *ctx,
				JSValueConst this_val,
				int magic
			) -> JSElement::bigint
			{
				return handle<T>(ctx, this_val, 0, nullptr, "getter", [&](Data<T>* s) {
					return JS::Converter::to_bigint(ctx, static_cast<std::int64_t>(s->value));
				});
			}

			template <typename T>
				requires std::is_same<T, char>::value or std::is_same<T, unsigned char>::value or std::is_same<T, wchar_t>::value && (!std::is_class<T>::value && !std::is_pointer<T>::value)
			inline static auto setter(
				JSContext *ctx,
				JSValueConst this_val,
				JSValueConst val,
				int magic
			) -> JSElement::undefined
			{
				return handle<T>(ctx, this_val, 0, nullptr, "getter", [&](Data<T>* s) {
					s->value = static_cast<T>(JS::Converter::get_bigint64(ctx, val));
					return JS_UNDEFINED;
				});
			}

			template <typename T>
				requires std::is_same<T, char>::value or std::is_same<T, unsigned char>::value or std::is_same<T, wchar_t>::value && (!std::is_class<T>::value && !std::is_pointer<T>::value)
			inline static auto constexpr proto_functions = std::to_array<JSCFunctionListEntry>({
				JS_CPPGETSET_MAGIC_DEF("value", getter<T>, setter<T>, 0),
			});

			template <typename T>
				requires std::is_same<T, char>::value or std::is_same<T, unsigned char>::value or std::is_same<T, wchar_t>::value && (!std::is_class<T>::value && !std::is_pointer<T>::value)
			inline static auto instance(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSDefine::Character
			{
				return proxy_wrapper(ctx, "instance", [&](){
					assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "instance");
					auto s = std::make_unique<Data<T>>(static_cast<T>(JS::Converter::get_bigint64(ctx, argv[0])));
					auto atom = Atom{ctx, "prototype"};
					auto proto = JS_GetProperty(ctx, this_val, atom.value);
					if (JS_IsException(proto)) {
						return JS_EXCEPTION;
					}
					auto obj = JS_NewObjectProtoClass(ctx, proto, ClassID<T>::value);
					JS_FreeValue(ctx, proto);
					if (JS_IsException(obj)) {
						return JS_EXCEPTION;
					}
					JS_SetOpaque(obj, s.release());
					return obj;
				});
			}

			template <typename T>
				requires std::is_same<T, char>::value or std::is_same<T, unsigned char>::value or std::is_same<T, wchar_t>::value && (!std::is_class<T>::value && !std::is_pointer<T>::value)
			inline static auto register_class(
				JSContext *ctx
			) -> void
			{
				JS_NewClassID(JS_GetRuntime(ctx), &(ClassID<T>::value));
				assert_conditional(JS_NewClass(JS_GetRuntime(ctx), ClassID<T>::value, &this_class<T>) == 0, fmt::format("{} class register failed", this_class<T>.class_name), "register_class");
				auto class_name = std::string_view{};
				if constexpr (std::is_same<T, char>::value)
				{
					class_name = "Character"_sv;
				}
				else if constexpr (std::is_same<T, wchar_t>::value)
				{
					class_name = "WideCharacter"_sv;
				}
				else
				{
					class_name = "UCharacter"_sv;
				}
				auto point_ctor = JS_NewCFunction2(ctx, constructor<T>, class_name.data(), 2, JS_CFUNC_constructor, 0);
				auto proto = JS_NewObject(ctx);
				auto instance_c = JS_NewCFunction2(ctx, instance<T>, "instance", 0, JS_CFUNC_generic, 0);
				auto atom = Atom{ctx, "instance"};
				JS_DefinePropertyValue(ctx, point_ctor, atom.value, instance_c, int{JS_PROP_C_W_E});
				JS_SetPropertyFunctionList(ctx, proto, proto_functions<T>.data(), proto_functions<T>.size());
				JS_SetConstructor(ctx, point_ctor, proto);
				auto global_obj = JS_GetGlobalObject(ctx);
				auto obj1 = make_instance_object(ctx, global_obj, "Sen");
				auto obj2 = make_instance_object(ctx, obj1, "Kernel");
				auto atomData = Atom{ctx, class_name};
				JS_DefinePropertyValue(ctx, obj2, atomData.value, point_ctor, int{JS_PROP_C_W_E});
				JS_FreeValue(ctx, global_obj);
				JS_FreeValue(ctx, obj1);
				JS_FreeValue(ctx, obj2);
				JS_FreeValue(ctx, proto);
				return;
			}

		}

		// String Support

		namespace String
		{

			struct CData
			{
				char *value = nullptr;

				std::size_t size{};

				CData(

					) = default;

				CData(
					std::size_t size) : size(size)
				{
				}

				CData(
					char *value,
					std::size_t size) : value(value), size(size)
				{
				}

				CData(
					char *value) : value(value), size(strlen(value))
				{
				}

				~CData()
				{
					free(const_cast<char *>(value));
				}

				auto view(

					) -> std::string_view
				{
					return std::string_view{thiz.value, thiz.size};
				}
			};

			using Data = CData;

			inline static JSClassID class_id;

			inline static auto constructor(
				JSContext *ctx,
				JSValueConst new_target,
				int argc,
				JSValueConst *argv
			) -> JSValue
			{
				return proxy_wrapper(ctx, "constructor", [&](){
					auto s = std::unique_ptr<Data, decltype(make_deleter<Data>())>(nullptr, make_deleter<Data>());
					auto obj = JS_UNDEFINED;
					auto proto = JSDefine::String{};
					if (argc == 1) {
						s.reset(new Data());
						auto str = JS::Converter::get_string(ctx, argv[0]);
						s->value = static_cast<char*>(malloc(sizeof(char) * str.size() + 1));
						if (s->value == nullptr) {
							JS_ThrowReferenceError(ctx, "could not allocate Kernel String");
							return JS_EXCEPTION;
						}
						std::memcpy(s->value, str.data(), str.size());
						s->size = str.size();
						s->value[str.size()] = '\0';
					}
					else {
						return JS_EXCEPTION;
					}
					auto atom = Atom{ctx, "prototype"};
					proto = JS_GetProperty(ctx, new_target, atom.value);
					if (JS_IsException(proto)) {
						goto fail;
					}
					obj = JS_NewObjectProtoClass(ctx, proto, class_id);
					JS_FreeValue(ctx, proto);
					if (JS_IsException(obj)) {
						goto fail;
					}
					JS_SetOpaque(obj, s.release());
					return obj;
				fail:
					JS_FreeValue(ctx, obj);
					return JS_EXCEPTION;
				});
			}

			inline static auto constexpr _class_name(

			) -> std::string_view
			{
				return "String";
			}

			inline static auto this_class = make_class_definition<Data>(
				_class_name(),
				class_id
			);

			inline static auto handle(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv,
				std::string_view method_name,
				std::function<JSValue(Data*)> method
			) -> JSValue {
				return proxy_wrapper(ctx, method_name, [&](){
					auto s = static_cast<Data*>(JS_GetOpaque2(ctx, this_val, class_id));
					if (s == nullptr) {
						return JS_EXCEPTION;
					}
					return method(s);
				});
			}

			inline static auto getter(
				JSContext *ctx,
				JSValueConst this_val,
				int magic
			) -> JSElement::string
			{
				return handle(ctx, this_val, 0, nullptr, "getter", [&](Data* s) {
					return JS::Converter::to_string(ctx, s->view());
				});
			}

			inline static auto setter(
				JSContext *ctx,
				JSValueConst this_val,
				JSValueConst val,
				int magic
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, 0, nullptr, "setter", [&](Data* s) {
					auto str = JS::Converter::get_string(ctx, val);
					if (s->value != nullptr) {
						std::free(s->value);
					}
					s->value = static_cast<char *>(std::malloc(sizeof(char) * str.size() + 1));
					std::memcpy(s->value, str.data(), str.size());
					s->size = str.size();
					s->value[str.size()] = '\0';
					return JS_UNDEFINED;
				});
			}

			inline static auto constexpr proto_functions = std::to_array<JSCFunctionListEntry>({
				JS_CPPGETSET_MAGIC_DEF("value", getter, setter, 0),
			});

			inline static auto instance(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSDefine::String
			{
				return proxy_wrapper(ctx, "instance", [&](){
					assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "instance");
					auto s = std::make_unique<Data>();
					auto str = JS::Converter::get_string(ctx, argv[0]);
					s->value = static_cast<char*>(malloc(sizeof(char) * str.size() + 1));
					std::memcpy(s->value, str.data(), str.size());
					s->size = str.size();
					s->value[str.size()] = '\0';
					auto atom = Atom{ctx, "prototype"};
					auto proto = JS_GetProperty(ctx, this_val, atom.value);
					if (JS_IsException(proto)) {
						return JS_EXCEPTION;
					}
					auto obj = JS_NewObjectProtoClass(ctx, proto, class_id);
					JS_FreeValue(ctx, proto);
					if (JS_IsException(obj)) {
						return JS_EXCEPTION;
					}
					JS_SetOpaque(obj, s.release());
					return obj;
				});
			}

			inline static auto register_class(
				JSContext *ctx
			) -> void
			{
				JS_NewClassID(JS_GetRuntime(ctx), &class_id);
				assert_conditional(JS_NewClass(JS_GetRuntime(ctx), class_id, &this_class) == 0, "String class register failed", "register_class");
				auto class_name = _class_name();
				auto point_ctor = JS_NewCFunction2(ctx, constructor, class_name.data(), 2, JS_CFUNC_constructor, 0);
				auto proto = JS_NewObject(ctx);
				auto instance_c = JS_NewCFunction2(ctx, instance, "instance", 0, JS_CFUNC_generic, 0);
				auto atom = Atom{ctx, "instance"};
				JS_DefinePropertyValue(ctx, point_ctor, atom.value, instance_c, int{JS_PROP_C_W_E});
				JS_SetPropertyFunctionList(ctx, proto, proto_functions.data(), proto_functions.size());
				JS_SetConstructor(ctx, point_ctor, proto);
				auto global_obj = JS_GetGlobalObject(ctx);
				auto obj1 = make_instance_object(ctx, global_obj, "Sen");
				auto obj2 = make_instance_object(ctx, obj1, "Kernel");
				auto atom_data = Atom{ctx, class_name};
				JS_DefinePropertyValue(ctx, obj2, atom_data.value, point_ctor, int{JS_PROP_C_W_E});
				JS_FreeValue(ctx, global_obj);
				JS_FreeValue(ctx, obj1);
				JS_FreeValue(ctx, obj2);
				JS_FreeValue(ctx, proto);
				return;
			}

		}

		namespace BinaryView
		{

			struct Data
			{

				List<std::uint8_t> value;

				using It = decltype(value.begin());

				Data() = default;

				explicit Data(
					List<std::uint8_t> &&value
				) : value(std::move(value))
				{
				}

				explicit Data(
					It begin,
					It end) : value(begin, end)
				{
				}

				explicit Data(
					uint8_t *begin,
					uint8_t *end) : value(begin, end)
				{
				}

				~Data(

				) = default;
			};

			using Data = Data;

			inline static JSClassID class_id;

			inline static auto constexpr _class_name(

			) -> std::string_view
			{
				return "BinaryView";
			}

			inline static auto this_class = make_class_definition<Data>(
				_class_name(),
				class_id
			);

			inline static auto handle(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv,
				std::string_view method_name,
				std::function<JSValue(Data*)> method
			) -> JSValue {
				return proxy_wrapper(ctx, method_name, [&](){
					auto s = static_cast<Data*>(JS_GetOpaque2(ctx, this_val, class_id));
					if (s == nullptr) {
						return JS_EXCEPTION;
					}
					return method(s);
				});
			}

			inline static auto size(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::bigint
			{
				return handle(ctx, this_val, argc, argv, "size", [&](Data* s){
					return JS::Converter::to_bigint<uint64_t>(ctx, s->value.size());
				});
			}

			inline static auto capacity(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::bigint
			{
				return handle(ctx, this_val, argc, argv, "capacity", [&](Data* s){
					return JS::Converter::to_bigint<uint64_t>(ctx, s->value.capacity());
				});
			}

			inline static auto allocate(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "allocate", [&](Data* s){
					assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "allocate");
					s->value.reserve(static_cast<std::size_t>(JS::Converter::get_bigint64(ctx, argv[0])));
					return JS_UNDEFINED;
				});
			}

			inline static auto sub(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::ArrayBuffer
			{
				return handle(ctx, this_val, argc, argv, "sub", [&](Data* s){
					assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "sub");
					auto from = static_cast<std::size_t>(JS::Converter::get_bigint64(ctx, argv[0]));
					auto to = static_cast<std::size_t>(JS::Converter::get_bigint64(ctx, argv[1]));
					assert_conditional(from < to, fmt::format("sub failed because {} >= {}", from, to), "sub");
					assert_conditional(to < s->value.size(), fmt::format("sub failed because trying to reach outside bounds"), "sub");
					assert_conditional(from >= 0, fmt::format("from cannot smaller than zero. Got value: {}", from), "sub");
					return JS_NewArrayBufferCopy(ctx, s->value.data() + from, to);
				});
			}

			template <auto use_big_endian>
			inline static auto stream(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSDefine::DataStreamView
			{
				static_assert(use_big_endian == true || use_big_endian == false, "use_big_endian can only be true or false");
				auto m_function_name = std::string{DataStreamView::_class_name<use_big_endian>().data(), DataStreamView::_class_name<use_big_endian>().size()};
				return handle(ctx, this_val, argc, argv, m_function_name, [&](Data* s) {
					auto sub = std::make_unique<Definition::Buffer::Stream<use_big_endian>>(s->value);
					auto global_obj = JS_GetGlobalObject(ctx);
					auto atom_1 = Atom{ctx, "Sen"};
					auto sen_obj = JS_GetProperty(ctx, global_obj, atom_1.value);
					auto atom_2 = Atom{ctx, "Kernel"};
					auto kernel_obj = JS_GetProperty(ctx, sen_obj, atom_2.value);
					auto constructor_name = DataStreamView::_class_name<use_big_endian>();
					auto stream_atom = Atom{ctx, constructor_name};
					auto stream_ctor = JS_GetProperty(ctx, kernel_obj, stream_atom.value);
					auto prototype_atom = Atom{ctx, "prototype"};
					JS_FreeValue(ctx, global_obj);
					JS_FreeValue(ctx, sen_obj);
					JS_FreeValue(ctx, kernel_obj);
					JS_FreeValue(ctx, stream_ctor);
					auto proto = JS_GetProperty(ctx, stream_ctor, prototype_atom.value);
					if (JS_IsException(proto)) {
						throw Exception("not a constructor", std::source_location::current(), m_function_name);
					}
					auto obj = JS_NewObjectProtoClass(ctx, proto, DataStreamView::ClassID<use_big_endian>::value);
					JS_FreeValue(ctx, proto);
					if (JS_IsException(obj)) {
						throw Exception("can't define class", std::source_location::current(), m_function_name);
					}
					JS_SetOpaque(obj, sub.release());
					return obj;
				});
			}

			inline static auto constructor(
				JSContext *ctx,
				JSValueConst new_target,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return proxy_wrapper(ctx, "constructor", [&](){
					auto s = std::unique_ptr<Data, decltype(make_deleter<Data>())>(nullptr, make_deleter<Data>());
					auto obj = JS_UNDEFINED;
					auto proto = JSElement::Prototype{};
					if (argc == 1) {
						auto size = std::size_t{};
						auto data = JS_GetArrayBuffer(ctx, &size, argv[0]);
						if (data == nullptr) {
							return JS_EXCEPTION;
						}
						s.reset(new Data(make_list(data, size)));
					}
					else {
						JS_ThrowInternalError(ctx, "BinaryView cannot be initialized because the number of argument does not match. Expected: %d, got: %d", 1, argc);
						return JS_EXCEPTION;
					}
					auto atom = Atom{ctx, "prototype"};
					proto = JS_GetProperty(ctx, new_target, atom.value);
					if (JS_IsException(proto)) {
						goto fail;
					}
					obj = JS_NewObjectProtoClass(ctx, proto, class_id);
					JS_FreeValue(ctx, proto);
					if (JS_IsException(obj)) {
						goto fail;
					}
					JS_SetOpaque(obj, s.release());
					return obj;
				fail:
					JS_FreeValue(ctx, obj);
					return JS_EXCEPTION; });
			}

			inline static auto getter(
				JSContext *ctx,
				JSValueConst this_val,
				int magic
			) -> JSValue
			{
				return handle(ctx, this_val, 0, nullptr, "getter", [&](Data* s){
					return JS_NewArrayBufferCopy(ctx, s->value.data(), s->value.size());
				});
			}

			inline static auto setter(
				JSContext *ctx,
				JSValueConst this_val,
				JSValueConst val,
				int magic
			) -> JSValue
			{
				return handle(ctx, this_val, 0, nullptr, "setter", [&](Data* s){
					auto byteLength = std::size_t{};
					auto data = JS_GetArrayBuffer(ctx, &byteLength, val);
					if (data == nullptr)
					{
						return JS_EXCEPTION;
					}
					s->value.clear();
					s->value.assign(data, data + byteLength);
					return JS_UNDEFINED;
				});
			}


			inline static auto constexpr proto_functions = std::to_array<JSCFunctionListEntry>({
				JS_CPPGETSET_MAGIC_DEF("value", getter, setter, 0),
				JS_CPPFUNC_DEF("size", 0, size),
				JS_CPPFUNC_DEF("capacity", 0, capacity),
				JS_CPPFUNC_DEF("allocate", 1, allocate),
				JS_CPPFUNC_DEF("sub", 2, sub),
				JS_CPPFUNC_DEF("stream_view", 0, stream<false>),
				JS_CPPFUNC_DEF("big_stream_view", 0, stream<true>),
			});

			inline static auto instance(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSDefine::BinaryView
			{
				return proxy_wrapper(ctx, "instance", [&]() {
					auto s = std::make_unique<Data>();
					auto atom_proto = Atom{ctx, "prototype"};
					auto proto = JS_GetProperty(ctx, this_val, atom_proto.value);
					if (JS_IsException(proto)) {
						return JS_EXCEPTION;
					}
					auto obj = JS_NewObjectProtoClass(ctx, proto, class_id);
					JS_FreeValue(ctx, proto);
					if (JS_IsException(obj)) {
						return JS_EXCEPTION;
					}
					JS_SetOpaque(obj, s.release());
					return obj;
				});
			}

			inline static auto register_class(
				JSContext *ctx
			) -> void
			{
				JS_NewClassID(JS_GetRuntime(ctx), &class_id);
				assert_conditional(JS_NewClass(JS_GetRuntime(ctx), class_id, &this_class) == 0, "BinaryView class register failed", "register_class");
				auto class_name = "BinaryView"_sv;
				auto point_ctor = JS_NewCFunction2(ctx, constructor, class_name.data(), 2, JS_CFUNC_constructor, 0);
				auto proto = JS_NewObject(ctx);
				auto instance_c = JS_NewCFunction2(ctx, instance, "instance", 0, JS_CFUNC_generic, 0);
				auto atom = Atom(ctx, "instance");
				JS_DefinePropertyValue(ctx, point_ctor, atom.value, instance_c, int{JS_PROP_C_W_E});
				JS_SetPropertyFunctionList(ctx, proto, proto_functions.data(), proto_functions.size());
				JS_SetConstructor(ctx, point_ctor, proto);
				auto global_obj = JS_GetGlobalObject(ctx);
				auto obj1 = make_instance_object(ctx, global_obj, "Sen");
				auto obj2 = make_instance_object(ctx, obj1, "Kernel");
				auto atom_data = Atom(ctx, class_name);
				JS_DefinePropertyValue(ctx, obj2, atom_data.value, point_ctor, int{JS_PROP_C_W_E});
				JS_FreeValue(ctx, global_obj);
				JS_FreeValue(ctx, obj1);
				JS_FreeValue(ctx, obj2);
				JS_FreeValue(ctx, proto);
				return;
			}

		}

		namespace Canvas
		{

			using Data = canvas_ity::canvas;

			struct Detail
			{
				inline static JSClassID class_id;
			};

			inline static auto constructor(
				JSContext *ctx,
				JSValueConst new_target,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return proxy_wrapper(ctx, "constructor", [&]() {
					auto s = std::unique_ptr<Data, decltype(make_deleter<Data>())>(nullptr, make_deleter<Data>());
					auto obj = JS_UNDEFINED;
					auto proto = JSDefine::Canvas{};
					if (argc == 2) {
						s.reset(new Data(static_cast<int>(JS::Converter::get_bigint64(ctx, argv[0])), static_cast<int>(JS::Converter::get_bigint64(ctx, argv[1]))));
					}
					else {
						JS_ThrowInternalError(ctx, "Canvas class cannot be initialized because the number of argument does not match. Expected: %d, got: %d", 2, argc);
						return JS_EXCEPTION;
					}
					auto atom = Atom{ctx, "prototype"};
					proto = JS_GetProperty(ctx, new_target, atom.value);
					if (JS_IsException(proto)) {
						goto fail;
					}
					obj = JS_NewObjectProtoClass(ctx, proto, Detail::class_id);
					JS_FreeValue(ctx, proto);
					if (JS_IsException(obj)) {
						goto fail;
					}
					JS_SetOpaque(obj, s.release());
					return obj;
				fail:
					JS_FreeValue(ctx, obj);
					return JS_EXCEPTION;
				});
			}

			inline auto constexpr _class_name( 

			) -> std::string_view
			{
				return "Canvas";
			}

			inline static auto this_class = make_class_definition<Data>(
				_class_name(),
				Detail::class_id
			);

			inline static auto handle (
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv,
				std::string_view method_name,
				std::function<JSValue(Data*)> method
			) -> JSValue {
				return proxy_wrapper(ctx, method_name, [&](){
					auto s = static_cast<Data*>(JS_GetOpaque2(ctx, this_val, Detail::class_id));
					if (s == nullptr) {
						return JS_EXCEPTION;
					}
					return method(s);
				});
			}

			inline static auto scale (
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "scale", [&](Data* s) {
					assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "scale");
					s->scale(JS::Converter::get_float32(ctx, argv[0]), JS::Converter::get_float32(ctx, argv[1]));
					return JS_UNDEFINED;
				});
			}


			inline static auto rotate (
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "rotate", [&](Data* s) {
					assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "rotate");
					s->rotate(JS::Converter::get_float32(ctx, argv[0]));
					return JS_UNDEFINED;
				});
			}

			inline static auto translate (
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "translate", [&](Data* s) {
					assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "translate");
					s->translate(JS::Converter::get_float32(ctx, argv[0]), JS::Converter::get_float32(ctx, argv[1]));
					return JS_UNDEFINED;
				});
			}

			inline static auto transform (
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "transform", [&](Data* s) {
					assert_conditional(argc == 6, fmt::format("{} 6, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "transform");
					s->transform(JS::Converter::get_float32(ctx, argv[0]), JS::Converter::get_float32(ctx, argv[1]), JS::Converter::get_float32(ctx, argv[2]), JS::Converter::get_float32(ctx, argv[3]), JS::Converter::get_float32(ctx, argv[4]), JS::Converter::get_float32(ctx, argv[5]));
					return JS_UNDEFINED;
				});
			}

			inline static auto set_transform (
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "set_transform", [&](Data* s) {
					assert_conditional(argc == 6, fmt::format("{} 6, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "set_transform");
					s->set_transform(JS::Converter::get_float32(ctx, argv[0]), JS::Converter::get_float32(ctx, argv[1]), JS::Converter::get_float32(ctx, argv[2]), JS::Converter::get_float32(ctx, argv[3]), JS::Converter::get_float32(ctx, argv[4]), JS::Converter::get_float32(ctx, argv[5]));
					return JS_UNDEFINED;
				});
			}

			inline static auto set_global_alpha (
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "set_global_alpha", [&](Data* s) {
					assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "set_global_alpha");
					s->set_global_alpha(JS::Converter::get_float32(ctx, argv[0]));
					return JS_UNDEFINED;
				});
			}

			inline static auto set_shadow_color (
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "set_shadow_color", [&](Data* s) {
					assert_conditional(argc == 4, fmt::format("{} 4, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "set_shadow_color");
					s->set_shadow_color(JS::Converter::get_float32(ctx, argv[0]), JS::Converter::get_float32(ctx, argv[1]), JS::Converter::get_float32(ctx, argv[2]), JS::Converter::get_float32(ctx, argv[3]));
					return JS_UNDEFINED;
				});
			}

			inline static auto set_line_width (
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "set_line_width", [&](Data* s) {
					assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "set_line_width");
					s->set_line_width(JS::Converter::get_float32(ctx, argv[0]));
					return JS_UNDEFINED;
				});
			}

			inline static auto set_shadow_blur (
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "set_shadow_blur", [&](Data* s) {
					assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "set_shadow_blur");
					s->set_global_alpha(JS::Converter::get_float32(ctx, argv[0]));
					return JS_UNDEFINED;
				});
			}

			inline static auto set_miter_limit (
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "set_miter_limit", [&](Data* s) {
					assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "set_miter_limit");
					s->set_miter_limit(JS::Converter::get_float32(ctx, argv[0]));
					return JS_UNDEFINED;
				});
			}

			using brush_type = canvas_ity::brush_type;

			inline static auto set_color (
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "set_color", [&](Data* s) {
					assert_conditional(argc == 5, fmt::format("{} 5, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "set_color");
					s->set_color(static_cast<brush_type>(JS::Converter::get_bigint64(ctx, argv[0])), JS::Converter::get_float32(ctx, argv[1]), JS::Converter::get_float32(ctx, argv[2]), JS::Converter::get_float32(ctx, argv[3]), JS::Converter::get_float32(ctx, argv[4]));
					return JS_UNDEFINED;
				});
			}

			inline static auto set_linear_gradient (
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "set_linear_gradient", [&](Data* s) {
					assert_conditional(argc == 5, fmt::format("{} 5, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "set_linear_gradient");
					s->set_linear_gradient(static_cast<brush_type>(JS::Converter::get_bigint64(ctx, argv[0])), JS::Converter::get_float32(ctx, argv[1]), JS::Converter::get_float32(ctx, argv[2]), JS::Converter::get_float32(ctx, argv[3]), JS::Converter::get_float32(ctx, argv[4]));
					return JS_UNDEFINED;
				});
			}

			inline static auto set_radial_gradient (
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "set_radial_gradient", [&](Data* s) {
					assert_conditional(argc == 7, fmt::format("{} 7, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "set_radial_gradient");
					s->set_radial_gradient(static_cast<brush_type>(JS::Converter::get_bigint64(ctx, argv[0])), JS::Converter::get_float32(ctx, argv[1]), JS::Converter::get_float32(ctx, argv[2]), JS::Converter::get_float32(ctx, argv[3]), JS::Converter::get_float32(ctx, argv[4]), JS::Converter::get_float32(ctx, argv[5]), JS::Converter::get_float32(ctx, argv[6]));
					return JS_UNDEFINED;
				});
			}

			inline static auto add_color_stop (
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "add_color_stop", [&](Data* s) {
					assert_conditional(argc == 6, fmt::format("{} 6, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "add_color_stop");
					s->add_color_stop(static_cast<brush_type>(JS::Converter::get_bigint64(ctx, argv[0])), JS::Converter::get_float32(ctx, argv[1]), JS::Converter::get_float32(ctx, argv[2]), JS::Converter::get_float32(ctx, argv[3]), JS::Converter::get_float32(ctx, argv[4]), JS::Converter::get_float32(ctx, argv[5]));
					return JS_UNDEFINED;
				});
			}

			inline static auto begin_path (
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "begin_path", [&](Data* s) {
					s->begin_path();
					return JS_UNDEFINED;
				});
			}

			inline static auto move_to (
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "move_to", [&](Data* s) {
					assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "move_to");
					s->move_to(JS::Converter::get_float32(ctx, argv[0]), JS::Converter::get_float32(ctx, argv[1]));
					return JS_UNDEFINED;
				});
			}

			inline static auto close_path (
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "close_path", [&](Data* s) {
					s->close_path();
					return JS_UNDEFINED;
				});
			}

			inline static auto line_to (
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "line_to", [&](Data* s) {
					assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "line_to");
					s->line_to(JS::Converter::get_float32(ctx, argv[0]), JS::Converter::get_float32(ctx, argv[1]));
					return JS_UNDEFINED;
				});
			}

			inline static auto quadratic_curve_to (
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "quadratic_curve_to", [&](Data* s) {
					assert_conditional(argc == 4, fmt::format("{} 4, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "quadratic_curve_to");
					s->quadratic_curve_to(JS::Converter::get_float32(ctx, argv[0]), JS::Converter::get_float32(ctx, argv[1]), JS::Converter::get_float32(ctx, argv[2]), JS::Converter::get_float32(ctx, argv[3]));
					return JS_UNDEFINED;
				});
			}

			inline static auto bezier_curve_to (
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "bezier_curve_to", [&](Data* s) {
					assert_conditional(argc == 6, fmt::format("{} 6, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "bezier_curve_to");
					s->bezier_curve_to(JS::Converter::get_float32(ctx, argv[0]), JS::Converter::get_float32(ctx, argv[1]), JS::Converter::get_float32(ctx, argv[2]), JS::Converter::get_float32(ctx, argv[3]), JS::Converter::get_float32(ctx, argv[4]), JS::Converter::get_float32(ctx, argv[5]));
					return JS_UNDEFINED;
				});
			}

			inline static auto arc_to (
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "arc_to", [&](Data* s) {
					assert_conditional(argc == 5, fmt::format("{} 5, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "arc_to");
					s->arc_to(JS::Converter::get_float32(ctx, argv[0]), JS::Converter::get_float32(ctx, argv[1]), JS::Converter::get_float32(ctx, argv[2]), JS::Converter::get_float32(ctx, argv[3]), JS::Converter::get_float32(ctx, argv[4]));
					return JS_UNDEFINED;
				});
			}

			inline static auto set_image_color (
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "set_image_color", [&](Data* s) {
					assert_conditional(argc == 4, fmt::format("{} 4, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "set_image_color");
					s->set_image_color(JS::Converter::get_float32(ctx, argv[0]), JS::Converter::get_float32(ctx, argv[1]), JS::Converter::get_float32(ctx, argv[2]), JS::Converter::get_float32(ctx, argv[3]));
					return JS_UNDEFINED; 
				});
			}

			inline static auto arc (
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "arc", [&](Data* s) {
					assert_conditional(argc == 6, fmt::format("{} 6, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "arc");
					s->arc(JS::Converter::get_float32(ctx, argv[0]), JS::Converter::get_float32(ctx, argv[1]), JS::Converter::get_float32(ctx, argv[2]), JS::Converter::get_float32(ctx, argv[3]), JS::Converter::get_float32(ctx, argv[4]), JS::Converter::get_bool(ctx, argv[5]));
					return JS_UNDEFINED;
				});
			}

			inline static auto rectangle (
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "rectangle", [&](Data* s) {
					assert_conditional(argc == 4, fmt::format("{} 4, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "rectangle");
					s->rectangle(JS::Converter::get_float32(ctx, argv[0]), JS::Converter::get_float32(ctx, argv[1]), JS::Converter::get_float32(ctx, argv[2]), JS::Converter::get_float32(ctx, argv[3]));
					return JS_UNDEFINED;
				});
			}

			inline static auto fill (
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "fill", [&](Data* s) {
					s->fill();
					return JS_UNDEFINED;
				});
			}

			inline static auto stroke (
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "stroke", [&](Data* s) {
					s->stroke();
					return JS_UNDEFINED;
				});
			}

			inline static auto clip (
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "clip", [&](Data* s) {
					s->clip();
					return JS_UNDEFINED;
				});
			}

			inline static auto is_point_in_path (
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::boolean
			{
				return handle(ctx, this_val, argc, argv, "is_point_in_path", [&](Data* s) {
					assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "is_point_in_path");
					return JS::Converter::to_bool(ctx, s->is_point_in_path(JS::Converter::get_float32(ctx, argv[0]), JS::Converter::get_float32(ctx, argv[1])));
				});
			}

			inline static auto clear_rectangle (
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "clear_rectangle", [&](Data* s) {
					assert_conditional(argc == 4, fmt::format("{} 4, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "clear_rectangle");
					s->clear_rectangle(JS::Converter::get_float32(ctx, argv[0]), JS::Converter::get_float32(ctx, argv[1]), JS::Converter::get_float32(ctx, argv[2]), JS::Converter::get_float32(ctx, argv[3]));
					return JS_UNDEFINED;
				});
			}

			inline static auto fill_rectangle(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "fill_rectangle", [&](Data* s) {
					assert_conditional(argc == 4, fmt::format("{} 4, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "fill_rectangle");
					s->fill_rectangle(JS::Converter::get_float32(ctx, argv[0]), JS::Converter::get_float32(ctx, argv[1]), JS::Converter::get_float32(ctx, argv[2]), JS::Converter::get_float32(ctx, argv[3]));
					return JS_UNDEFINED;
				});
			}

			inline static auto stroke_rectangle (
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "stroke_rectangle", [&](Data* s) {
					assert_conditional(argc == 4, fmt::format("{} 4, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "stroke_rectangle");
					s->stroke_rectangle(JS::Converter::get_float32(ctx, argv[0]), JS::Converter::get_float32(ctx, argv[1]), JS::Converter::get_float32(ctx, argv[2]), JS::Converter::get_float32(ctx, argv[3]));
					return JS_UNDEFINED;
				});
			}

			inline static auto set_font (
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::boolean
			{
				return handle(ctx, this_val, argc, argv, "set_font", [&](Data* s) {
					assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "set_font");
					auto size = std::size_t{};
					auto data = JS_GetArrayBuffer(ctx, &size, argv[0]);
					return JS::Converter::to_bool(ctx, s->set_font(data, static_cast<int>(JS::Converter::get_bigint64(ctx, argv[1])), size));
				});
			}

			inline static auto draw_image (
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "draw_image", [&](Data* s) {
					assert_conditional(argc == 8, fmt::format("{} 8, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "draw_image");
					auto size = std::size_t{};
					auto data = JS_GetArrayBuffer(ctx, &size, argv[0]);
					s->draw_image(data, static_cast<int>(JS::Converter::get_bigint64(ctx, argv[1])), static_cast<int>(JS::Converter::get_bigint64(ctx, argv[2])), static_cast<int>(JS::Converter::get_bigint64(ctx, argv[3])), static_cast<float>(JS::Converter::get_float32(ctx, argv[4])), static_cast<float>(JS::Converter::get_float32(ctx, argv[5])), static_cast<float>(JS::Converter::get_float32(ctx, argv[6])), static_cast<float>(JS::Converter::get_float32(ctx, argv[7])));
					return JS_UNDEFINED;
				});
			}

			inline static auto get_image_data (
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "get_image_data", [&](Data* s) {
					assert_conditional(argc == 6, fmt::format("{} 6, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "get_image_data");
					auto size = std::size_t{};
					auto data = JS_GetArrayBuffer(ctx, &size, argv[0]);
					if (data == nullptr) {
						return JS_EXCEPTION;
					}
					s->get_image_data(data, static_cast<int>(JS::Converter::get_bigint64(ctx, argv[1])), static_cast<int>(JS::Converter::get_bigint64(ctx, argv[2])), static_cast<int>(JS::Converter::get_bigint64(ctx, argv[3])), static_cast<int>(JS::Converter::get_bigint64(ctx, argv[4])), static_cast<int>(JS::Converter::get_bigint64(ctx, argv[5])));
					return JS_UNDEFINED;
				});
			}

			inline static auto put_image_data (
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "put_image_data", [&](Data* s) {
					assert_conditional(argc == 6, fmt::format("{} 6, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "put_image_data");
					auto size = std::size_t{};
					auto data = JS_GetArrayBuffer(ctx, &size, argv[0]);
					s->put_image_data(data, static_cast<int>(JS::Converter::get_bigint64(ctx, argv[1])), static_cast<int>(JS::Converter::get_bigint64(ctx, argv[2])), static_cast<int>(JS::Converter::get_bigint64(ctx, argv[3])), static_cast<int>(JS::Converter::get_bigint64(ctx, argv[4])), static_cast<int>(JS::Converter::get_bigint64(ctx, argv[5])));
					return JS_UNDEFINED;
				});
			}

			inline static auto save (
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "save", [&](Data* s) {
					s->save();
					return JS_UNDEFINED;
				});
			}

			inline static auto restore (
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, argc, argv, "restore", [&](Data* s) {
					s->restore();
					return JS_UNDEFINED;
				});
			}

			inline static auto constexpr proto_functions = std::to_array<JSCFunctionListEntry>({
				JS_CPPFUNC_DEF("scale", 2, scale),
				JS_CPPFUNC_DEF("rotate", 1, rotate),
				JS_CPPFUNC_DEF("translate", 2, translate),
				JS_CPPFUNC_DEF("transform", 2, transform),
				JS_CPPFUNC_DEF("set_transform", 2, set_transform),
				JS_CPPFUNC_DEF("set_global_alpha", 1, set_global_alpha),
				JS_CPPFUNC_DEF("set_shadow_color", 2, set_shadow_color),
				JS_CPPFUNC_DEF("set_shadow_blur", 1, set_shadow_blur),
				JS_CPPFUNC_DEF("set_line_width", 1, set_line_width),
				JS_CPPFUNC_DEF("set_miter_limit", 1, set_miter_limit),
				JS_CPPFUNC_DEF("set_linear_gradient", 2, set_linear_gradient),
				JS_CPPFUNC_DEF("set_color", 2, set_color),
				JS_CPPFUNC_DEF("set_radial_gradient", 2, set_radial_gradient),
				JS_CPPFUNC_DEF("add_color_stop", 2, add_color_stop),
				JS_CPPFUNC_DEF("begin_path", 0, begin_path),
				JS_CPPFUNC_DEF("close_path", 0, close_path),
				JS_CPPFUNC_DEF("move_to", 2, move_to),
				JS_CPPFUNC_DEF("line_to", 2, line_to),
				JS_CPPFUNC_DEF("quadratic_curve_to", 2, quadratic_curve_to),
				JS_CPPFUNC_DEF("bezier_curve_to", 2, bezier_curve_to),
				JS_CPPFUNC_DEF("arc_to", 2, arc_to),
				JS_CPPFUNC_DEF("arc", 2, arc),
				JS_CPPFUNC_DEF("rectangle", 2, rectangle),
				JS_CPPFUNC_DEF("fill", 0, fill),
				JS_CPPFUNC_DEF("stroke", 0, stroke),
				JS_CPPFUNC_DEF("clip", 0, clip),
				JS_CPPFUNC_DEF("is_point_in_path", 1, is_point_in_path),
				JS_CPPFUNC_DEF("clear_rectangle", 2, clear_rectangle),
				JS_CPPFUNC_DEF("fill_rectangle", 2, fill_rectangle),
				JS_CPPFUNC_DEF("stroke_rectangle", 2, stroke_rectangle),
				JS_CPPFUNC_DEF("set_font", 2, set_font),
				JS_CPPFUNC_DEF("draw_image", 2, draw_image),
				JS_CPPFUNC_DEF("get_image_data", 2, get_image_data),
				JS_CPPFUNC_DEF("put_image_data", 2, put_image_data),
				JS_CPPFUNC_DEF("save", 0, save),
				JS_CPPFUNC_DEF("restore", 0, restore),
				JS_CPPFUNC_DEF("set_image_color", 2, set_image_color),
			});

			inline static auto register_class(
				JSContext *ctx
			) -> void
			{
				JS_NewClassID(JS_GetRuntime(ctx), &(Detail::class_id));
				assert_conditional(JS_NewClass(JS_GetRuntime(ctx), Detail::class_id, &this_class) == 0, "Canvas class register failed", "register_class");
				auto class_name = _class_name();
				auto point_ctor = JS_NewCFunction2(ctx, constructor, class_name.data(), 2, JS_CFUNC_constructor, 0);
				auto proto = JS_NewObject(ctx);
				JS_SetPropertyFunctionList(ctx, proto, proto_functions.data(), proto_functions.size());
				JS_SetConstructor(ctx, point_ctor, proto);
				auto global_obj = JS_GetGlobalObject(ctx);
				auto obj1 = make_instance_object(ctx, global_obj, "Sen");
				auto obj2 = make_instance_object(ctx, obj1, "Kernel");
				auto atom = Atom{ctx, class_name};
				JS_DefinePropertyValue(ctx, obj2, atom.value, point_ctor, int{JS_PROP_C_W_E});
				JS_FreeValue(ctx, global_obj);
				JS_FreeValue(ctx, obj1);
				JS_FreeValue(ctx, obj2);
				JS_FreeValue(ctx, proto);
				return;
			}

		}

		namespace DimensionView
		{

			using Data = Kernel::Definition::Dimension<int>;

			inline static JSClassID class_id;

			enum Magic
			{
				width,
				height,
			};

			inline static auto constructor(
				JSContext *ctx,
				JSValueConst new_target,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return proxy_wrapper(ctx, "constructor", [&](){
					auto s = std::unique_ptr<Data, decltype(make_deleter<Data>())>(nullptr, make_deleter<Data>());
					auto obj = JS_UNDEFINED;
					auto proto = JSDefine::ImageView{};
					if (argc == 1) {
						auto width_atom = Atom{ctx, "width"};
						auto height_atom = Atom{ctx, "height"};
						auto width = get_property_bigint64(ctx, argv[0], width_atom.value);
						auto height = get_property_bigint64(ctx, argv[0], height_atom.value);
						s.reset(new Data(static_cast<int>(width),static_cast<int>(height)));
					}
					else {
						JS_ThrowInternalError(ctx, "DimensionView cannot be initialized because the number of argument does not match. Expected: %d, got: %d", 1, argc);
						return JS_EXCEPTION;
					}
					auto atom = Atom{ctx, "prototype"};
					proto = JS_GetProperty(ctx, new_target, atom.value);
					if (JS_IsException(proto)) {
						goto fail;
					}
					obj = JS_NewObjectProtoClass(ctx, proto, class_id);
					JS_FreeValue(ctx, proto);
					if (JS_IsException(obj)) {
						goto fail;
					}
					JS_SetOpaque(obj, s.release());
					return obj;
				fail:
					JS_FreeValue(ctx, obj);
					return JS_EXCEPTION;
				});
			}

			inline auto constexpr _class_name( 

			) -> std::string_view
			{
				return "DimensionView";
			}

			inline static auto this_class = make_class_definition<Data>(
				_class_name(),
				class_id
			);

			inline static auto handle (
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv,
				std::string_view method_name,
				std::function<JSValue(Data*)> method
			) -> JSValue {
				return proxy_wrapper(ctx, method_name, [&](){
					auto s = static_cast<Data*>(JS_GetOpaque2(ctx, this_val, class_id));
					if (s == nullptr) {
						return JS_EXCEPTION;
					}
					return method(s);
				});
			}

			inline static auto getter(
				JSContext *ctx,
				JSValueConst this_val,
				int magic
			) -> JSElement::any
			{
				return handle(ctx, this_val, 0, nullptr, "getter", [&](Data* s){
					switch (static_cast<DimensionView::Magic>(magic))
					{
						case DimensionView::Magic::width:
						{
							return JS::Converter::to_bigint(ctx, static_cast<std::int64_t>(s->width));
						}
						case DimensionView::Magic::height:
						{
							return JS::Converter::to_bigint(ctx, static_cast<std::int64_t>(s->height));
						}
						default:
						{
							assert_conditional(false, fmt::format("Cannot call getter method on magic {}", magic), "getter");
						}
					}
				});
			}

			inline static auto setter(
				JSContext *ctx,
				JSValueConst this_val,
				JSValueConst val,
				int magic
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, 0, nullptr, "setter", [&](Data* s){
					switch (static_cast<DimensionView::Magic>(magic))
					{
						case DimensionView::Magic::width:
						{
							s->width = static_cast<int>(JS::Converter::get_bigint64(ctx, val));
							break;
						}
						case DimensionView::Magic::height:
						{
							s->height = static_cast<int>(JS::Converter::get_bigint64(ctx, val));
							break;
						}
						default:
						{
							assert_conditional(false, fmt::format("Cannot call setter method on magic {}", magic), "setter");
						}
					}
					return JS_UNDEFINED;
				});
			}

			inline static auto constexpr proto_functions = std::to_array<JSCFunctionListEntry>({
				JS_CPPGETSET_MAGIC_DEF("width", getter, setter, DimensionView::Magic::width),
				JS_CPPGETSET_MAGIC_DEF("height", getter, setter, DimensionView::Magic::height),
			});

			inline static auto register_class(
				JSContext *ctx
			) -> void
			{
				JS_NewClassID(JS_GetRuntime(ctx), &class_id);
				assert_conditional(JS_NewClass(JS_GetRuntime(ctx), class_id, &this_class) == 0, "DimensionView class failed register", "register_class");
				auto class_name = _class_name();
				auto point_ctor = JS_NewCFunction2(ctx, constructor, class_name.data(), 2, JS_CFUNC_constructor, 0);
				auto proto = JS_NewObject(ctx);
				JS_SetPropertyFunctionList(ctx, proto, proto_functions.data(), proto_functions.size());
				JS_SetConstructor(ctx, point_ctor, proto);
				auto global_obj = JS_GetGlobalObject(ctx);
				auto obj1 = make_instance_object(ctx, global_obj, "Sen");
				auto obj2 = make_instance_object(ctx, obj1, "Kernel");
				auto atom = Atom(ctx, class_name);
				JS_DefinePropertyValue(ctx, obj2, atom.value, point_ctor, int{JS_PROP_C_W_E});
				JS_FreeValue(ctx, global_obj);
				JS_FreeValue(ctx, obj1);
				JS_FreeValue(ctx, obj2);
				JS_FreeValue(ctx, proto);
				return;
			}

		}

		namespace Rectangle
		{

			using Data = Kernel::Definition::Rectangle<int>;

			inline static JSClassID class_id;

			enum Magic
			{
				x,
				y,
				width,
				height,
			};

			inline static auto constructor(
				JSContext *ctx,
				JSValueConst new_target,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return proxy_wrapper(ctx, "constructor", [&]() {
					auto s = std::unique_ptr<Data, decltype(make_deleter<Data>())>(nullptr, make_deleter<Data>());
					auto obj = JS_UNDEFINED;
					auto proto = JSDefine::ImageView{};
					if (argc == 1) {
						auto width_atom = Atom{ctx, "width"};
						auto height_atom = Atom{ctx, "height"};
						auto x_atom = Atom{ctx, "x"};
						auto y_atom = Atom{ctx, "y"};
						auto width = get_property_bigint64(ctx, argv[0], width_atom.value);
						auto height = get_property_bigint64(ctx, argv[0], height_atom.value);
						auto x = get_property_bigint64(ctx, argv[0], x_atom.value);
						auto y = get_property_bigint64(ctx, argv[0], y_atom.value);
						s.reset(new Data{static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height)});
					}
					else {
						JS_ThrowInternalError(ctx, "Rectangle cannot be initialized because the number of argument does not match. Expected: %d, got: %d", 1, argc);
						return JS_EXCEPTION;
					}
					auto atom = Atom{ctx, "prototype"};
					proto = JS_GetProperty(ctx, new_target, atom.value);
					if (JS_IsException(proto)) {
						goto fail;
					}
					obj = JS_NewObjectProtoClass(ctx, proto, class_id);
					JS_FreeValue(ctx, proto);
					if (JS_IsException(obj)) {
						goto fail;
					}
					JS_SetOpaque(obj, s.release());
					return obj;
				fail:
					JS_FreeValue(ctx, obj);
					return JS_EXCEPTION; 
				});
			}

			inline auto constexpr _class_name( 

			) -> std::string_view
			{
				return "Rectangle";
			}

			inline static auto this_class = make_class_definition<Data>(
				_class_name(),
				class_id
			);

			inline static auto handle (
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv,
				std::string_view method_name,
				std::function<JSValue(Data*)> method
			) -> JSValue {
				return proxy_wrapper(ctx, method_name, [&](){
					auto s = static_cast<Data*>(JS_GetOpaque2(ctx, this_val, class_id));
					if (s == nullptr) {
						return JS_EXCEPTION;
					}
					return method(s);
				});
			}

			inline static auto getter(
				JSContext *ctx,
				JSValueConst this_val,
				int magic
			) -> JSElement::any
			{
				return handle(ctx, this_val, 0, nullptr, "getter", [&](Data* s) {
					switch (static_cast<Rectangle::Magic>(magic))
					{
						case Rectangle::Magic::x:
						{
							return JS::Converter::to_bigint(ctx, static_cast<std::int64_t>(s->x));
						}
						case Rectangle::Magic::y:
						{
							return JS::Converter::to_bigint(ctx, static_cast<std::int64_t>(s->y));
						}
						case Rectangle::Magic::width:
						{
							return JS::Converter::to_bigint(ctx, static_cast<std::int64_t>(s->width));
						}
						case Rectangle::Magic::height:
						{
							return JS::Converter::to_bigint(ctx, static_cast<std::int64_t>(s->height));
						}
						default:
						{
							assert_conditional(false, fmt::format("Cannot call getter method on magic {}", magic), "getter");
						}
					}
				});
			}

			inline static auto setter(
				JSContext *ctx,
				JSValueConst this_val,
				JSValueConst val,
				int magic
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, 0, nullptr, "getter", [&](Data* s) {
					switch (static_cast<Rectangle::Magic>(magic))
					{
						case Rectangle::Magic::x:
					{
						s->x = static_cast<int>(JS::Converter::get_bigint64(ctx, val));
						break;
					}
					case Rectangle::Magic::y:
					{
						s->y = static_cast<int>(JS::Converter::get_bigint64(ctx, val));
						break;
					}
					case Rectangle::Magic::width:
					{
						s->width = static_cast<int>(JS::Converter::get_bigint64(ctx, val));
						break;
					}
					case Rectangle::Magic::height:
					{
						s->height = static_cast<int>(JS::Converter::get_bigint64(ctx, val));
						break;
					}
					default:
					{
						assert_conditional(false, fmt::format("Cannot call setter method on magic {}", magic), "setter");
					}
					}
					return JS_UNDEFINED;
				});
			}

			inline static auto constexpr proto_functions = std::to_array<JSCFunctionListEntry>({
				JS_CPPGETSET_MAGIC_DEF("x", getter, setter, Rectangle::Magic::x),
				JS_CPPGETSET_MAGIC_DEF("y", getter, setter, Rectangle::Magic::y),
				JS_CPPGETSET_MAGIC_DEF("width", getter, setter, Rectangle::Magic::width),
				JS_CPPGETSET_MAGIC_DEF("height", getter, setter, Rectangle::Magic::height),
			});

			inline static auto register_class(
				JSContext *ctx
			) -> void
			{
				JS_NewClassID(JS_GetRuntime(ctx), &class_id);
				assert_conditional(JS_NewClass(JS_GetRuntime(ctx), class_id, &this_class) == 0, "Rectangle class register failed", "register_class");
				auto class_name = "Rectangle"_sv;
				auto point_ctor = JS_NewCFunction2(ctx, constructor, class_name.data(), 2, JS_CFUNC_constructor, 0);
				auto proto = JS_NewObject(ctx);
				JS_SetPropertyFunctionList(ctx, proto, proto_functions.data(), proto_functions.size());
				JS_SetConstructor(ctx, point_ctor, proto);
				auto global_obj = JS_GetGlobalObject(ctx);
				auto obj1 = make_instance_object(ctx, global_obj, "Sen");
				auto obj2 = make_instance_object(ctx, obj1, "Kernel");
				auto atom = Atom{ctx, class_name};
				JS_DefinePropertyValue(ctx, obj2, atom.value, point_ctor, int{JS_PROP_C_W_E});
				JS_FreeValue(ctx, global_obj);
				JS_FreeValue(ctx, obj1);
				JS_FreeValue(ctx, obj2);
				JS_FreeValue(ctx, proto);
				return;
			}

		}

		namespace ImageView
		{

			using Data = Kernel::Definition::Image<int>;

			inline static JSClassID class_id;

			enum Magic
			{
				bit_depth,
				color_type,
				interlace_type,
				channels,
				rowbytes,
				data,
				width,
				height,
			};

			inline static auto constructor(
				JSContext *ctx,
				JSValueConst new_target,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return proxy_wrapper(ctx, "constructor", [&](){
					auto s = std::unique_ptr<Data, decltype(make_deleter<Data>())>(nullptr, make_deleter<Data>());
					auto obj = JS_UNDEFINED;
					auto proto = JSDefine::ImageView{};
					if (argc == 1) {
						auto width_atom = Atom{ctx, "width"};
						auto height_atom = Atom{ctx, "height"};
						auto bit_depth_atom = Atom{ctx, "bit_depth"};
						auto color_type_atom = Atom{ctx, "color_type"};
						auto interlace_type_atom = Atom{ctx, "interlace_type"};
						auto channels_atom = Atom{ctx, "channels"};
						auto rowbytes_atom = Atom{ctx, "rowbytes"};
						auto data_atom = Atom{ctx, "data"};
						auto width = get_property_bigint64(ctx, argv[0], width_atom.value);
						auto height = get_property_bigint64(ctx, argv[0], height_atom.value);
						auto bit_depth = get_property_bigint64(ctx, argv[0], bit_depth_atom.value);
						auto color_type = get_property_bigint64(ctx, argv[0], color_type_atom.value);
						auto interlace_type = get_property_bigint64(ctx, argv[0], interlace_type_atom.value);
						auto channels = get_property_bigint64(ctx, argv[0], channels_atom.value);
						auto rowbytes = get_property_bigint64(ctx, argv[0], rowbytes_atom.value);
						auto data_val = JS_GetProperty(ctx, argv[0], data_atom.value);
						if (JS_IsException(data_val)) {
							return JS_EXCEPTION;
						}
						auto size = std::size_t{};
						auto data = JS_GetArrayBuffer(ctx, &size, data_val);
						JS_FreeValue(ctx, data_val);
						assert_conditional(data != nullptr, "Cannot get ArrayBuffer from current object", "constructor");
						s.reset(new Data{static_cast<int>(width), static_cast<int>(height), static_cast<int>(bit_depth), static_cast<int>(color_type), static_cast<int>(interlace_type), static_cast<int>(channels), static_cast<int>(rowbytes), std::move(List<uint8_t>(data, data + size))});
					}
					else {
						JS_ThrowInternalError(ctx, "ImageView cannot be initialized because the number of argument does not valid. Expected: %d, got: %d", 1, argc);
						return JS_EXCEPTION;
					}
					auto atom = Atom{ctx, "prototype"};
					proto = JS_GetProperty(ctx, new_target, atom.value);
					if (JS_IsException(proto)) {
						goto fail;
					}
					obj = JS_NewObjectProtoClass(ctx, proto, class_id);
					JS_FreeValue(ctx, proto);
					if (JS_IsException(obj)) {
						goto fail;
					}
					JS_SetOpaque(obj, s.release());
					return obj;
				fail:
					JS_FreeValue(ctx, obj);
					return JS_EXCEPTION;
				});
			}

			inline auto constexpr _class_name( 

			) -> std::string_view
			{
				return "ImageView";
			}

			inline static auto this_class = make_class_definition<Data>(
				_class_name(),
				class_id
			);

			inline static auto handle (
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv,
				std::string_view method_name,
				std::function<JSValue(Data*)> method
			) -> JSValue {
				return proxy_wrapper(ctx, method_name, [&](){
					auto s = static_cast<Data*>(JS_GetOpaque2(ctx, this_val, class_id));
					if (s == nullptr) {
						return JS_EXCEPTION;
					}
					return method(s);
				});
			}

			inline static auto getter(
				JSContext *ctx,
				JSValueConst this_val,
				int magic
			) -> JSElement::any
			{
				return handle(ctx, this_val, 0, nullptr, "getter", [&](Data *s) {
					switch (static_cast<ImageView::Magic>(magic))
					{
						case ImageView::Magic::bit_depth:
						{
							return JS::Converter::to_bigint(ctx, static_cast<std::int64_t>(s->bit_depth));
						}
						case ImageView::Magic::channels:
						{
							return JS::Converter::to_bigint(ctx, static_cast<std::int64_t>(s->channels));
						}
						case ImageView::Magic::color_type:
						{
							return JS::Converter::to_bigint(ctx, static_cast<std::int64_t>(s->color_type));
						}
						case ImageView::Magic::interlace_type:
						{
							return JS::Converter::to_bigint(ctx, static_cast<std::int64_t>(s->interlace_type));
						}
						case ImageView::Magic::rowbytes:
						{
							return JS::Converter::to_bigint(ctx, static_cast<std::int64_t>(s->rowbytes));
						}
						case ImageView::Magic::width:
						{
							return JS::Converter::to_bigint(ctx, static_cast<std::int64_t>(s->width));
						}
						case ImageView::Magic::height:
						{
							return JS::Converter::to_bigint(ctx, static_cast<std::int64_t>(s->height));
						}
						case ImageView::Magic::data:
						{
							return JS_NewArrayBufferCopy(ctx, s->data().data(), s->data().size());
						}
						default:
						{
							assert_conditional(false, fmt::format("Cannot call getter method on magic {}", magic), "getter");
						}
					}
				});
			}

			inline static auto setter(
				JSContext *ctx,
				JSValueConst this_val,
				JSValueConst val,
				int magic
			) -> JSElement::undefined
			{
				return handle(ctx, this_val, 0, nullptr, "getter", [&](Data *s) {
					switch (static_cast<ImageView::Magic>(magic))
					{
						case ImageView::Magic::bit_depth:
						{
							s->bit_depth = static_cast<int>(JS::Converter::get_bigint64(ctx, val));
							break;
						}
						case ImageView::Magic::channels:
						{
							s->channels = static_cast<int>(JS::Converter::get_bigint64(ctx, val));
							break;
						}
						case ImageView::Magic::color_type:
						{
							s->color_type = static_cast<int>(JS::Converter::get_bigint64(ctx, val));
							break;
						}
						case ImageView::Magic::interlace_type:
						{
							s->interlace_type = static_cast<int>(JS::Converter::get_bigint64(ctx, val));
							break;
						}
						case ImageView::Magic::rowbytes:
						{
							s->rowbytes = static_cast<int>(JS::Converter::get_bigint64(ctx, val));
							break;
						}
						case ImageView::Magic::width:
						{
							s->width = static_cast<int>(JS::Converter::get_bigint64(ctx, val));
							break;
						}
						case ImageView::Magic::height:
						{
							s->height = static_cast<int>(JS::Converter::get_bigint64(ctx, val));
							break;
						}
						case ImageView::Magic::data:
						{
							auto size = std::size_t{};
							auto data = JS_GetArrayBuffer(ctx, &size, val);
							if (data == nullptr)
							{
								throw Exception(fmt::format("{}", Kernel::Language::get("js.converter.failed_to_get_js_array_buffer")), std::source_location::current(), "random");
							}
							s->set_data(make_list(data, size));
							break;
						}
						default:
						{
							assert_conditional(false, fmt::format("Cannot call setter method on magic {}", magic), "setter");
						}
					}
					return JS_UNDEFINED;
				});
			}

			inline static auto area(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::bigint
			{
				return handle(ctx, this_val, argc, argv, "area", [&](Data* s) {
					return JS::Converter::to_bigint<uint64_t>(ctx, s->area());
				});
			}

			inline static auto circumference(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::bigint
			{
				return handle(ctx, this_val, argc, argv, "circumference", [&](Data* s) {
					return JS::Converter::to_bigint<uint64_t>(ctx, s->circumference());
				});
			}

			template <typename T>
				requires std::is_integral<T>::value
			using Rectangle = Kernel::Definition::Rectangle<T>;

			inline static auto cut(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSDefine::ImageView
			{
				return proxy_wrapper(ctx, "cut", [&]() {
					assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "cut");
					auto s = static_cast<Data*>(JS_GetOpaque2(ctx, argv[0], class_id));
					if (s == nullptr) {
						return JS_EXCEPTION;
					}
					auto x_atom = Atom{ctx, "x"};
					auto y_atom = Atom{ctx, "y"};
					auto width_atom = Atom{ctx, "width"};
					auto height_atom = Atom{ctx, "height"};
					auto x = get_property_bigint64(ctx, argv[1], x_atom.value);
					auto y = get_property_bigint64(ctx, argv[1], y_atom.value);
					auto width = get_property_bigint64(ctx, argv[1], width_atom.value);
					auto height = get_property_bigint64(ctx, argv[1], height_atom.value);
					auto data = std::make_unique<Data>(std::move(Data::cut(*s, Rectangle<int>(static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height)))));
					auto atom = Atom{ctx, "prototype"};
					auto proto = JS_GetProperty(ctx, this_val, atom.value);
					if (JS_IsException(proto)) {
						return JS_EXCEPTION;
					}
					auto obj = JS_NewObjectProtoClass(ctx, proto, class_id);
					JS_FreeValue(ctx, proto);
					if (JS_IsException(obj)) {
						return JS_EXCEPTION;
					}
					JS_SetOpaque(obj, data.release());
					return obj;
				});
			}

			inline static auto resize (
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSDefine::ImageView
			{
				return proxy_wrapper(ctx, "resize", [&]() {
					assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "resize");
					auto s = static_cast<Data*>(JS_GetOpaque2(ctx, argv[0], class_id));
					if (s == nullptr) {
						return JS_EXCEPTION;
					}
					auto data = std::unique_ptr<Data, decltype(make_deleter<Data>())>(new Data(std::move(Data::resize(*s, JS::Converter::get_float32(ctx, argv[1])))), make_deleter<Data>());
					auto atom = Atom{ctx, "prototype"};
					auto proto = JS_GetProperty(ctx, this_val, atom.value);
					if (JS_IsException(proto)) {
						return JS_EXCEPTION;
					}
					auto obj = JS_NewObjectProtoClass(ctx, proto, class_id);
					JS_FreeValue(ctx, proto);
					if (JS_IsException(obj)) {
						return JS_EXCEPTION;
					}
					JS_SetOpaque(obj, data.release());
					return obj;
				});
			}

			inline static auto scale(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSDefine::ImageView
			{
				return proxy_wrapper(ctx, "scale", [&]() {
					assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "scale");
					auto s = static_cast<Data*>(JS_GetOpaque2(ctx, argv[0], class_id));
					if (s == nullptr) {
						return JS_EXCEPTION;
					}
					auto data = std::make_unique<Data>(std::move(Data::scale(*s, JS::Converter::get_float64(ctx, argv[1]))));
					auto atom = Atom{ctx, "prototype"};
					auto proto = JS_GetProperty(ctx, this_val, atom.value);
					if (JS_IsException(proto)) {
						return JS_EXCEPTION;
					}
					auto obj = JS_NewObjectProtoClass(ctx, proto, class_id);
					JS_FreeValue(ctx, proto);
					if (JS_IsException(obj)) {
						return JS_EXCEPTION;
					}
					JS_SetOpaque(obj, data.release());
					return obj;
				});
			}

			inline static auto rotate (
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSDefine::ImageView
			{
				return proxy_wrapper(ctx, "rotate", [&]() {
					assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "rotate");
					auto s = static_cast<Data*>(JS_GetOpaque2(ctx, argv[0], class_id));
					if (s == nullptr) {
						return JS_EXCEPTION;
					}
					auto data = std::make_unique<Data>(std::move(Data::rotate(*s, JS::Converter::get_float64(ctx, argv[1]))));
					auto atom = Atom{ctx, "prototype"};
					auto proto = JS_GetProperty(ctx, this_val, atom.value);
					if (JS_IsException(proto)) {
						return JS_EXCEPTION;
					}
					auto obj = JS_NewObjectProtoClass(ctx, proto, class_id);
					JS_FreeValue(ctx, proto);
					if (JS_IsException(obj)) {
						return JS_EXCEPTION;
					}
					JS_SetOpaque(obj, data.release());
					return obj;
				});
			}

			inline static auto write_fs (
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return proxy_wrapper(ctx, "write_fs", [&]() {
					assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "write_fs");
					auto s = static_cast<Data*>(JS_GetOpaque2(ctx, argv[1], class_id));
					if (s == nullptr) {
						return JS_EXCEPTION;
					}
					Kernel::Definition::ImageIO::write_png(JS::Converter::get_string(ctx, argv[0]), *s);
					return JS_UNDEFINED;
				});
			}

			inline static auto read_fs(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSDefine::ImageView
			{
				return proxy_wrapper(ctx, "read_fs", [&]() {
					assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "read_fs");
					auto data = std::make_unique<Data>(std::move(Kernel::Definition::ImageIO::read_png(JS::Converter::get_string(ctx, argv[0]))));
					auto atom = Atom{ctx, "prototype"};
					auto proto = JS_GetProperty(ctx, this_val, atom.value);
					if (JS_IsException(proto)) {
						return JS_EXCEPTION;
					}
					auto obj = JS_NewObjectProtoClass(ctx, proto, class_id);
					JS_FreeValue(ctx, proto);
					if (JS_IsException(obj)) {
						return JS_EXCEPTION;
					}
					JS_SetOpaque(obj, data.release());
					return obj;
				});
			}

			inline static auto instance(
				JSContext *ctx,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSDefine::ImageView
			{
				return proxy_wrapper(ctx, "instance", [&]() {
					assert_conditional(argc == 3, fmt::format("{} 3, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "instance");
					auto s = std::make_unique<Data>(0, 0, static_cast<int>(JS::Converter::get_bigint64(ctx, argv[0])), static_cast<int>(JS::Converter::get_bigint64(ctx, argv[1])), JS::Converter::to_binary_list(ctx, argv[2]));
					auto atom = Atom{ctx, "prototype"};
					auto proto = JS_GetProperty(ctx, this_val, atom.value);
					if (JS_IsException(proto)) {
						return JS_EXCEPTION;
					}
					auto obj = JS_NewObjectProtoClass(ctx, proto, class_id);
					JS_FreeValue(ctx, proto);
					if (JS_IsException(obj)) {
						return JS_EXCEPTION;
					}
					JS_SetOpaque(obj, s.release());
					return obj;
				});
			}

			inline static auto constexpr proto_functions = std::to_array<JSCFunctionListEntry>({
				JS_CPPGETSET_MAGIC_DEF("bit_depth", getter, setter, ImageView::Magic::bit_depth),
				JS_CPPGETSET_MAGIC_DEF("channels", getter, setter, ImageView::Magic::channels),
				JS_CPPGETSET_MAGIC_DEF("color_type", getter, setter, ImageView::Magic::color_type),
				JS_CPPGETSET_MAGIC_DEF("interlace_type", getter, setter, ImageView::Magic::interlace_type),
				JS_CPPGETSET_MAGIC_DEF("rowbytes", getter, setter, ImageView::Magic::rowbytes),
				JS_CPPGETSET_MAGIC_DEF("width", getter, setter, ImageView::Magic::width),
				JS_CPPGETSET_MAGIC_DEF("height", getter, setter, ImageView::Magic::height),
				JS_CPPGETSET_MAGIC_DEF("data", getter, setter, ImageView::Magic::data),
				JS_CPPFUNC_DEF("area", 0, area),
				JS_CPPFUNC_DEF("circumference", 0, circumference),
			});

			inline static auto register_class(
				JSContext *ctx
			) -> void
			{
				JS_NewClassID(JS_GetRuntime(ctx), &class_id);
				assert_conditional(JS_NewClass(JS_GetRuntime(ctx), class_id, &this_class) == 0, "ImageView class register failed", "register_class");
				auto class_name = _class_name();
				auto point_ctor = JS_NewCFunction2(ctx, constructor, class_name.data(), 2, JS_CFUNC_constructor, 0);
				auto proto = JS_NewObject(ctx);
				auto instance_c = JS_NewCFunction2(ctx, instance, "instance", 0, JS_CFUNC_generic, 0);
				auto instance_atom = Atom(ctx, "instance");
				JS_DefinePropertyValue(ctx, point_ctor, instance_atom.value, instance_c, int{JS_PROP_C_W_E});
				auto default_cut = JS_NewCFunction2(ctx, cut, "cut", 0, JS_CFUNC_generic, 0);
				auto default_resize = JS_NewCFunction2(ctx, resize, "resize", 0, JS_CFUNC_generic, 0);
				auto default_scale = JS_NewCFunction2(ctx, scale, "scale", 0, JS_CFUNC_generic, 0);
				auto default_rotate = JS_NewCFunction2(ctx, rotate, "rotate", 0, JS_CFUNC_generic, 0);
				auto default_read_fs = JS_NewCFunction2(ctx, read_fs, "read_fs", 0, JS_CFUNC_generic, 0);
				auto default_write_fs = JS_NewCFunction2(ctx, write_fs, "write_fs", 0, JS_CFUNC_generic, 0);
				auto atom_cut = Atom(ctx, "cut");
				JS_DefinePropertyValue(ctx, point_ctor, atom_cut.value, default_cut, int{JS_PROP_C_W_E});
				auto atom_resize = Atom(ctx, "resize");
				JS_DefinePropertyValue(ctx, point_ctor, atom_resize.value, default_resize, int{JS_PROP_C_W_E});
				auto atom_scale = Atom(ctx, "scale");
				JS_DefinePropertyValue(ctx, point_ctor, atom_scale.value, default_scale, int{JS_PROP_C_W_E});
				auto atom_rotate = Atom(ctx, "rotate");
				JS_DefinePropertyValue(ctx, point_ctor, atom_rotate.value, default_rotate, int{JS_PROP_C_W_E});
				auto atom_read_fs = Atom(ctx, "read_fs");
				JS_DefinePropertyValue(ctx, point_ctor, atom_read_fs.value, default_read_fs, int{JS_PROP_C_W_E});
				auto atom_write_fs = Atom(ctx, "write_fs");
				JS_DefinePropertyValue(ctx, point_ctor, atom_write_fs.value, default_write_fs, int{JS_PROP_C_W_E});
				JS_SetPropertyFunctionList(ctx, proto, proto_functions.data(), proto_functions.size());
				JS_SetConstructor(ctx, point_ctor, proto);
				auto global_obj = JS_GetGlobalObject(ctx);
				auto obj1 = make_instance_object(ctx, global_obj, "Sen");
				auto obj2 = make_instance_object(ctx, obj1, "Kernel");
				auto atom = Atom(ctx, class_name.data(), class_name.size());
				JS_DefinePropertyValue(ctx, obj2, atom.value, point_ctor, int{JS_PROP_C_W_E});
				JS_FreeValue(ctx, global_obj);
				JS_FreeValue(ctx, obj1);
				JS_FreeValue(ctx, obj2);
				JS_FreeValue(ctx, proto);
				return;
			}

		}

	}

	namespace Thread
	{

		/**
		 * ----------------------------------------
		 * JavaScript Sleep
		 * @param argv[0]: time to sleep
		 * @return: undefined
		 * ----------------------------------------
		 */

		inline static auto sleep (
			JSContext *ctx,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::undefined
		{
			return proxy_wrapper(ctx, "sleep", [&](){
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "sleep");
				auto time = JS::Converter::get_bigint64(ctx, argv[0]);
				Sen::Kernel::Definition::Timer::sleep(time);
				return JS_UNDEFINED;
			});
		}

		inline static auto now (
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::number
		{
			return proxy_wrapper(context, "now", [&](){
				auto current = std::chrono::duration<double>(std::chrono::system_clock::now().time_since_epoch()).count();
				return JS::Converter::to_number(context, current);
			});
		}

	}

	namespace Diff
	{

		namespace VCDiff
		{

			inline static auto encode_fs(
				JSContext *context,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return proxy_wrapper(context, "encode_fs", [&](){
					assert_conditional(argc == 4, fmt::format("{} 4, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "encode_fs");
					auto before_file = JS::Converter::get_string(context, argv[0]);
					auto after_file = JS::Converter::get_string(context, argv[1]);
					auto patch_file = JS::Converter::get_string(context, argv[2]);
					auto flag = JS::Converter::get_int32(context, argv[3]);
					Sen::Kernel::Definition::Diff::VCDiff::encode_fs(before_file, after_file, patch_file, static_cast<Sen::Kernel::Definition::Diff::VCDiff::Flag>(flag));
					return JS_UNDEFINED; 
				});
			}

			inline static auto decode_fs(
				JSContext *context,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return proxy_wrapper(context, "decode_fs", [&](){
					assert_conditional(argc == 3, fmt::format("{} 3, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "decode_fs");
					auto before_file = JS::Converter::get_string(context, argv[0]);
					auto patch_file = JS::Converter::get_string(context, argv[1]);
					auto after_file = JS::Converter::get_string(context, argv[2]);
					Sen::Kernel::Definition::Diff::VCDiff::decode_fs(before_file, patch_file, after_file);
					return JS_UNDEFINED;
				});
			}

		}

	}

	namespace OperatingSystem
	{

		inline static auto current(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::string
		{
			return proxy_wrapper(context, "current", [&](){
				#if WINDOWS
				return JS::Converter::to_string(context, std::string{"Windows"});
				#elif ANDROID
				return JS::Converter::to_string(context, std::string{"Android"});
				#elif LINUX
				return JS::Converter::to_string(context, std::string{"Linux"});
				#elif APPLE
				return JS::Converter::to_string(context, std::string{"Macintosh"});
				#elif IPHONE
				return JS::Converter::to_string(context, std::string{"iPhone"});
				#else
				return JS::Converter::to_string(context, std::string{"Unknown OS"});
				#endif
			});
		}

		#if WINDOWS
		struct FileDeleter
		{
			inline auto operator()(FILE *file) const -> void
			{
				if (file != nullptr)
				{
					std::fclose(file);
					file = nullptr;
				}
				return;
			}
		};
		#else
		struct FileDeleter
		{
			inline auto operator()(FILE *file) const -> void
			{
				if (file != nullptr)
				{
					pclose(file);
					file = nullptr;
				}
				return;
			}
		};
		#endif


		inline static auto architecture(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::string
		{
			return proxy_wrapper(context, "architecture", [&](){
				#if WINDOWS
				auto si = SYSTEM_INFO{};
				GetNativeSystemInfo(&si);
				switch (si.wProcessorArchitecture)
				{
					case PROCESSOR_ARCHITECTURE_AMD64:
					{
						return JS::Converter::to_string(context, std::string{"x64"});
					}
					case PROCESSOR_ARCHITECTURE_ARM:
					{
						return JS::Converter::to_string(context, std::string{"x86"});
					}
					case PROCESSOR_ARCHITECTURE_IA64:
					{
						return JS::Converter::to_string(context, std::string{"arm64"});
					}
					case PROCESSOR_ARCHITECTURE_INTEL:
					{
						return JS::Converter::to_string(context, std::string{"arm"});
					}
					default:
					{
						return JS::Converter::to_string(context, std::string{"Unknown"});
					}
				}
				#else
				auto buffer = std::array<char, 128>();
				auto result = std::string();
				auto pipe = std::unique_ptr<FILE, FileDeleter>(popen("uname -m", "r"));
				if (pipe == nullptr)
				{
					throw Exception("cannot open pipe", std::source_location::current(), "architecture");
				}
				while (fgets(buffer.data(), 128, pipe.get()) != nullptr)
				{
					result += buffer.data();
				}
				result.erase(std::remove(result.begin(), result.end(), '\n'), result.end());
				if (result == "x86_64")
				{
					return JS::Converter::to_string(context, std::string{"x64"});
				}
				else if (result == "i686")
				{
					return JS::Converter::to_string(context, std::string{"x86"});
				}
				else if (result == "aarch64")
				{
					return JS::Converter::to_string(context, std::string{"arm64"});
				}
				else if (result == "armv7l")
				{
					return JS::Converter::to_string(context, std::string{"arm"});
				}
				else
				{
					return JS::Converter::to_string(context, std::string{"Unknown"});
				}
				#endif
			});
		}

	}

	namespace ArrayBuffer
	{

		inline static auto open (
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::string
		{
			return proxy_wrapper(context, "open", [&]() {
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "open");
				auto source = JS::Converter::get_string(context, argv[0]);
				auto result = JS::Converter::read_file_as_js_arraybuffer(context, source);
				return result;
			});
		}

		inline static auto out(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::undefined
		{
			return proxy_wrapper(context, "out", [&]() {
				assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "out");
				auto destination = JS::Converter::get_string(context, argv[0]);
				auto array_buffer = argv[1];
				JS::Converter::write_file_as_arraybuffer(context, destination, array_buffer);
				return JS_UNDEFINED;
			});
		}

		inline static auto random(
			JSContext *ctx,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::undefined
		{
			return proxy_wrapper(ctx, "random", [&](){
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "random");
				auto arrayBuffer = argv[0];
				auto byteLength = std::size_t{};
				auto data = JS_GetArrayBuffer(ctx, &byteLength, arrayBuffer);
				if (data == nullptr) {
					throw Exception(fmt::format("{}", Kernel::Language::get("js.converter.failed_to_get_js_array_buffer")), std::source_location::current(), "random");
				}
				for (auto i : Range<std::size_t>(byteLength)) {
					data[i] = rand() % 256;
				}
				return JS_UNDEFINED;
			});
		}

		inline static auto fill(
			JSContext *ctx,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::undefined
		{
			return proxy_wrapper(ctx, "fill", [&](){
				assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "fill");
				auto arrayBuffer = argv[0];
				auto byte_c = static_cast<std::uint8_t>(JS::Converter::get_bigint64(ctx, argv[0]));
				auto byteLength = std::size_t{};
				auto data = JS_GetArrayBuffer(ctx, &byteLength, arrayBuffer);
				if (data == nullptr) {
					throw Exception(fmt::format("{}", Kernel::Language::get("js.converter.failed_to_get_js_array_buffer")), std::source_location::current(), "fill");
				}
				for (auto i : Range<std::size_t>(byteLength)) {
					data[i] = byte_c;
				}
				return JS_UNDEFINED;
			});
		}

	}

	namespace Process
	{

		inline static auto run(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::undefined
		{
			return proxy_wrapper(context, "run", [&](){
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "run");
				auto source = JS::Converter::get_string(context, argv[0]);
				Sen::Kernel::Process::run(source);
				return JS_UNDEFINED;
			});
		}

		inline static auto is_exists_in_path_environment(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::boolean
		{
			return proxy_wrapper(context, "is_exists_in_path_environment", [&](){
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "is_exists_in_path_environment");
				auto source = JS::Converter::get_string(context, argv[0]);
				auto result = Sen::Kernel::Process::is_exists_in_path_environment(source);
				return JS::Converter::to_bool(context, result);
			});
		}

		inline static auto get_path_environment(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::string
		{
			return proxy_wrapper(context, "get_path_environment", [&](){
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "get_path_environment");
				auto source = JS::Converter::get_string(context, argv[0]);
				auto result = Sen::Kernel::Process::get_path_environment(source);
				return JS::Converter::to_string(context, result);
			});
		}

		inline static auto execute(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::string
		{
			return proxy_wrapper(context, "execute", [&](){
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "execute");
				auto source = JS::Converter::get_string(context, argv[0]);
				auto result = Sen::Kernel::Process::execute(source);
				return JS::Converter::to_string(context, result);
			});
		}

	}

	namespace Console
	{

		inline static auto exchange_color(
			Sen::Kernel::Interface::Color color
		) -> std::string
		{
			using Color = Sen::Kernel::Interface::Color;
			if (color == Color::RED)
			{
				return "red";
			}
			if (color == Color::GREEN)
			{
				return "green";
			}
			if (color == Color::CYAN)
			{
				return "cyan";
			}
			if (color == Color::YELLOW)
			{
				return "yellow";
			}
			return "default";
		}

		inline static auto print(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::undefined
		{
			return proxy_wrapper(context, "print", [&](){
				assert_conditional(argc >= 1, fmt::format("argument expected greater than {} but received {}", "1", argc), "print");
				switch (argc)
				{
					case 1:
					{
						Shell::callback(construct_string_list(List<std::string>{std::string{"display"}, JS::Converter::get_string(context, argv[0])}).get(), nullptr);
						break;
					}
					case 2:
					{
						Shell::callback(construct_string_list(List<std::string>{std::string{"display"}, JS::Converter::get_string(context, argv[0]), JS::Converter::get_string(context, argv[1])}).get(), nullptr);
						break;
					}
					default:
					{
						Shell::callback(construct_string_list(List<std::string>{std::string{"display"}, JS::Converter::get_string(context, argv[0]), JS::Converter::get_string(context, argv[1]), exchange_color(static_cast<Sen::Kernel::Interface::Color>(JS::Converter::get_int32(context, argv[2])))}).get(), nullptr);
						break;
					}
				}
				return JS_UNDEFINED;
			});
		}

		inline static auto readline(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::string
		{
			return proxy_wrapper(context, "readline", [&](){
				Shell::callback(construct_string_list(List<std::string>{std::string{"wait"}}).get(), nullptr);
				auto destination = std::make_unique<CStringView>();
				Shell::callback(construct_string_list(List<std::string>{std::string{"input"}}).get(), destination.get());
				return JS::Converter::to_string(context, std::string{destination->value, static_cast<std::size_t>(destination->size)});
			});
		}
	}

	namespace Path
	{

		inline static auto join(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::string
		{
			return proxy_wrapper(context, "join", [&](){
				auto value = List<std::string>{};
				for(auto i : Range<int>(argc))
				{
					auto source = JS::Converter::get_string(context, argv[i]);
					value.emplace_back(source);
				}
				return JS::Converter::to_string(context, Sen::Kernel::Path::Script::join(value));
			});
		}

		inline static auto basename(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::string
		{
			return proxy_wrapper(context, "basename", [&](){
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "basename");
				auto source = JS::Converter::get_string(context, argv[0]);
				auto result = Sen::Kernel::Path::Script::basename(source);
				return JS::Converter::to_string(context, result);
			});
		}

		inline static auto delimiter(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::string
		{
			return proxy_wrapper(context, "delimiter", [&](){
				auto result = Sen::Kernel::Path::Script::delimiter();
				return JS::Converter::to_string(context, result);
			});
		}

		inline static auto dirname(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::string
		{
			return proxy_wrapper(context, "dirname", [&](){
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "dirname");
				auto source = JS::Converter::get_string(context, argv[0]);
				auto result = Sen::Kernel::Path::Script::dirname(source);
				return JS::Converter::to_string(context, result);
			});
		}

		inline static auto format(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::string
		{
			return proxy_wrapper(context, "format", [&](){
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "format");
				auto dir_atom = Atom{context, "dir"};
				auto base_atom = Atom{context, "base"};
				auto dir = get_property_string(context, argv[0], dir_atom.value);
				auto base = get_property_string(context, argv[0], base_atom.value);
				auto result = Kernel::Path::Script::format(Kernel::Path::Format{dir, base});
				return JS::Converter::to_string(context, result);
			});
		}

		inline static auto normalize(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::string
		{
			return proxy_wrapper(context, "normalize", [&](){
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "normalize");
				auto source = JS::Converter::get_string(context, argv[0]);
				auto result = Sen::Kernel::Path::Script::normalize(source);
				return JS::Converter::to_string(context, result);
			});
		}

		inline static auto base_without_extension(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::string
		{
			return proxy_wrapper(context, "base_without_extension", [&](){
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "base_without_extension");
				auto source = JS::Converter::get_string(context, argv[0]);
				auto result = Sen::Kernel::Path::Script::base_without_extension(source);
				return JS::Converter::to_string(context, result);
			});
		}

		inline static auto except_extension(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::string
		{
			return proxy_wrapper(context, "except_extension", [&](){
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "except_extension");
				auto source = JS::Converter::get_string(context, argv[0]);
				auto result = Sen::Kernel::Path::Script::except_extension(source);
				return JS::Converter::to_string(context, result);
			});
		}

		inline static auto resolve(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::string
		{
			return proxy_wrapper(context, "resolve", [&](){
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "resolve");
				auto source = JS::Converter::get_string(context, argv[0]);
				auto result = Sen::Kernel::Path::Script::resolve(source);
				return JS::Converter::to_string(context, result);
			});
		}

		inline static auto extname(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::string
		{
			return proxy_wrapper(context, "extname", [&](){
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "extname");
				auto source = JS::Converter::get_string(context, argv[0]);
				auto result = Sen::Kernel::Path::Script::extname(source);
				return JS::Converter::to_string(context, result);
			});
		}

		inline static auto is_absolute(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::boolean
		{
			return proxy_wrapper(context, "extname", [&](){
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "is_absolute");
				auto source = JS::Converter::get_string(context, argv[0]);
				auto result = Sen::Kernel::Path::Script::is_absolute(source);
				return JS::Converter::to_bool(context, result);
			});
		}

		inline static auto relative(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::string
		{
			return proxy_wrapper(context, "relative", [&](){
				assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "relative");
				auto from = JS::Converter::get_string(context, argv[0]);
				auto to = JS::Converter::get_string(context, argv[1]);
				auto result = Sen::Kernel::Path::Script::relative(from, to);
				return JS::Converter::to_string(context, result);
			});
		}

	}

	namespace FileSystem
	{

		inline static auto read_file(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::string
		{
			return proxy_wrapper(context, "read_file", [&](){
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "read_file");
				auto source = JS::Converter::get_string(context, argv[0]);
				auto result = Sen::Kernel::FileSystem::read_file(source);
				return JS::Converter::to_string(context, result);
			});
		}

		inline static auto read_file_encode_with_utf16le(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::string
		{
			return proxy_wrapper(context, "read_file_encode_with_utf16le", [&](){
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "read_file_encode_with_utf16le");
				auto source = JS::Converter::get_string(context, argv[0]);
				auto result = Sen::Kernel::FileSystem::read_file_by_utf16(source);
				auto converter = std::wstring_convert<std::codecvt_utf8<wchar_t>>{};
				auto utf8_string = std::string{converter.to_bytes(result)};
				return JS::Converter::to_string(context, utf8_string);
			});
		}

		inline static auto write_file(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::undefined
		{
			return proxy_wrapper(context, "write_file", [&](){
				assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "write_file");
				auto destination = JS::Converter::get_string(context, argv[0]);
				auto data = JS::Converter::get_string(context, argv[1]);
				Sen::Kernel::FileSystem::write_file(destination, data);
				return JS_UNDEFINED;
			});
		}

		inline static auto write_file_encode_with_utf16le(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::undefined
		{
			return proxy_wrapper(context, "write_file_encode_with_utf16le", [&](){
				assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "write_file_encode_with_utf16le");
				auto destination = JS::Converter::get_string(context, argv[0]);
				auto data = JS::Converter::get_string(context, argv[1]);
				auto converter = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>{};
				auto result = std::wstring{ converter.from_bytes(data) };
				Sen::Kernel::FileSystem::write_file_by_utf16le(destination, result);
				return JS_UNDEFINED;
			});
		}

		inline static auto read_current_directory(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::Array<JSElement::string>
		{
			return proxy_wrapper(context, "read_current_directory", [&](){
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "read_current_directory");
				auto source = JS::Converter::get_string(context, argv[0]);
				auto result = Sen::Kernel::FileSystem::read_directory(source);
				return JS::Converter::to_array(context, result);
			});
		}

		inline static auto read_directory_only_file(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::Array<JSElement::string>
		{
			return proxy_wrapper(context, "read_directory_only_file", [&](){
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "read_directory_only_file");
				auto source = JS::Converter::get_string(context, argv[0]);
				auto result = Sen::Kernel::FileSystem::read_directory_only_file(source);
				return JS::Converter::to_array(context, result);
			});
		}

		inline static auto read_directory_only_directory(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::Array<JSElement::string>
		{
			return proxy_wrapper(context, "read_directory_only_directory", [&](){
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "read_directory_only_directory");
				auto source = JS::Converter::get_string(context, argv[0]);
				auto result = Sen::Kernel::FileSystem::read_directory_only_directory(source);
				return JS::Converter::to_array(context, result);
			});
		}

		inline static auto read_directory(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::Array<JSElement::string>
		{
			return proxy_wrapper(context, "read_directory", [&](){
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "read_directory");
				auto source = JS::Converter::get_string(context, argv[0]);
				auto result = Sen::Kernel::FileSystem::read_whole_directory(source);
				return JS::Converter::to_array(context, result);
			});
		}

		inline static auto create_directory(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::undefined
		{
			return proxy_wrapper(context, "create_directory", [&](){
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "create_directory");
				auto destination = JS::Converter::get_string(context, argv[0]);
				Sen::Kernel::FileSystem::create_directory(destination);
				return JS_UNDEFINED;
			});
		}

		inline static auto is_file(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::boolean
		{
			return proxy_wrapper(context, "is_file", [&](){
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "is_file");
				auto source = JS::Converter::get_string(context, argv[0]);
				return JS::Converter::to_bool(context, Kernel::Path::Script::is_file(source));
			});
		}

		inline static auto is_directory(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::boolean
		{
			return proxy_wrapper(context, "is_directory", [&](){
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "is_directory");
				auto source = JS::Converter::get_string(context, argv[0]);
				return JS::Converter::to_bool(context, Kernel::Path::Script::is_directory(source));
			});
		}

		namespace Operation
		{

			inline static auto rename(
				JSContext *context,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return proxy_wrapper(context, "rename", [&](){
					assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "rename");
					auto source = JS::Converter::get_string(context, argv[0]);
					auto destination = JS::Converter::get_string(context, argv[1]);
					Kernel::Path::Script::rename(source, destination);
					return JS_UNDEFINED;
				});
			}

			inline static auto copy(
				JSContext *context,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return proxy_wrapper(context, "copy", [&](){
					assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "copy");
					auto source = JS::Converter::get_string(context, argv[0]);
					auto destination = JS::Converter::get_string(context, argv[1]);
					Kernel::Path::Script::copy(source, destination);
					return JS_UNDEFINED;
				});
			}

			inline static auto copy_directory(
				JSContext *context,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return proxy_wrapper(context, "copy_directory", [&](){
					assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "copy_directory");
					auto source = JS::Converter::get_string(context, argv[0]);
					auto destination = JS::Converter::get_string(context, argv[1]);
					Kernel::Path::Script::copy_directory(source, destination);
					return JS_UNDEFINED;
				});
			}

			inline static auto remove(
				JSContext *context,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return proxy_wrapper(context, "remove", [&](){
					assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "remove");
					auto source = JS::Converter::get_string(context, argv[0]);
					Kernel::Path::Script::remove(source);
					return JS_UNDEFINED;
				});
			}

			inline static auto remove_all(
				JSContext *context,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return proxy_wrapper(context, "remove_all", [&](){
					assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "remove_all");
					auto source = JS::Converter::get_string(context, argv[0]);
					Kernel::Path::Script::remove_all(source);
					return JS_UNDEFINED;
				});
			}

		}
	}

	namespace Dimension
	{

		inline static auto area(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::bigint
		{
			return proxy_wrapper(context, "area", [&](){
				auto width_atom = Atom{context, "width"};
				auto height_atom = Atom{context, "height"};
				auto width = get_property_bigint64(context, this_val, width_atom.value);
				auto height = get_property_bigint64(context, this_val, height_atom.value);
				auto area = width * height;
				return JS_NewBigInt64(context, area);
			});
		}

		inline static auto circumference(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::bigint
		{
			return proxy_wrapper(context, "circumference", [&](){
				auto width_atom = Atom{context, "width"};
				auto height_atom = Atom{context, "height"};
				auto width = get_property_bigint64(context, this_val, width_atom.value);
				auto height = get_property_bigint64(context, this_val, height_atom.value);
				auto area = (width + height) * 2;
				return JS_NewBigInt64(context, area);
			});
		}

		inline static auto instance(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::Object
		{
			return proxy_wrapper(context, "instance", [&](){
				assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "instance");
				auto image_obj = JSValue{};
				auto area_func = JS_NewCFunction2(context, area, "area", 0, JS_CFUNC_generic, 0);
				auto circumference_func = JS_NewCFunction2(context, circumference, "circumference", 0, JS_CFUNC_generic, 0);
				image_obj = JS_NewObject(context);
				auto atom_width = Atom(context, "width");
				JS_DefinePropertyValue(context, image_obj, atom_width.value, JS_NewBigInt64(context, JS::Converter::get_bigint64(context, argv[0])), int{JS_PROP_C_W_E});
				auto atom_height = Atom(context, "height");
				JS_DefinePropertyValue(context, image_obj, atom_height.value, JS_NewBigInt64(context, JS::Converter::get_bigint64(context, argv[1])), int{JS_PROP_C_W_E});
				auto atom_area = Atom(context, "area");
				JS_DefinePropertyValue(context, image_obj, atom_area.value, area_func, JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE);
				auto atom_circumference = Atom(context, "circumference");
				JS_DefinePropertyValue(context, image_obj, atom_circumference.value, circumference_func, JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE);
				return image_obj;
			});
		}

		inline static auto open(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::Object
		{
			return proxy_wrapper(context, "open", [&](){
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "open");
				auto source = JS::Converter::get_string(context, argv[0]);
				auto image = Sen::Kernel::Definition::ImageIO::read_png(source);
				auto image_obj = JSValue{};
				auto area_func = JS_NewCFunction2(context, area, "area", 0, JS_CFUNC_generic, 0);
				auto circumference_func = JS_NewCFunction2(context, circumference, "circumference", 0, JS_CFUNC_generic, 0);
				image_obj = JS_NewObject(context);
				auto atom_width = Atom(context, "width");
				JS_DefinePropertyValue(context, image_obj, atom_width.value, JS_NewBigInt64(context, image.width), int{JS_PROP_C_W_E});
				auto atom_height = Atom(context, "height"); 
				JS_DefinePropertyValue(context, image_obj, atom_height.value, JS_NewBigInt64(context, image.height), int{JS_PROP_C_W_E});
				auto atom_bit_depth = Atom(context, "bit_depth");  
				JS_DefinePropertyValue(context, image_obj, atom_bit_depth.value, JS_NewBigInt64(context, image.bit_depth), int{JS_PROP_C_W_E});
				auto atom_color_type = Atom(context, "color_type"); 
				JS_DefinePropertyValue(context, image_obj, atom_color_type.value, JS_NewBigInt64(context, image.color_type), int{JS_PROP_C_W_E});
				auto atom_interlace_type = Atom(context, "interlace_type");
				JS_DefinePropertyValue(context, image_obj, atom_interlace_type.value, JS_NewBigInt64(context, image.interlace_type), int{JS_PROP_C_W_E});
				auto atom_channels = Atom(context, "channels"); 
				JS_DefinePropertyValue(context, image_obj, atom_channels.value, JS_NewBigInt64(context, image.channels), int{JS_PROP_C_W_E});
				auto atom_rowbytes = Atom(context, "rowbytes"); 
				JS_DefinePropertyValue(context, image_obj, atom_rowbytes.value, JS_NewBigInt64(context, image.rowbytes), int{JS_PROP_C_W_E});
				auto atom_data = Atom(context, "data");  
				JS_DefinePropertyValue(context, image_obj, atom_data.value, JS_NewArrayBufferCopy(context, image.data().data(), image.data().size()), int{JS_PROP_C_W_E});
				auto atom_area = Atom(context, "area");  
				JS_DefinePropertyValue(context, image_obj, atom_area.value, area_func, JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE);
				auto atom_circumference = Atom(context, "circumference");
				JS_DefinePropertyValue(context, image_obj, atom_circumference.value, circumference_func, JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE);
				return image_obj;
			});
		}

		inline static auto write(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::undefined
		{
			using Image = Sen::Kernel::Definition::Image<int>;
			return proxy_wrapper(context, "write", [&](){
				assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "write");
				auto source_file = JS::Converter::get_string(context, argv[0]);
				auto obj = argv[1];
				auto atom_width = Atom{context, "width"};
				auto atom_height = Atom{context, "height"};
				auto atom_bit_depth = Atom{context, "bit_depth"};
				auto atom_color_type = Atom{context, "color_type"};
				auto atom_interlace_type = Atom{context, "interlace_type"};
				auto atom_channels = Atom{context, "channels"};
				auto atom_rowbytes = Atom{context, "rowbytes"};
				auto atom_data = Atom{context, "data"};
				auto width = get_property_bigint64(context, obj, atom_width.value);
				auto height = get_property_bigint64(context, obj, atom_height.value);
				auto bit_depth = get_property_bigint64(context, obj, atom_bit_depth.value);
				auto color_type = get_property_bigint64(context, obj, atom_color_type.value);
				auto interlace_type = get_property_bigint64(context, obj, atom_interlace_type.value);
				auto channels = get_property_bigint64(context, obj, atom_channels.value);
				auto rowbytes = get_property_bigint64(context, obj, atom_rowbytes.value);
				auto data_val = JS_GetProperty(context, obj, atom_data.value);
				auto data_len = std::size_t{};
				auto data = JS_GetArrayBuffer(context, &data_len, data_val);
				assert_conditional(data != nullptr, "Cannot get ArrayBuffer from the current object", "write");
				JS_FreeValue(context, data_val);
				auto image = Image(static_cast<int>(width), static_cast<int>(height), static_cast<int>(bit_depth), static_cast<int>(color_type), static_cast<int>(interlace_type), static_cast<int>(channels), static_cast<int>(rowbytes), std::move(make_list(data, data_len)));
				Sen::Kernel::Definition::ImageIO::write_png(source_file, image);
				return JS_UNDEFINED;
			});
		}

	}

	namespace Image
	{

		inline static auto scale_fs(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::undefined
		{
			return proxy_wrapper(context, "scale_fs", [&](){
				assert_conditional(argc == 3, fmt::format("{} 3, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "scale_fs");
				auto source = JS::Converter::get_string(context, argv[0]);
				auto destination = JS::Converter::get_string(context, argv[1]);
				auto percentage = JS::Converter::get_float32(context, argv[2]);
				Sen::Kernel::Definition::ImageIO::scale_png(source, destination, percentage);
				return JS_UNDEFINED;
			});
		}

		inline static auto transparent_fs(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::undefined
		{
			return proxy_wrapper(context, "transparent_fs", [&](){
				assert_conditional(argc == 3, fmt::format("{} 3, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "transparent_fs");
				auto destination = JS::Converter::get_string(context, argv[0]);
				auto width = JS::Converter::get_int32(context, argv[1]);
				auto height = JS::Converter::get_int32(context, argv[2]);
				Sen::Kernel::Definition::ImageIO::transparent_png(destination, width, height);
				return JS_UNDEFINED;
			});
		}

		struct Coordinate
		{
			int x;
			int y;
		};

		inline static auto join_png(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::undefined
		{
			using Image = Sen::Kernel::Definition::Image<int>;
			return proxy_wrapper(context, "join_png", [&](){
				assert_conditional(argc == 3, fmt::format("{} 3, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "join_png");
				auto destination = JS::Converter::get_string(context, argv[0]);
				auto atom_width = Atom{context, "width"};
				auto atom_height = Atom{context, "height"};
				auto atom_length = Atom{context, "length"};
				auto width = get_property_bigint64(context, argv[1], atom_width.value);
				auto height = get_property_bigint64(context, argv[1], atom_height.value);
				auto length = get_property_int32(context, argv[2], atom_length.value);
				auto m_data = List<Sen::Kernel::Definition::Image<int>>{};
				auto x_atom = Atom{context, "x"};
				auto y_atom = Atom{context, "y"};
				auto data_atom = Atom{context, "data"};
				auto x_y = List<Coordinate>{};
				for (auto i : Range<int>(length)) {
					auto current_object = JS_GetPropertyUint32(context, argv[2], i);
					auto data_val = JS_GetProperty(context, current_object, data_atom.value);
					auto data_len = std::size_t{};
					auto image_x = static_cast<int>(get_property_bigint64(context, current_object, x_atom.value));
					auto image_y = static_cast<int>(get_property_bigint64(context, current_object, y_atom.value));
					auto image_width = static_cast<int>(get_property_bigint64(context, current_object, atom_width.value));
					auto image_height = static_cast<int>(get_property_bigint64(context, current_object, atom_height.value));
					auto data = JS_GetArrayBuffer(context, &data_len, data_val);
					assert_conditional(data != nullptr, "Cannot get ArrayBuffer from current object, its property is missing", "join_png");
					x_y.emplace_back(Coordinate{.x = image_x, .y = image_y});
					m_data.emplace_back(Image{image_x, image_y, image_width, image_height, std::move(make_list(data, data_len))});
					JS_FreeValue(context, data_val);
					JS_FreeValue(context, current_object);
				}
				for (auto i : Range<size_t>(x_y.size())) {
					m_data[i].x = x_y[i].x;
					m_data[i].y = x_y[i].y;
				}
				Sen::Kernel::Definition::ImageIO::join_png(destination, Sen::Kernel::Definition::Dimension<int>{static_cast<int>(width), static_cast<int>(height)}, m_data);
				return JS_UNDEFINED;
			});
		}

		inline static auto join(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::Object
		{
			using Image = Sen::Kernel::Definition::Image<int>;
			return proxy_wrapper(context, "join", [&](){
				assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "join");
				auto atom_width = Atom{context, "width"};
				auto atom_height = Atom{context, "height"};
				auto atom_length = Atom{context, "length"};
				auto width = get_property_bigint64(context, argv[1], atom_width.value);
				auto height = get_property_bigint64(context, argv[1], atom_height.value);
				auto length = get_property_int32(context, argv[2], atom_length.value);
				auto m_data = List<Sen::Kernel::Definition::Image<int>>{};
				auto x_y = List<Coordinate>{};
				auto x_atom = Atom{context, "x"};
				auto y_atom = Atom{context, "y"};
				auto data_atom = Atom{context, "data"};
				for (auto i : Range<int>(length)) {
					auto current_object = JS_GetPropertyUint32(context, argv[2], i);
					auto data_val = JS_GetProperty(context, current_object, data_atom.value);
					auto data_len = std::size_t{};
					auto image_x = static_cast<int>(get_property_bigint64(context, current_object, x_atom.value));
					auto image_y = static_cast<int>(get_property_bigint64(context, current_object, y_atom.value));
					auto image_width = static_cast<int>(get_property_bigint64(context, current_object, atom_width.value));
					auto image_height = static_cast<int>(get_property_bigint64(context, current_object, atom_height.value));
					auto data = JS_GetArrayBuffer(context, &data_len, data_val);
					assert_conditional(data != nullptr, "Cannot get ArrayBuffer from current object, its property is missing", "join_png");
					x_y.emplace_back(Coordinate{.x = image_x, .y = image_y});
					m_data.emplace_back(Image{image_x, image_y, image_width, image_height, std::move(make_list(data, data_len))});
					JS_FreeValue(context, data_val);
					JS_FreeValue(context, current_object);
				}
				for (auto i : Range<size_t>(x_y.size())) {
					m_data[i].x = x_y[i].x;
					m_data[i].y = x_y[i].y;
				}
				auto destination = Kernel::Definition::Image<int>::transparent(Kernel::Definition::Dimension<int>(static_cast<int>(width), static_cast<int>(height)));
				Sen::Kernel::Definition::ImageIO::join(destination, m_data);
				auto image_obj = JS_NewObject(context);
				auto area_func = JS_NewCFunction2(context, Dimension::area, "area", 0, JS_CFUNC_generic, 0);
				auto circumference_func = JS_NewCFunction2(context, Dimension::circumference, "circumference", 0, JS_CFUNC_generic, 0);
				JS_DefinePropertyValue(context, image_obj, atom_width.value, JS_NewBigInt64(context, destination.width), int{JS_PROP_C_W_E});
				JS_DefinePropertyValue(context, image_obj, atom_height.value, JS_NewBigInt64(context, destination.height), int{JS_PROP_C_W_E});
				auto atom_bit_depth = Atom{context, "bit_depth"};
				JS_DefinePropertyValue(context, image_obj, atom_bit_depth.value, JS_NewBigInt64(context, destination.bit_depth), int{JS_PROP_C_W_E});
				auto atom_color_type = Atom{context, "color_type"};
				JS_DefinePropertyValue(context, image_obj, atom_color_type.value, JS_NewBigInt64(context, destination.color_type), int{JS_PROP_C_W_E});
				auto atom_interlace_type = Atom(context, "interlace_type");
				JS_DefinePropertyValue(context, image_obj, atom_interlace_type.value, JS_NewBigInt64(context, destination.interlace_type), int{JS_PROP_C_W_E});
				auto atom_channels = Atom(context, "channels"); 
				JS_DefinePropertyValue(context, image_obj, atom_channels.value, JS_NewBigInt64(context, destination.channels), int{JS_PROP_C_W_E});
				auto atom_rowbytes = Atom(context, "rowbytes"); 
				JS_DefinePropertyValue(context, image_obj, atom_rowbytes.value, JS_NewBigInt64(context, destination.rowbytes), int{JS_PROP_C_W_E});
				auto atom_data = Atom(context, "data");  
				JS_DefinePropertyValue(context, image_obj, atom_data.value, JS_NewArrayBufferCopy(context, destination.data().data(), destination.data().size()), int{JS_PROP_C_W_E});
				auto atom_area = Atom(context, "area");  
				JS_DefinePropertyValue(context, image_obj, atom_area.value, area_func, JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE);
				auto atom_circumference = Atom(context, "circumference");
				JS_DefinePropertyValue(context, image_obj, atom_circumference.value, circumference_func, JS_PROP_WRITABLE | JS_PROP_CONFIGURABLE);
				return image_obj;
			});
		}

		inline static auto join_extend(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::Object
		{
			using Image = Sen::Kernel::Definition::Image<int>;
			return proxy_wrapper(context, "join_extend", [&](){
				assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "join_extend");
				auto atom_width = Atom{context, "width"};
				auto atom_height = Atom{context, "height"};
				auto atom_length = Atom{context, "length"};
				auto atlas_width = get_property_bigint64(context, argv[1], atom_width.value);
				auto atlas_height = get_property_bigint64(context, argv[1], atom_height.value);
				auto length = get_property_int32(context, argv[2], atom_length.value);
				auto m_data = List<Sen::Kernel::Definition::Image<int>>{};
				auto x_y = List<Coordinate>{};auto x_atom = Atom{context, "x"};
				auto y_atom = Atom{context, "y"};
				auto data_atom = Atom{context, "data"};
				for (auto i : Range<int>(length)) {
					auto current_object = JS_GetPropertyUint32(context, argv[2], i);
					auto data_val = JS_GetProperty(context, current_object, data_atom.value);
					auto data_len = std::size_t{};
					auto image_x = static_cast<int>(get_property_bigint64(context, current_object, x_atom.value));
					auto image_y = static_cast<int>(get_property_bigint64(context, current_object, y_atom.value));
					auto image_width = static_cast<int>(get_property_bigint64(context, current_object, atom_width.value));
					auto image_height = static_cast<int>(get_property_bigint64(context, current_object, atom_height.value));
					auto data = JS_GetArrayBuffer(context, &data_len, data_val);
					assert_conditional(data != nullptr, "Cannot get ArrayBuffer from current object, its property is missing", "join_png");
					x_y.emplace_back(Coordinate{.x = image_x, .y = image_y});
					m_data.emplace_back(Image{image_x, image_y, image_width, image_height, std::move(make_list(data, data_len))});
					JS_FreeValue(context, data_val);
					JS_FreeValue(context, current_object);
				}
				for (auto i : Range<size_t>(x_y.size())) {
					m_data[i].x = x_y[i].x;
					m_data[i].y = x_y[i].y;
				}
				auto destination = Kernel::Definition::Image<int>::transparent(Kernel::Definition::Dimension<int>(atlas_width, atlas_height));
				Sen::Kernel::Definition::ImageIO::join_extend(destination,m_data);
				auto image_obj = JS_NewObject(context);
				auto area_func = JS_NewCFunction2(context, Dimension::area, "area", 0, JS_CFUNC_generic, 0);
				auto circumference_func = JS_NewCFunction2(context, Dimension::circumference, "circumference", 0, JS_CFUNC_generic, 0);
				JS_DefinePropertyValue(context, image_obj, atom_width.value, JS_NewBigInt64(context, destination.width), int{JS_PROP_C_W_E});
				JS_DefinePropertyValue(context, image_obj, atom_height.value, JS_NewBigInt64(context, destination.height), int{JS_PROP_C_W_E});
				auto atom_bit_depth = Atom{context, "bit_depth"};
				JS_DefinePropertyValue(context, image_obj, atom_bit_depth.value, JS_NewBigInt64(context, destination.bit_depth), int{JS_PROP_C_W_E});
				auto atom_color_type = Atom{context, "color_type"};
				JS_DefinePropertyValue(context, image_obj, atom_color_type.value, JS_NewBigInt64(context, destination.color_type), int{JS_PROP_C_W_E});
				auto atom_interlace_type = Atom(context, "interlace_type");
				JS_DefinePropertyValue(context, image_obj, atom_interlace_type.value, JS_NewBigInt64(context, destination.interlace_type), int{JS_PROP_C_W_E});
				auto atom_channels = Atom(context, "channels"); 
				JS_DefinePropertyValue(context, image_obj, atom_channels.value, JS_NewBigInt64(context, destination.channels), int{JS_PROP_C_W_E});
				auto atom_rowbytes = Atom(context, "rowbytes"); 
				JS_DefinePropertyValue(context, image_obj, atom_rowbytes.value, JS_NewBigInt64(context, destination.rowbytes), int{JS_PROP_C_W_E});
				auto atom_data = Atom(context, "data");  
				JS_DefinePropertyValue(context, image_obj, atom_data.value, JS_NewArrayBufferCopy(context, destination.data().data(), destination.data().size()), int{JS_PROP_C_W_E});
				return image_obj;
			});
		}

		inline static auto resize_fs(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::undefined
		{
			return proxy_wrapper(context, "resize_fs", [&](){
				assert_conditional(argc == 3, fmt::format("{} 3, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "resize_fs");
				auto source = JS::Converter::get_string(context, argv[0]);
				auto destination = JS::Converter::get_string(context, argv[1]);
				auto percentage = JS::Converter::get_float32(context, argv[2]);
				Sen::Kernel::Definition::ImageIO::resize_png(source, destination, percentage);
				return JS_UNDEFINED;
			});
		}

		inline static auto rotate_fs(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::undefined
		{
			return proxy_wrapper(context, "rotate_fs", [&](){
				assert_conditional(argc == 3, fmt::format("{} 3, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "rotate_fs");
				auto source = JS::Converter::get_string(context, argv[0]);
				auto destination = JS::Converter::get_string(context, argv[1]);
				auto percentage = JS::Converter::get_float64(context, argv[2]);
				Sen::Kernel::Definition::ImageIO::rotate_png(source, destination, percentage);
				return JS_UNDEFINED;
			});
		}

		inline static auto cut_fs(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::undefined
		{
			return proxy_wrapper(context, "cut_fs", [&](){
				assert_conditional(argc == 3, fmt::format("{} 3, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "cut_fs");
				auto source = JS::Converter::get_string(context, argv[0]);
				auto destination = JS::Converter::get_string(context, argv[1]);
				auto width_atom = Atom{context, "width"};
				auto height_atom = Atom{context, "height"};
				auto x_atom = Atom{context, "x"};
				auto y_atom = Atom{context, "y"};
				auto rectangle_width = get_property_int32(context, argv[2], width_atom.value);
				auto rectangle_height = get_property_int32(context, argv[2], height_atom.value);
				auto rectangle_x = get_property_int32(context, argv[2], x_atom.value);
				auto rectangle_y = get_property_int32(context, argv[2], y_atom.value);
				Sen::Kernel::Definition::ImageIO::cut_fs(source, destination, Sen::Kernel::Definition::Rectangle<int>(rectangle_x, rectangle_y, rectangle_width, rectangle_height));
				return JS_UNDEFINED;
			});
		}

		inline static auto cut_multiple_fs(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::undefined
		{
			return proxy_wrapper(context, "cut_multiple_fs", [&](){
				assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "cut_multiple_fs");
				auto data = List<Sen::Kernel::Definition::RectangleFileIO<int>>{};
				if (JS_IsArray(context, argv[1])) {
					auto length_atom = Atom{context, "length"};
					auto length = get_property_int32(context, argv[1], length_atom.value);
					auto width_atom = Atom{context, "width"};
					auto height_atom = Atom{context, "height"};
					auto x_atom = Atom{context, "x"};
					auto y_atom = Atom{context, "y"};
					auto destination_atom = Atom{context, "destination"};
					for (auto i : Range<uint32_t>(length)) {
						auto current_object = JS_GetPropertyUint32(context, argv[1], i);
						auto rectangle_width = get_property_int32(context, current_object, width_atom.value);
						auto rectangle_height = get_property_int32(context, current_object, height_atom.value);
						auto rectangle_x = get_property_int32(context, current_object, x_atom.value);
						auto rectangle_y = get_property_int32(context, current_object, y_atom.value);
						auto destination = get_property_string(context, current_object, destination_atom.value);
						data.emplace_back(Sen::Kernel::Definition::RectangleFileIO<int>(rectangle_x, rectangle_y, rectangle_width, rectangle_height, destination));
						JS_FreeValue(context, current_object);
					}
				} else {
					throw Exception("Cannot read property \"length\" of undefined", std::source_location::current(), "cut_multiple_fs");
				}
				auto source = JS::Converter::get_string(context, argv[0]);
				Sen::Kernel::Definition::ImageIO::cut_pngs(source, data);
				return JS_UNDEFINED;
			});
		}

		/**
		 * ----------------------------------------
		 * JavaScript rotate image
		 * @param argv[0]: source file
		 * @param argv[1]: destination file
		 * @param argv[2]: percentage
		 * @return: UNDEFINED
		 * ----------------------------------------
		 */

		inline static auto cut_multiple_fs_asynchronous(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::undefined
		{
			return proxy_wrapper(context, "cut_multiple_fs_asynchronous", [&](){
				assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "cut_multiple_fs");
				auto data = List<Sen::Kernel::Definition::RectangleFileIO<int>>{};
				if (JS_IsArray(context, argv[1])) {
					auto length_atom = Atom{context, "length"};
					auto length = get_property_int32(context, argv[1], length_atom.value);
					auto width_atom = Atom{context, "width"};
					auto height_atom = Atom{context, "height"};
					auto x_atom = Atom{context, "x"};
					auto y_atom = Atom{context, "y"};
					auto destination_atom = Atom{context, "destination"};
					for (auto i : Range<uint32_t>(length)) {
						auto current_object = JS_GetPropertyUint32(context, argv[1], i);
						auto rectangle_width = get_property_int32(context, current_object, width_atom.value);
						auto rectangle_height = get_property_int32(context, current_object, height_atom.value);
						auto rectangle_x = get_property_int32(context, current_object, x_atom.value);
						auto rectangle_y = get_property_int32(context, current_object, y_atom.value);
						auto destination = get_property_string(context, current_object, destination_atom.value);
						data.emplace_back(Sen::Kernel::Definition::RectangleFileIO<int>(rectangle_x, rectangle_y, rectangle_width, rectangle_height, destination));
						JS_FreeValue(context, current_object);
					}
				} else {
					throw Exception("Cannot read property \"length\" of undefined", std::source_location::current(), "cut_multiple_fs_asynchronous");
				}
				auto source = JS::Converter::get_string(context, argv[0]);
				Sen::Kernel::Definition::ImageIO::cut_pngs_asynchronous(source, data);
				return JS_UNDEFINED;
			});
		}

	}

	namespace JavaScript
	{

		inline static auto evaluate(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::any
		{
			return proxy_wrapper(context, "evaluate", [&](){
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "evaluate");
				auto source = JS::Converter::get_string(context, argv[0]);
				auto m_value = JS_Eval(context, source.data(), source.size(), "unknown", JS_EVAL_TYPE_GLOBAL);
				return m_value;
			});
		}

		inline static auto evaluate_fs(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::any
		{
			return proxy_wrapper(context, "evaluate", [&](){
				auto source = JS::Converter::get_string(context, argv[0]);
				auto js_source = Sen::Kernel::FileSystem::read_file(source);
				auto m_value = JS_Eval(context, js_source.data(), js_source.size(), source.data(), JS_EVAL_TYPE_GLOBAL);
				return m_value;
			});
		}
	}

	namespace Encryption
	{

		namespace Rijndael
		{

			inline static auto encrypt(
				JSContext *context,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::ArrayBuffer
			{
				using Mode = Kernel::Definition::Encryption::Rijndael::Mode;
				static constexpr auto callback = []<auto mode>(
													 const List<std::uint8_t> &plain,
													 std::string_view key,
													 std::string_view iv) -> List<std::uint8_t>
				{
					static_assert(mode == Mode::CBC or mode == Mode::CFB or mode == Mode::ECB, "mode must be cbc or cfb or ecb");
					return Kernel::Definition::Encryption::Rijndael::encrypt<std::size_t, Mode::CBC>(reinterpret_cast<char const *>(plain.data()), key, iv, plain.size());
				};
				return proxy_wrapper(context, "encrypt", [&](){
					assert_conditional(argc == 4, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "encrypt");
						auto source = JS::Converter::to_binary_list(context, argv[0]);
						auto key = JS::Converter::get_string(context, argv[1]);
						auto iv = JS::Converter::get_string(context, argv[2]);
						auto mode = JS::Converter::get_bigint64(context, argv[3]);
						switch (static_cast<Mode>(mode)) {
							case Mode::CBC: {
								return JS::Converter::toArrayBuffer(context, callback.operator() <Mode::CBC>(source, key, iv));
							}
							case Mode::CFB: {
								return JS::Converter::toArrayBuffer(context, callback.operator() <Mode::CFB>(source, key, iv));
							}
							case Mode::ECB: {
								return JS::Converter::toArrayBuffer(context, callback.operator() <Mode::ECB>(source, key, iv));
							}
							default: {
								assert_conditional(false, "No mode available for this option, expected: CBC, CFB, ECB but received unknown mode", "encrypt");
							}
						}
				});
			}

			inline static auto decrypt(
				JSContext *context,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::ArrayBuffer
			{
				using Mode = Kernel::Definition::Encryption::Rijndael::Mode;
				static constexpr auto callback = []<auto mode>(
													 const List<std::uint8_t> &plain,
													 std::string_view key,
													 std::string_view iv) -> List<std::uint8_t>
				{
					static_assert(mode == Mode::CBC or mode == Mode::CFB or mode == Mode::ECB, "mode must be cbc or cfb or ecb");
					return Kernel::Definition::Encryption::Rijndael::decrypt<std::size_t, Mode::CBC>(reinterpret_cast<char const *>(plain.data()), key, iv, plain.size());
				};
				return proxy_wrapper(context, "decrypt", [&](){
					assert_conditional(argc == 4, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "decrypt");
					auto source = JS::Converter::to_binary_list(context, argv[0]);
					auto key = JS::Converter::get_string(context, argv[1]);
					auto iv = JS::Converter::get_string(context, argv[2]);
					auto mode = JS::Converter::get_bigint64(context, argv[3]);
					switch (static_cast<Mode>(mode)) {
						case Mode::CBC: {
							return JS::Converter::toArrayBuffer(context, callback.operator() <Mode::CBC> (source, key, iv));
						}
						case Mode::CFB: {
							return JS::Converter::toArrayBuffer(context, callback.operator() <Mode::CFB> (source, key, iv));
						}
						case Mode::ECB: {
							return JS::Converter::toArrayBuffer(context, callback.operator() <Mode::ECB> (source, key, iv));
						}
						default: {
							assert_conditional(false, "No mode available for this option, expected: CBC, CFB, ECB but received unknown mode", "encrypt");
						}
					}
				});
			}

		}

		namespace FNV
		{

			inline static auto hash(
				JSContext *context,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::bigint
			{
				return proxy_wrapper(context, "hash", [&](){
					assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "hash");
					auto source = JS::Converter::get_string(context, argv[0]);
					auto result = Kernel::Definition::Encryption::FNV::Hash<std::uint32_t>{}.make_hash(source.data());
					return JS::Converter::to_bigint(context, result);
				});
			}

			inline static auto hash_fs(
				JSContext *context,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::bigint
			{
				return proxy_wrapper(context, "hash_fs", [&](){
					assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "hash_fs");
					auto source = JS::Converter::get_string(context, argv[0]);
					auto result = Sen::Kernel::Definition::Encryption::FNV::Hash<std::uint32_t>::hash_fs(source.data());
					return JS::Converter::to_bigint(context, result);
				});
			}

		}

		namespace MD5
		{

			inline static auto hash(
				JSContext *context,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::string
			{
				return proxy_wrapper(context, "hash", [&](){
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "hash");
					auto source = JS::Converter::get_string(context, argv[0]);
					auto result = Kernel::Definition::Encryption::MD5::hash(Kernel::FileSystem::read_binary<unsigned char>(source));
					return JS::Converter::to_string(context, result);
				});
			}

			inline static auto hash_fs(
				JSContext *context,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::string
			{
				return proxy_wrapper(context, "hash_fs", [&](){
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "hash_fs");
					auto source = JS::Converter::get_string(context, argv[0]);
					auto result = Sen::Kernel::Definition::Encryption::MD5::hash_fs(source);
					return JS::Converter::to_string(context, result);
				});
			}
		}

		namespace Base64
		{

			inline static auto encode(
				JSContext *context,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::string
			{
				return proxy_wrapper(context, "encode", [&](){
					assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "encode");
					auto source = JS::Converter::get_string(context, argv[0]);
					auto result = Sen::Kernel::Definition::Encryption::Base64::encode(source);
					return JS::Converter::to_string(context, result);
				});
			}

			inline static auto encode_fs_as_multiple_threads(
				JSContext *context,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return proxy_wrapper(context, "encode_fs_as_multiple_threads", [&](){
					auto paths = List<List<std::string>>{};
					for (const auto &i : Range<int>(argc))
					{
						const auto &data = JS::Converter::get_vector<std::string>(context, argv[i]);
						paths.emplace_back(data);
					}
					Sen::Kernel::Definition::Encryption::Base64::encode_fs_as_multiple_thread(paths);
					return JS_UNDEFINED;
				});
			}

			inline static auto decode_fs_as_multiple_threads(
				JSContext *context,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return proxy_wrapper(context, "decode_fs_as_multiple_threads", [&](){
					auto paths = List<List<std::string>>{};
					for (const auto &i : Range<int>(argc))
					{
						const auto &data = JS::Converter::get_vector<std::string>(context, argv[i]);
						paths.emplace_back(data);
					}
					Sen::Kernel::Definition::Encryption::Base64::encode_fs_as_multiple_thread(paths);
					return JS_UNDEFINED;
				});
			}

			inline static auto decode(
				JSContext *context,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::string
			{
				return proxy_wrapper(context, "decode", [&](){
					assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "decode");
					auto source = JS::Converter::get_string(context, argv[0]);
					auto result = Sen::Kernel::Definition::Encryption::Base64::decode(source);
					return JS::Converter::to_string(context, result);
				});
			}

			inline static auto encode_fs(
				JSContext *context,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return proxy_wrapper(context, "encode_fs", [&](){
					assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "encode_fs");
					auto source = JS::Converter::get_string(context, argv[0]);
					auto destination = JS::Converter::get_string(context, argv[1]);
					Sen::Kernel::Definition::Encryption::Base64::encode_fs(source, destination);
					return JS_UNDEFINED;
				});
			}

			inline static auto decode_fs(
				JSContext *context,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return proxy_wrapper(context, "decode_fs", [&](){
					assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "decode_fs");
					auto source = JS::Converter::get_string(context, argv[0]);
					auto destination = JS::Converter::get_string(context, argv[1]);
					Sen::Kernel::Definition::Encryption::Base64::decode_fs(source, destination);
					return JS_UNDEFINED; 
				});
			}
		}

		namespace SHA224
		{

			inline static auto hash(
				JSContext *context,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::string
			{
				return proxy_wrapper(context, "hash", [&](){
					assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "hash");
					auto source = JS::Converter::get_string(context, argv[0]);
					auto result = Sen::Kernel::Definition::Encryption::Sha224::hash(source);
					return JS::Converter::to_string(context, result);
				});
			}

			inline static auto hash_fs(
				JSContext *context,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::string
			{
				return proxy_wrapper(context, "hash_fs", [&](){
					assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "hash_fs");
					auto source = JS::Converter::get_string(context, argv[0]);
					auto result = Sen::Kernel::Definition::Encryption::Sha224::hash_fs(source);
					return JS::Converter::to_string(context, result);
				});
			}

		}

		namespace SHA256
		{

			inline static auto hash(
				JSContext *context,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::string
			{
				return proxy_wrapper(context, "hash", [&](){
					assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "hash");
					auto source = JS::Converter::get_string(context, argv[0]);
					auto result = Sen::Kernel::Definition::Encryption::SHA256::hash(source);
					return JS::Converter::to_string(context, result);
				});
			}

			inline static auto hash_fs(
				JSContext *context,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::string
			{
				return proxy_wrapper(context, "hash_fs", [&](){
					assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "hash_fs");
					auto source = JS::Converter::get_string(context, argv[0]);
					auto result = Sen::Kernel::Definition::Encryption::SHA256::hash_fs(source);
					return JS::Converter::to_string(context, result);
				});
			}
		}

		namespace SHA384
		{

			inline static auto hash(
				JSContext *context,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::string
			{
				return proxy_wrapper(context, "hash", [&](){
					assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "hash");
					auto source = JS::Converter::get_string(context, argv[0]);
					auto result = Sen::Kernel::Definition::Encryption::SHA384::hash(source);
					return JS::Converter::to_string(context, result);
				});
			}

			inline static auto hash_fs(
				JSContext *context,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::string
			{
				return proxy_wrapper(context, "hash_fs", [&](){
					assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "hash_fs");
					auto source = JS::Converter::get_string(context, argv[0]);
					auto result = Sen::Kernel::Definition::Encryption::SHA384::hash_fs(source);
					return JS::Converter::to_string(context, result);
				});
			}
		}

		namespace SHA512
		{

			inline static auto hash(
				JSContext *context,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::string
			{
				return proxy_wrapper(context, "hash", [&](){
					assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "hash");
					auto source = JS::Converter::get_string(context, argv[0]);
					auto result = Sen::Kernel::Definition::Encryption::SHA512::hash(source);
					return JS::Converter::to_string(context, result);
				});
			}

			inline static auto hash_fs(
				JSContext *context,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::string
			{
				return proxy_wrapper(context, "hash", [&](){
					assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "hash_fs");
					auto source = JS::Converter::get_string(context, argv[0]);
					auto result = Sen::Kernel::Definition::Encryption::SHA512::hash_fs(source);
					return JS::Converter::to_string(context, result);
				});
			}
		}

		namespace XOR
		{

			inline static auto encrypt(
				JSContext *context,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::ArrayBuffer
			{
				return proxy_wrapper(context, "encrypt", [&](){
					assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "encrypt");
					auto plain = JS::Converter::to_binary_list(context, argv[0]);
					auto key = JS::Converter::to_binary_list(context, argv[1]);
					auto result = Sen::Kernel::Definition::Encryption::XOR::encrypt(plain, key);
					return JS::Converter::toArrayBuffer(context, result);
				});
			}

			inline static auto encrypt_fs(
				JSContext *context,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return proxy_wrapper(context, "encrypt_fs", [&](){
					assert_conditional(argc == 3, fmt::format("{} 3, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "encrypt_fs");
					auto source = JS::Converter::get_string(context, argv[0]);
					auto destination = JS::Converter::get_string(context, argv[1]);
					auto key = JS::Converter::to_binary_list(context, argv[2]);
					Sen::Kernel::Definition::Encryption::XOR::encrypt_fs(source, destination, key);
					return JS_UNDEFINED;
				});
			}

		}
	}

	namespace Compression
	{

		namespace Zip
		{

			namespace Compress
			{

				inline static auto directory(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "directory", [&](){
						assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "directory");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						Sen::Kernel::Definition::Compression::Zip::Compress::directory(source, destination);
						return JS_UNDEFINED;
					});
				}

				inline static auto file(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "file", [&](){
						assert_conditional(argc == 3 || argc == 2, fmt::format("argument expected {} but received {}", "2 or 3", argc), "file");
						auto source = JS::Converter::get_vector<std::string>(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						if (argc == 3) {
							auto root = JS::Converter::get_string(context, argv[2]);
							Sen::Kernel::Definition::Compression::Zip::Compress::file(source, destination, root);
						}
						else {
							Sen::Kernel::Definition::Compression::Zip::Compress::file(source, destination);
						}
						return JS_UNDEFINED;
					});
				}

			}

			namespace Uncompress
			{

				inline static auto process(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "process", [&](){
						assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "process");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						Sen::Kernel::Definition::Compression::Zip::Uncompress::process(source, destination);
						return JS_UNDEFINED;
					});
				}

			}

		}

		namespace Zlib
		{

			inline static auto compress_fs(
				JSContext *context,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return proxy_wrapper(context, "compress_fs", [&](){
					assert_conditional(argc == 3, fmt::format("{} 3, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "compress_fs");
					auto source = JS::Converter::get_string(context, argv[0]);
					auto destination = JS::Converter::get_string(context, argv[1]);
					auto level = JS::Converter::get_int32(context, argv[2]);
					if (!(static_cast<int>(Sen::Kernel::Definition::Compression::Zlib::Level::DEFAULT) <= level or level <= static_cast<int>(Sen::Kernel::Definition::Compression::Zlib::Level::LEVEL_9)))
					{
						throw Exception(fmt::format("{}, {} {}", Kernel::Language::get("zlib.compress.invalid_level"), Kernel::Language::get("but_received"), level), std::source_location::current(), "compress_fs");
					}
					Sen::Kernel::Definition::Compression::Zlib::compress_fs(source, destination, static_cast<Sen::Kernel::Definition::Compression::Zlib::Level>(level));
					return JS_UNDEFINED;
				});
			}

			inline static auto uncompress_fs(
				JSContext *context,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return proxy_wrapper(context, "uncompress_fs", [&](){
					assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "uncompress_fs");
					auto source = JS::Converter::get_string(context, argv[0]);
					auto destination = JS::Converter::get_string(context, argv[1]);
					Sen::Kernel::Definition::Compression::Zlib::uncompress_fs(source, destination);
					return JS_UNDEFINED;
				});
			}

			inline static auto uncompress(
				JSContext *context,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSDefine::BinaryView
			{
				using Data = Class::BinaryView::Data;
				return proxy_wrapper(context, "uncompress", [&](){
					assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "uncompress");
					auto s = static_cast<Data*>(JS_GetOpaque2(context, argv[0], Class::BinaryView::class_id));
					if (s == nullptr) {
						return JS_EXCEPTION;
					}
					auto sub = std::make_unique<Data>(Sen::Kernel::Definition::Compression::Zlib::uncompress(s->value));
					auto global_obj = JS_GetGlobalObject(context);
					auto sen_atom = Atom{context, "Sen"};
					auto kernel_atom = Atom{context, "Kernel"};
					auto binary_view_atom = Atom{context, "BinaryView"};
					auto proto_atom = Atom{context, "prototype"};
					auto sen_obj = JS_GetProperty(context, global_obj, sen_atom.value);
					JS_FreeValue(context, global_obj);
					auto kernel_obj = JS_GetProperty(context, sen_obj, kernel_atom.value);
					JS_FreeValue(context, sen_obj);
					auto binary_ctor = JS_GetProperty(context, kernel_obj, binary_view_atom.value);
					JS_FreeValue(context, kernel_obj);
					auto proto = JS_GetProperty(context, binary_ctor, proto_atom.value);
					JS_FreeValue(context, binary_ctor);
					if (JS_IsException(proto)) {
						throw Exception("not a constructor", std::source_location::current(), "uncompress");
					}
					auto obj = JS_NewObjectProtoClass(context, proto, Class::BinaryView::class_id);
					JS_FreeValue(context, proto);
					if (JS_IsException(obj)) {
						throw Exception("Cannot get BinaryView class from its prototype", std::source_location::current(), "uncompress");
					}
					JS_SetOpaque(obj, sub.release());
					return obj;
				});
			}

			inline static auto compress(
				JSContext *context,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSDefine::BinaryView
			{
				using Data = Class::BinaryView::Data;
				using Level = Definition::Compression::Zlib::Level;
				using Zlib = Definition::Compression::Zlib;
				return proxy_wrapper(context, "compress", [&](){
					assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "compress");
					auto s = static_cast<Data*>(JS_GetOpaque2(context, argv[0], Class::BinaryView::class_id));
					if (s == nullptr) {
						return JS_EXCEPTION;
					}
					auto sub = std::unique_ptr<Data, decltype(Class::make_deleter<Data>())>{nullptr, Class::make_deleter<Data>()};
					switch (static_cast<Level>(JS::Converter::get_int32(context, argv[1]))) {
						case Level::DEFAULT:{
							sub.reset(new Data(Zlib::compress_deflate<Level::LEVEL_6>(s->value)));
							break;
						}
						case Level::LEVEL_0:{
							sub.reset(new Data(Zlib::compress_deflate<Level::LEVEL_0>(s->value)));
							break;
						}
						case Level::LEVEL_1:{
							sub.reset(new Data(Zlib::compress_deflate<Level::LEVEL_1>(s->value)));
							break;
						}
						case Level::LEVEL_2:{
							sub.reset(new Data(Zlib::compress_deflate<Level::LEVEL_2>(s->value)));
							break;
						}
						case Level::LEVEL_3:{
							sub.reset(new Data(Zlib::compress_deflate<Level::LEVEL_3>(s->value)));
							break;
						}
						case Level::LEVEL_4:{
							sub.reset(new Data(Zlib::compress_deflate<Level::LEVEL_4>(s->value)));
							break;
						}
						case Level::LEVEL_5:{
							sub.reset(new Data(Zlib::compress_deflate<Level::LEVEL_5>(s->value)));
							break;
						}
						case Level::LEVEL_6:{
							sub.reset(new Data(Zlib::compress_deflate<Level::LEVEL_6>(s->value)));
							break;
						}
						case Level::LEVEL_7:{
							sub.reset(new Data(Zlib::compress_deflate<Level::LEVEL_7>(s->value)));
							break;
						}
						case Level::LEVEL_8:{
							sub.reset(new Data(Zlib::compress_deflate<Level::LEVEL_8>(s->value)));
							break;
						}
						case Level::LEVEL_9:{
							sub.reset(new Data(Zlib::compress_deflate<Level::LEVEL_9>(s->value)));
							break;
						}
						default:{
							throw Exception(fmt::format("{}", Kernel::Language::get("zlib.compress.invalid_level")), std::source_location::current(), "compress");
						}
					}
					auto global_obj = JS_GetGlobalObject(context);
					auto sen_atom = Atom{context, "Sen"};
					auto kernel_atom = Atom{context, "Kernel"};
					auto binary_view_atom = Atom{context, "BinaryView"};
					auto proto_atom = Atom{context, "prototype"};
					auto sen_obj = JS_GetProperty(context, global_obj, sen_atom.value);
					JS_FreeValue(context, global_obj);
					auto kernel_obj = JS_GetProperty(context, sen_obj, kernel_atom.value);
					JS_FreeValue(context, sen_obj);
					auto binary_ctor = JS_GetProperty(context, kernel_obj, binary_view_atom.value);
					JS_FreeValue(context, kernel_obj);
					auto proto = JS_GetProperty(context, binary_ctor, proto_atom.value);
					JS_FreeValue(context, binary_ctor);
					if (JS_IsException(proto)) {
						throw Exception("not a constructor", std::source_location::current(), "compress");
					}
					auto obj = JS_NewObjectProtoClass(context, proto, Class::BinaryView::class_id);
					JS_FreeValue(context, proto);
					if (JS_IsException(obj)) {
						throw Exception("Cannot get BinaryView class from its prototype", std::source_location::current(), "compress");
					}
					JS_SetOpaque(obj, sub.release());
					return obj;
				});
			}

		}

		namespace Bzip2
		{

			inline static auto compress_fs(
				JSContext *context,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return proxy_wrapper(context, "compress_fs", [&](){
					assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "compress_fs");
					auto source = JS::Converter::get_string(context, argv[0]);
					auto destination = JS::Converter::get_string(context, argv[1]);
					Sen::Kernel::Definition::Compression::Bzip2::compress_fs(source, destination);
					return JS_UNDEFINED;
				});
			}

			inline static auto uncompress_fs(
				JSContext *context,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return proxy_wrapper(context, "uncompress_fs", [&](){
					assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "uncompress_fs");
					auto source = JS::Converter::get_string(context, argv[0]);
					auto destination = JS::Converter::get_string(context, argv[1]);
					Sen::Kernel::Definition::Compression::Bzip2::uncompress_fs(source, destination);
					return JS_UNDEFINED;
				});
			}

		}

		namespace Lzma
		{

			inline static auto compress_fs(
				JSContext *context,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return proxy_wrapper(context, "compress_fs", [&](){
					assert_conditional(argc == 3, fmt::format("{} 3, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "compress_fs");
					auto source = JS::Converter::get_string(context, argv[0]);
					auto destination = JS::Converter::get_string(context, argv[1]);
					auto level = JS::Converter::get_bigint64(context, argv[2]);
					Sen::Kernel::Definition::Compression::Lzma::compress_fs(source, destination, static_cast<Sen::Kernel::Definition::Compression::Lzma::Level>(level));
					return JS_UNDEFINED;
				});
			}

			inline static auto uncompress_fs(
				JSContext *context,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return proxy_wrapper(context, "uncompress_fs", [&](){
					assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "uncompress_fs");
					auto source = JS::Converter::get_string(context, argv[0]);
					auto destination = JS::Converter::get_string(context, argv[1]);
					Sen::Kernel::Definition::Compression::Lzma::uncompress_fs(source, destination);
					return JS_UNDEFINED;
				});
			}

		}

		namespace Gzip
		{

			inline static auto compress_fs(
				JSContext *context,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return proxy_wrapper(context, "compress_fs", [&](){
					assert_conditional(argc == 3, fmt::format("{} 3, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "compress_fs");
					auto source = JS::Converter::get_string(context, argv[0]);
					auto destination = JS::Converter::get_string(context, argv[1]);
					auto level = JS::Converter::get_int32(context, argv[2]);
					if (!(static_cast<int>(Sen::Kernel::Definition::Compression::Zlib::Level::DEFAULT) <= level or level <= static_cast<int>(Sen::Kernel::Definition::Compression::Zlib::Level::LEVEL_9)))
					{
						throw std::invalid_argument(fmt::format("{}, {} {}", Kernel::Language::get("zlib.compress.invalid_level"), Kernel::Language::get("but_received"), level));
					}
					Sen::Kernel::Definition::Compression::Zlib::compress_gzip_fs(source, destination, static_cast<Sen::Kernel::Definition::Compression::Zlib::Level>(level));
					return JS_UNDEFINED;
				});
			}

			inline static auto uncompress_fs(
				JSContext *context,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return proxy_wrapper(context, "uncompress_fs", [&](){
					assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "uncompress_fs");
					auto source = JS::Converter::get_string(context, argv[0]);
					auto destination = JS::Converter::get_string(context, argv[1]);
					Sen::Kernel::Definition::Compression::Zlib::uncompress_gzip_fs(source, destination);
					return JS_UNDEFINED;
				});
			}
		}
	}

	namespace Support
	{

		namespace Texture
		{

			inline static auto decode_fs(
				JSContext *context,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return proxy_wrapper(context, "decode_fs", [&](){
					assert_conditional(argc == 5, fmt::format("{} 5, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "decode_fs");
					auto source = JS::Converter::get_string(context, argv[0]);
					auto destination = JS::Converter::get_string(context, argv[1]);
					auto width = JS::Converter::get_bigint64(context, argv[2]);
					auto height = JS::Converter::get_bigint64(context, argv[3]);
					auto format = JS::Converter::get_int32(context, argv[4]);
					Sen::Kernel::Support::Texture::InvokeMethod::decode_fs(source, destination, static_cast<int>(width), static_cast<int>(height), static_cast<Sen::Kernel::Support::Texture::Format>(format));
					return JS_UNDEFINED;
				});
			}

			inline static auto encode_fs(
				JSContext *context,
				JSValueConst this_val,
				int argc,
				JSValueConst *argv
			) -> JSElement::undefined
			{
				return proxy_wrapper(context, "encode_fs", [&](){
					assert_conditional(argc == 3, fmt::format("{} 3, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "encode_fs");
					auto source = JS::Converter::get_string(context, argv[0]);
					auto destination = JS::Converter::get_string(context, argv[1]);
					auto format = JS::Converter::get_int32(context, argv[2]);
					Sen::Kernel::Support::Texture::InvokeMethod::encode_fs(source, destination, static_cast<Sen::Kernel::Support::Texture::Format>(format));
					return JS_UNDEFINED;
				});
			}

		}

		namespace Marmalade {

			namespace DZip
			{

				inline static auto unpack_fs(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "unpack_fs", [&](){
						assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "unpack_fs");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						Kernel::Support::Marmalade::DZip::Unpack::process_fs(source, destination);
						return JS_UNDEFINED;
					});
				}

				inline static auto pack_fs(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "pack_fs", [&](){
						assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "pack_fs");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						Kernel::Support::Marmalade::DZip::Pack::process_fs(source, destination);
						return JS_UNDEFINED;
					});
				}
			}
		}
		

		namespace PopCap
		{

			namespace Zlib
			{

				inline static auto compress_fs(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "pack_fs", [&](){
						assert_conditional(argc == 3, fmt::format("{} 3, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "compress_fs");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						auto use_64_bit_variant = JS::Converter::get_bool(context, argv[2]);
						if (use_64_bit_variant) {
							Sen::Kernel::Support::PopCap::Zlib::Compress<true>::compress_fs(source, destination);
						}
						else {
							Sen::Kernel::Support::PopCap::Zlib::Compress<false>::compress_fs(source, destination);
						}
						return JS_UNDEFINED;
					});
				}

				inline static auto compress(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					using Data = Class::BinaryView::Data;
					return proxy_wrapper(context, "pack_fs", [&](){
						assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "pack_fs");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						Kernel::Support::Marmalade::DZip::Pack::process_fs(source, destination);
						return JS_UNDEFINED;
					});
					using CompressPointer = std::unique_ptr<Sen::Kernel::Support::PopCap::Zlib::VirtualC, decltype(Class::make_deleter<Sen::Kernel::Support::PopCap::Zlib::VirtualC>())>;
					return proxy_wrapper(context, "compress", [&](){
						assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "compress");
						auto source = JS::Converter::to_binary_list(context, argv[0]);
						auto use_64_bit_variant = JS::Converter::get_bool(context, argv[1]);
						auto compressor = CompressPointer(nullptr, Class::make_deleter<Sen::Kernel::Support::PopCap::Zlib::VirtualC>());
						if (use_64_bit_variant) {
							compressor.reset(new Sen::Kernel::Support::PopCap::Zlib::Compress<true>());
						}
						else {
							compressor.reset(new Sen::Kernel::Support::PopCap::Zlib::Compress<false>());
						}
						auto sub = std::make_unique<Data>(compressor->compress(source));
						auto global_obj = JS_GetGlobalObject(context);
						auto sen_atom = Atom{context, "Sen"};
						auto kernel_atom = Atom{context, "Kernel"};
						auto binary_view_atom = Atom{context, "BinaryView"};
						auto proto_atom = Atom{context, "prototype"};
						auto sen_obj = JS_GetProperty(context, global_obj, sen_atom.value);
						JS_FreeValue(context, global_obj);
						auto kernel_obj = JS_GetProperty(context, sen_obj, kernel_atom.value);
						JS_FreeValue(context, sen_obj);
						auto binary_ctor = JS_GetProperty(context, kernel_obj, binary_view_atom.value);
						JS_FreeValue(context, kernel_obj);
						auto proto = JS_GetProperty(context, binary_ctor, proto_atom.value);
						JS_FreeValue(context, binary_ctor);
						if (JS_IsException(proto)) {
							assert_conditional(false, "not a constructor", "compress");
						}
						auto obj = JS_NewObjectProtoClass(context, proto, Class::BinaryView::class_id);
						JS_FreeValue(context, proto);
						if (JS_IsException(obj)) {
							assert_conditional(false, "Cannot call constructor of BinaryView class", "compress");
						}
						JS_SetOpaque(obj, sub.release());
						return obj;
					});
				}

				inline static auto uncompress_fs(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "uncompress_fs", [&](){
						assert_conditional(argc == 3, fmt::format("{} 3, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "uncompress_fs");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						auto use_64_bit_variant = JS::Converter::get_bool(context, argv[2]);
						if (use_64_bit_variant) {
							Sen::Kernel::Support::PopCap::Zlib::Uncompress<true>::uncompress_fs(source, destination);
						}
						else {
							Sen::Kernel::Support::PopCap::Zlib::Uncompress<false>::uncompress_fs(source, destination);
						}
						return JS_UNDEFINED;
					});
				}

				inline static auto uncompress(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSDefine::BinaryView
				{
					using Data = Class::BinaryView::Data;
					using UncompressPointer = std::unique_ptr<Sen::Kernel::Support::PopCap::Zlib::Virtual, decltype(Class::make_deleter<Sen::Kernel::Support::PopCap::Zlib::Virtual>())>;
					return proxy_wrapper(context, "uncompress", [&](){
						assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "uncompress");
						auto source = JS::Converter::to_binary_list(context, argv[0]);
						auto use_64_bit_variant = JS::Converter::get_bool(context, argv[1]);
						auto compressor = UncompressPointer(nullptr, Class::make_deleter<Sen::Kernel::Support::PopCap::Zlib::Virtual>());
						if (use_64_bit_variant) {
							compressor.reset(new Sen::Kernel::Support::PopCap::Zlib::Uncompress<true>());
						}
						else {
							compressor.reset(new Sen::Kernel::Support::PopCap::Zlib::Uncompress<false>());
						}
						auto sub = std::make_unique<Data>(compressor->uncompress(source));
						auto global_obj = JS_GetGlobalObject(context);
						auto sen_atom = Atom{context, "Sen"};
						auto kernel_atom = Atom{context, "Kernel"};
						auto binary_view_atom = Atom{context, "BinaryView"};
						auto proto_atom = Atom{context, "prototype"};
						auto sen_obj = JS_GetProperty(context, global_obj, sen_atom.value);
						JS_FreeValue(context, global_obj);
						auto kernel_obj = JS_GetProperty(context, sen_obj, kernel_atom.value);
						JS_FreeValue(context, sen_obj);
						auto binary_ctor = JS_GetProperty(context, kernel_obj, binary_view_atom.value);
						JS_FreeValue(context, kernel_obj);
						auto proto = JS_GetProperty(context, binary_ctor, proto_atom.value);
						JS_FreeValue(context, binary_ctor);
						if (JS_IsException(proto)) {
							assert_conditional(false, "not a constructor", "uncompress");
						}
						auto obj = JS_NewObjectProtoClass(context, proto, Class::BinaryView::class_id);
						JS_FreeValue(context, proto);
						if (JS_IsException(obj)) {
							assert_conditional(false, "Cannot call constructor of BinaryView class", "uncompress");
						}
						JS_SetOpaque(obj, sub.release());
						JS_FreeValue(context, global_obj);
						JS_FreeValue(context, sen_obj);
						JS_FreeValue(context, kernel_obj);
						JS_FreeValue(context, binary_ctor);
						return obj;
					});
				}

			}

			namespace CompiledText
			{

				inline static auto decode_fs(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "decode_fs", [&](){
						assert_conditional(argc == 5, fmt::format("{} 5, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "decode_fs");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						auto key = JS::Converter::get_string(context, argv[2]);
						auto iv = JS::Converter::get_string(context, argv[3]);
						auto use_64_bit_variant = JS::Converter::get_bool(context, argv[4]);
						Sen::Kernel::Support::PopCap::CompiledText::Decode::process_fs(source, destination, key, iv, use_64_bit_variant);
						return JS_UNDEFINED;
					});
				}

				inline static auto encode_fs(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "encode_fs", [&](){
						assert_conditional(argc == 5, fmt::format("{} 5, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "encode_fs");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						auto key = JS::Converter::get_string(context, argv[2]);
						auto iv = JS::Converter::get_string(context, argv[3]);
						auto use_64_bit_variant = JS::Converter::get_bool(context, argv[4]);
						Sen::Kernel::Support::PopCap::CompiledText::Encode::process_fs(source, destination, key, iv, use_64_bit_variant);
						return JS_UNDEFINED;
					});
				}

			}

			namespace ResourceGroup
			{

				inline static auto split_fs(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "split_fs"_sv, [&](){
						assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "split_fs");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						Sen::Kernel::Support::PopCap::ResourceGroup::BasicConversion::split(source, destination);
						return JS_UNDEFINED; 
					});
				}

				inline static auto merge_fs(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "merge_fs"_sv, [&](){
						assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "merge_fs");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						Sen::Kernel::Support::PopCap::ResourceGroup::BasicConversion::merge(source, destination);
						return JS_UNDEFINED;
					});
				}

				inline static auto convert_fs(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					using PathStyle = Sen::Kernel::Support::PopCap::ResourceGroup::PathStyle;
					return proxy_wrapper(context, "convert_fs"_sv, [&](){
						assert_conditional(argc == 3, fmt::format("{} 3, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "convert_fs");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						auto path_style = JS::Converter::get_int32(context, argv[2]);
						switch (static_cast<PathStyle>(path_style)) {
							case PathStyle::ArrayStyle:{
								Sen::Kernel::Support::PopCap::ResourceGroup::Convert<false>::convert_fs(source, destination);
								break;
							}
							case PathStyle::WindowStyle:{
								Sen::Kernel::Support::PopCap::ResourceGroup::Convert<true>::convert_fs(source, destination);
								break;
							}
						}
						return JS_UNDEFINED;
					});
				}

			}

			namespace ResInfo
			{

				inline static auto split_fs(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "split_fs"_sv, [&](){
						assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "split_fs");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						Sen::Kernel::Support::PopCap::ResInfo::BasicConversion::split_fs(source, destination);
						return JS_UNDEFINED;
					});
				}

				inline static auto merge_fs(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "merge_fs"_sv, [&](){
						assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "merge_fs");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						Sen::Kernel::Support::PopCap::ResInfo::BasicConversion::merge_fs(source, destination);
						return JS_UNDEFINED;
					});
				}

				inline static auto convert_fs(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "convert_fs"_sv, [&](){
						assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "convert_fs");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						Sen::Kernel::Support::PopCap::ResInfo::Convert::convert_fs(source, destination);
						return JS_UNDEFINED;
					});
				}

			}

			namespace RenderEffects
			{

				inline static auto decode_fs(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "convert_fs"_sv, [&](){
						assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "decode_fs");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						Sen::Kernel::Support::PopCap::RenderEffects::Decode::process_fs(source, destination);
						return JS_UNDEFINED;
					});
				}

				inline static auto encode_fs(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "convert_fs"_sv, [&](){
						assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "encode_fs");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						Sen::Kernel::Support::PopCap::RenderEffects::Encode::process_fs(source, destination);
						return JS_UNDEFINED;
					});
				}

			}

			namespace CharacterFontWidget2
			{

				inline static auto decode_fs(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "decode_fs"_sv, [&]() {
						assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "decode_fs");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						Sen::Kernel::Support::PopCap::CharacterFontWidget2::Decode::process_fs(source, destination);
						return JS_UNDEFINED; 
					});
				}

				inline static auto encode_fs(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "encode_fs"_sv, [&]() {
						assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "encode_fs");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						Sen::Kernel::Support::PopCap::CharacterFontWidget2::Encode::process_fs(source, destination);
						return JS_UNDEFINED; 
					});
				}

			}

			namespace Particles
			{
				using Platform = Sen::Kernel::Support::PopCap::Particles::ParticlesPlatform;

				inline static auto constexpr get_platform(
					std::string_view platform
				) -> Platform
				{
					if (platform == "pc"_sv)
					{
						return Platform::PC_Compile;
					}
					if (platform == "game-console"_sv)
					{
						return Platform::GameConsole_Compile;
					}
					if (platform == "phone-32"_sv)
					{
						return Platform::Phone32_Compile;
					}
					if (platform == "phone-64"_sv)
					{
						return Platform::Phone64_Compile;
					}
					if (platform == "tv"_sv)
					{
						return Platform::TV_Compile;
					}
					if (platform == "wp"_sv)
					{
						return Platform::WP_XNB;
					}
				}

				inline static auto decode_fs(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "decode_fs", [&]() {
						assert_conditional(argc == 3, fmt::format("{} 3, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "decode_fs");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						auto platform = get_platform(JS::Converter::get_string(context, argv[2]));
						Sen::Kernel::Support::PopCap::Particles::Decode::process_fs(source, destination, platform);
						return JS_UNDEFINED; 
					});
				}

				inline static auto encode_fs(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "encode_fs", [&]() {
						assert_conditional(argc == 3, fmt::format("{} 3, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "encode_fs");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						auto platform = get_platform(JS::Converter::get_string(context, argv[2]));
						Sen::Kernel::Support::PopCap::Particles::Encode::process_fs(source, destination, platform);
						return JS_UNDEFINED; 
					});
				}

				inline static auto to_xml(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "to_xml", [&]() {
						assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "to_xml");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						Sen::Kernel::Support::PopCap::Particles::ToXML::process_fs(source, destination);
						return JS_UNDEFINED; 
					});
				}

				inline static auto from_xml(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "from_xml", [&]() {
						assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "from_xml");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						Sen::Kernel::Support::PopCap::Particles::FromXML::process_fs(source, destination);
						return JS_UNDEFINED; 
					});
				}

			}

			namespace PlayerInfo
			{

				inline static auto decode_fs(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "decode_fs", [&]() {
						assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "decode_fs");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						Sen::Kernel::Support::PopCap::PlayerInfo::Decode::process_fs(source, destination);
						return JS_UNDEFINED; 
					});
				}

				inline static auto encode_fs(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "encode_fs", [&]() {
						assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "encode_fs");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						Sen::Kernel::Support::PopCap::PlayerInfo::Encode::process_fs(source, destination);
						return JS_UNDEFINED; 
					});
				}

			}

			namespace CryptData
			{

				inline static auto decrypt_fs(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "decrypt_fs"_sv, [&]() {
						assert_conditional(argc == 3, fmt::format("{} 3, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "decrypt_fs");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						auto key = JS::Converter::get_string(context, argv[2]);
						Sen::Kernel::Support::PopCap::CryptData::Decrypt::process_fs(source, destination, key);
						return JS_UNDEFINED; 
					});
				}

				inline static auto encrypt_fs(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "encrypt_fs"_sv, [&]() {
						assert_conditional(argc == 3, fmt::format("{} 3, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "encrypt_fs");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						auto key = JS::Converter::get_string(context, argv[2]);
						Sen::Kernel::Support::PopCap::CryptData::Encrypt::process_fs(source, destination, key);
						return JS_UNDEFINED; 
					});
				}

			}

			namespace Newton
			{

				inline static auto decode_fs(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "decode_fs"_sv, [&]() {
						assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "decode_fs");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						Sen::Kernel::Support::PopCap::NewTypeObjectNotation::Decode::process_fs(source, destination);
						return JS_UNDEFINED; 
					});
				}

				inline static auto encode_fs(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "encode_fs"_sv, [&]() {
						assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "encode_fs");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						Sen::Kernel::Support::PopCap::NewTypeObjectNotation::Encode::process_fs(source, destination);
						return JS_UNDEFINED; 
					});
				}

			}

			namespace RTON
			{

				inline static auto decode_fs(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "decode_fs"_sv, [&]() {
						assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "decode_fs");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						Sen::Kernel::Support::PopCap::ReflectionObjectNotation::Decode::process_fs(source, destination);
						return JS_UNDEFINED; 
					});
				}

				inline static auto decrypt_fs(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "decrypt_fs"_sv, [&]() {
						assert_conditional(argc == 4, fmt::format("{} 4, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "decrypt_fs");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						auto key = JS::Converter::get_string(context, argv[2]);
						auto iv = JS::Converter::get_string(context, argv[3]);
						Sen::Kernel::Support::PopCap::ReflectionObjectNotation::Instance::decrypt_fs(source, destination, key, iv);
						return JS_UNDEFINED; 
					});
				}

				inline static auto decrypt_and_decode_fs(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "decrypt_and_decode_fs"_sv, [&]() {
						assert_conditional(argc == 4, fmt::format("{} 4, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "decrypt_and_decode_fs");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						auto key = JS::Converter::get_string(context, argv[2]);
						auto iv = JS::Converter::get_string(context, argv[3]);
						Sen::Kernel::Support::PopCap::ReflectionObjectNotation::Instance::decrypt_and_decode_fs(source, destination, key, iv);
						return JS_UNDEFINED; 
					});
				}

				inline static auto decode_fs_as_multiple_threads(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "decode_fs_as_multiple_threads"_sv, [&]() {
						auto paths = List<List<std::string>>{};
						for (const auto &i : Range<int>(argc))
						{
							const auto &data = JS::Converter::get_vector<std::string>(context, argv[i]);
							paths.emplace_back(data);
						}
						Sen::Kernel::Support::PopCap::ReflectionObjectNotation::Instance::decode_fs_as_multiple_threads(paths);
						return JS_UNDEFINED; 
					});
				}

				inline static auto encode_fs_as_multiple_threads(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "encode_fs_as_multiple_threads"_sv, [&]() {
						auto paths = List<List<std::string>>{};
						for (const auto &i : Range<int>(argc))
						{
							const auto &data = JS::Converter::get_vector<std::string>(context, argv[i]);
							paths.emplace_back(data);
						}
						Sen::Kernel::Support::PopCap::ReflectionObjectNotation::Instance::encode_fs_as_multiple_threads(paths);
						return JS_UNDEFINED; 
					});
				}

				inline static auto encode_fs(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "encode_fs"_sv, [&]() {
						assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "encode_fs");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						Sen::Kernel::Support::PopCap::ReflectionObjectNotation::Encode::process_fs(source, destination);
						return JS_UNDEFINED; 
					});
				}

				inline static auto encrypt_fs(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "encrypt_fs"_sv, [&]() {
						assert_conditional(argc == 4, fmt::format("{} 4, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "encrypt_fs");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						auto key = JS::Converter::get_string(context, argv[2]);
						auto iv = JS::Converter::get_string(context, argv[3]);
						Sen::Kernel::Support::PopCap::ReflectionObjectNotation::Instance::encrypt_fs(source, destination, key, iv);
						return JS_UNDEFINED; 
					});
				}

				inline static auto encode_and_encrypt_fs(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "encode_and_encrypt_fs"_sv, [&]() {
						assert_conditional(argc == 4, fmt::format("{} 4, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "encode_and_encrypt_fs");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						auto key = JS::Converter::get_string(context, argv[2]);
						auto iv = JS::Converter::get_string(context, argv[3]);
						Sen::Kernel::Support::PopCap::ReflectionObjectNotation::Instance::encode_and_encrypt_fs(source, destination, key, iv);
						return JS_UNDEFINED; });
				}
			}

			namespace RSBPatch
			{

				inline static auto encode_fs(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "encode_fs"_sv, [&]() {
						assert_conditional(argc == 3, fmt::format("{} 3, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "encode_fs");
						auto before_file = JS::Converter::get_string(context, argv[0]);
						auto after_file = JS::Converter::get_string(context, argv[1]);
						auto patch_file = JS::Converter::get_string(context, argv[2]);
						Kernel::Support::PopCap::ResourceStreamBundlePatch::Encode::process_fs(before_file, after_file, patch_file);
						return JS_UNDEFINED; 
					});
				}

				inline static auto decode_fs(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "decode_fs"_sv, [&](){
						assert_conditional(argc == 3, fmt::format("{} 3, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "decode_fs");
						auto before_file = JS::Converter::get_string(context, argv[0]);
						auto patch_file = JS::Converter::get_string(context, argv[1]);
						auto after_file = JS::Converter::get_string(context, argv[2]);
						Kernel::Support::PopCap::ResourceStreamBundlePatch::Decode::process_fs(before_file, patch_file, after_file);
						return JS_UNDEFINED; 
					});
				}

			}

			namespace RSB
			{

				inline static auto unpack_fs(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "unpack_fs"_sv, [&]() {
						assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "unpack_fs");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						Kernel::Support::PopCap::ResourceStreamBundle::Unpack::process_fs(source, destination);
						return JS_UNDEFINED; 
					});
				}

				inline static auto pack_resource(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "pack_resource"_sv, [&]() {
						assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "pack_resource");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						Kernel::Support::PopCap::ResourceStreamBundle::Miscellaneous::PackResource::process_fs(source, destination);
						return JS_UNDEFINED; 
					});
				}

				inline static auto unpack_resource(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "unpack_resource"_sv, [&]() {
					assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "unpack_resource");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						Kernel::Support::PopCap::ResourceStreamBundle::Miscellaneous::UnpackResource::process_fs(source, destination);
						return JS_UNDEFINED; 
					});
				}

				inline static auto unpack_cipher(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "unpack_cipher"_sv, [&]() {
					assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "unpack_cipher");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						Kernel::Support::PopCap::ResourceStreamBundle::Miscellaneous::UnpackCipher::process_fs(source, destination);
						return JS_UNDEFINED; 
					});
				}

				inline static auto pack_fs(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "pack_fs"_sv, [&]() {
						assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "pack_fs");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						Kernel::Support::PopCap::ResourceStreamBundle::Pack::process_fs(source, destination);
						return JS_UNDEFINED; 
					});
				}
			}

			namespace RSG
			{

				inline static auto unpack_fs(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "unpack_fs"_sv, [&]() {
						assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "unpack_fs");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						Kernel::Support::PopCap::ResourceStreamGroup::Unpack::process_fs(source, destination);
						return JS_UNDEFINED; 
					});
				}

				inline static auto pack_fs(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "pack_fs"_sv, [&]() {
						assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "pack_fs");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						Kernel::Support::PopCap::ResourceStreamGroup::Pack::process_fs(source, destination);
						return JS_UNDEFINED; 
					});
				}
			}

			namespace PAK
			{

				inline static auto unpack_fs(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "unpack_fs"_sv, [&](){
						assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "unpack_fs");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						Kernel::Support::PopCap::Package::Unpack::process_fs(source, destination);
						return JS_UNDEFINED; 
					});
				}

				inline static auto pack_fs(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "pack_fs"_sv, [&]() {
						assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "pack_fs");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						Kernel::Support::PopCap::Package::Pack::process_fs(source, destination);
						return JS_UNDEFINED; 
					});
				}
			}

			namespace Animation
			{

				inline static auto decode_fs(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "decode_fs"_sv, [&]() {
						assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "decode_fs");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						Sen::Kernel::Support::PopCap::Animation::Decode::process_fs(source, destination);
						return JS_UNDEFINED; 
					});
				}

				namespace ToFlash
				{

					inline static auto convert_fs(
						JSContext *context,
						JSValueConst this_val,
						int argc,
						JSValueConst *argv
					) -> JSElement::undefined
					{
						return proxy_wrapper(context, "convert_fs"_sv, [&]() {
							assert_conditional(argc == 4, fmt::format("{} 4, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "convert_fs");
							auto source = JS::Converter::get_string(context, argv[0]);
							auto destination = JS::Converter::get_string(context, argv[1]);
							auto resolution = static_cast<int>(JS::Converter::get_bigint64(context, argv[2]));
							if (JS::Converter::get_bool(context, argv[3])) {
								Sen::Kernel::Support::PopCap::Animation::Convert::ConvertToFlashWithLabel::process_fs(source, destination, resolution);
							}
							else {
								Sen::Kernel::Support::PopCap::Animation::Convert::ConvertToFlashWithMainSprite::process_fs(source, destination, resolution);
							}
							return JS_UNDEFINED; 
						});
					}

					inline static auto process(
						JSContext *context,
						JSValueConst this_val,
						int argc,
						JSValueConst *argv) -> JSElement::undefined
					{
						return proxy_wrapper(context, "process"_sv, [&]() {
							assert_conditional(argc == 4, fmt::format("{} 4, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "process");
							auto animation = JSON::object_to_json(context, argv[0]);
							auto destination = JS::Converter::get_string(context, argv[1]);
							auto resolution = static_cast<int>(JS::Converter::get_bigint64(context, argv[2]));
							auto extra = Sen::Kernel::Support::PopCap::Animation::Convert::ExtraInfo{.resolution = resolution};
							if (JS::Converter::get_bool(context, argv[3])) {
								Sen::Kernel::Support::PopCap::Animation::Convert::ConvertToFlashWithLabel::process_whole(animation, extra, destination);
							}
							else {
								Sen::Kernel::Support::PopCap::Animation::Convert::ConvertToFlashWithMainSprite::process_whole(animation, extra, destination);
							}
							Sen::Kernel::FileSystem::write_json(fmt::format("{}/data.json", destination), extra);
							return JS_UNDEFINED; 
						});
					}
				}

				namespace Miscellaneous
				{

					inline static auto resize_fs(
						JSContext *context,
						JSValueConst this_val,
						int argc,
						JSValueConst *argv
					) -> JSElement::undefined
					{
						return proxy_wrapper(context, "resize_fs"_sv, [&]() {
							assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "resize_fs");
							auto source = JS::Converter::get_string(context, argv[0]);
							auto resolution = static_cast<int>(JS::Converter::get_bigint64(context, argv[1]));
							Sen::Kernel::Support::PopCap::Animation::Convert::Resize::process_fs(source, resolution);
							return JS_UNDEFINED; 
						});
					}

					inline static auto dump_document(
						JSContext *context,
						JSValueConst this_val,
						int argc,
						JSValueConst *argv
					) -> JSElement::undefined
					{
						return proxy_wrapper(context, "dump_document"_sv, [&]() {
							assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "dump_document");
							auto source = JS::Converter::get_string(context, argv[0]);
							auto destination = argv[1];
							auto doc = Sen::Kernel::Support::PopCap::Animation::Miscellaneous::BasicDocument{};
							Sen::Kernel::Support::PopCap::Animation::Miscellaneous::Dump::process_fs(source, doc);
							auto atom_sprite = Atom(context, "sprite");  
							JS_DefinePropertyValue(context, destination, atom_sprite.value, JS::Converter::to_array(context, doc.sprite), int{JS_PROP_C_W_E});
							auto atom_image = Atom(context, "image");  
							JS_DefinePropertyValue(context, destination, atom_image.value, JS::Converter::to_array(context, doc.image), int{JS_PROP_C_W_E});
							auto atom_media = Atom(context, "media"); 
							JS_DefinePropertyValue(context, destination, atom_media.value, JS::Converter::to_array(context, doc.media), int{JS_PROP_C_W_E});
							auto atom_action = Atom(context, "action");  
							JS_DefinePropertyValue(context, destination, atom_action.value, JS::Converter::to_array(context, doc.action), int{JS_PROP_C_W_E});
							return JS_UNDEFINED; 
						});
					}

					inline static auto generate_image(
						JSContext *context,
						JSValueConst this_val,
						int argc,
						JSValueConst *argv) -> JSElement::undefined
					{
						return proxy_wrapper(context, "generate_image"_sv, [&]() {
							assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "generate_image");
							auto destination = JS::Converter::get_string(context, argv[0]);
							auto source = static_cast<Class::Image::Data*>(JS_GetOpaque2(context, argv[1], Class::Image::class_id));
							if (source == nullptr) {
								return JS_EXCEPTION;
							}
							Sen::Kernel::Support::PopCap::Animation::Miscellaneous::Generator::generate_image(destination, source);
							return JS_UNDEFINED; 
						});
					}

					inline static auto generate_document(
						JSContext *context,
						JSValueConst this_val,
						int argc,
						JSValueConst *argv
					) -> JSElement::undefined
					{
						return proxy_wrapper(context, "generate_document"_sv, [&]()  {
							assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "generate_document");
							auto destination = JS::Converter::get_string(context, argv[0]);
							auto source = Kernel::Support::PopCap::Animation::Miscellaneous::BasicDocument{};
							auto media_atom = Atom{context, "media"};
							auto sprite_atom = Atom{context, "sprite"};
							auto image_atom = Atom{context, "image"};
							auto media = JS_GetProperty(context, argv[1], media_atom.value);
							auto sprite = JS_GetProperty(context, argv[1], sprite_atom.value);
							auto image = JS_GetProperty(context, argv[1], image_atom.value);
							source.media = JS::Converter::get_vector<std::string>(context, media);
							source.sprite = JS::Converter::get_vector<std::string>(context, sprite);
							source.image = JS::Converter::get_vector<std::string>(context, image);
							JS_FreeValue(context, media);
							JS_FreeValue(context, sprite);
							JS_FreeValue(context, image);
							Sen::Kernel::Support::PopCap::Animation::Miscellaneous::Generator::generate_document(destination, &source);
							return JS_UNDEFINED; 
						});
					}

					inline static auto generate_sprite(
						JSContext *context,
						JSValueConst this_val,
						int argc,
						JSValueConst *argv
					) -> JSElement::undefined
					{
						return proxy_wrapper(context, "generate_sprite"_sv, [&]() {
							assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "generate_sprite");
							auto destination = JS::Converter::get_string(context, argv[0]);
							auto source = static_cast<Class::Sprite::Data*>(JS_GetOpaque2(context, argv[1], Class::Sprite::class_id));
							if (source == nullptr) {
								return JS_EXCEPTION;
							}
							Sen::Kernel::Support::PopCap::Animation::Miscellaneous::Generator::generate_sprite(destination, source);
							return JS_UNDEFINED; 
						});
					}

				}

				namespace FromFlash
				{

					inline static auto convert_fs(
						JSContext *context,
						JSValueConst this_val,
						int argc,
						JSValueConst *argv
					) -> JSElement::undefined
					{
						return proxy_wrapper(context, "convert_fs"_sv, [&]() {
							assert_conditional(argc == 3, fmt::format("{} 3, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "convert_fs");
							auto source = JS::Converter::get_string(context, argv[0]);
							auto destination = JS::Converter::get_string(context, argv[1]);
							if (JS::Converter::get_bool(context, argv[2])) {
								Sen::Kernel::Support::PopCap::Animation::Convert::ConvertFromFlashWithLabel::process_fs(source, destination);
							}
							else {
								Sen::Kernel::Support::PopCap::Animation::Convert::ConvertFromFlashWithMainSprite::process_fs(source, destination);
							}
							return JS_UNDEFINED; 
						});
					}
				}

				namespace Instance
				{

					inline static auto to_flash(
						JSContext *context,
						JSValueConst this_val,
						int argc,
						JSValueConst *argv
					) -> JSElement::undefined
					{
						return proxy_wrapper(context, "to_flash"_sv, [&]() {
							assert_conditional(argc == 4, fmt::format("{} 4, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "to_flash");
							auto source = JS::Converter::get_string(context, argv[0]);
							auto destination = JS::Converter::get_string(context, argv[1]);
							auto resolution = static_cast<int>(JS::Converter::get_bigint64(context, argv[2]));
							if (JS::Converter::get_bool(context, argv[3])) {
								Sen::Kernel::Support::PopCap::Animation::Convert::InstanceConvert::to_flash<true>(source, destination, resolution);
							}
							else {
								Sen::Kernel::Support::PopCap::Animation::Convert::InstanceConvert::to_flash<false>(source, destination, resolution);
							}
							return JS_UNDEFINED; 
						});
					}

					inline static auto from_flash(
						JSContext *context,
						JSValueConst this_val,
						int argc,
						JSValueConst *argv
					) -> JSElement::undefined
					{
						return proxy_wrapper(context, "from_flash"_sv, [&]() {
							assert_conditional(argc == 3, fmt::format("{} 3, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "from_flash");
							auto source = JS::Converter::get_string(context, argv[0]);
							auto destination = JS::Converter::get_string(context, argv[1]);
							if (JS::Converter::get_bool(context, argv[2])) {
								Sen::Kernel::Support::PopCap::Animation::Convert::InstanceConvert::from_flash<true>(source, destination);
							}
							else {
								Sen::Kernel::Support::PopCap::Animation::Convert::InstanceConvert::from_flash<false>(source, destination);
							}
							return JS_UNDEFINED; 
						});
					}
				}

				inline static auto encode_fs(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "encode_fs"_sv, [&]() {
						assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "encode_fs");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						Sen::Kernel::Support::PopCap::Animation::Encode::proces_fs(source, destination);
						return JS_UNDEFINED; 
					});
				}
			}

			namespace Custom
			{

				namespace Debug
				{
					inline static auto from_resource_custom(
						JSContext *context,
						JSValueConst this_val,
						int argc,
						JSValueConst *argv
					) -> JSElement::undefined
					{
						return proxy_wrapper(context, "from_resource_custom"_sv, [&]() {
							assert_conditional(argc == 3, fmt::format("{} 3, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "from_resource_custom");
							auto source = JS::Converter::get_string(context, argv[0]);
							auto destination = JS::Converter::get_string(context, argv[1]);
							Sen::Kernel::Support::Miscellaneous::Custom::StreamCompressedGroup::CustomResourceInformation json = *Sen::Kernel::FileSystem::read_json(source);
							auto result = nlohmann::ordered_json{};
							if (JS::Converter::get_bool(context, argv[2])) {
								Sen::Kernel::Support::Miscellaneous::Custom::StreamCompressedGroup::Common::exchange_custom_resource_info<true>(json, result);
							}
							else {
								Sen::Kernel::Support::Miscellaneous::Custom::StreamCompressedGroup::Common::exchange_custom_resource_info<false>(json, result);
							}
							Sen::Kernel::FileSystem::write_json(destination, result);
							return JS_UNDEFINED; 
						});
					}

					inline static auto to_resource_custom(
						JSContext *context,
						JSValueConst this_val,
						int argc,
						JSValueConst *argv
					) -> JSElement::undefined
					{
						return proxy_wrapper(context, "to_resource_custom"_sv, [&]() {
							assert_conditional(argc == 3, fmt::format("{} 3, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "to_resource_custom");
							auto source = JS::Converter::get_string(context, argv[0]);
							auto destination = JS::Converter::get_string(context, argv[1]);
							auto json = *Sen::Kernel::FileSystem::read_json(source);
							auto result = Sen::Kernel::Support::Miscellaneous::Custom::StreamCompressedGroup::CustomResourceInformation{};
							if (JS::Converter::get_bool(context, argv[2])) {
								Sen::Kernel::Support::Miscellaneous::Custom::StreamCompressedGroup::Common::exchange_custom_resource_info<true>(json, result);
							}
							else {
								Sen::Kernel::Support::Miscellaneous::Custom::StreamCompressedGroup::Common::exchange_custom_resource_info<false>(json, result);
							}
							Sen::Kernel::FileSystem::write_json(destination, result);
							return JS_UNDEFINED; 
						});
					}

				}

				namespace StreamCompressedGroup
				{
					inline static auto check_scg_composite(
						JSContext *context,
						JSValueConst this_val,
						int argc,
						JSValueConst *argv
					) -> JSElement::boolean
					{
						return proxy_wrapper(context, "check_scg_composite"_sv, [&]() {
							assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "check_scg_composite");
							auto source = JS::Converter::get_string(context, argv[0]);
							auto result = Sen::Kernel::Support::Miscellaneous::Custom::StreamCompressedGroup::Common::check_scg_composite(source);
							return JS::Converter::to_bool(context, result); 
						});
					}

					inline static auto decode_fs(
						JSContext *context,
						JSValueConst this_val,
						int argc,
						JSValueConst *argv
					) -> JSElement::undefined
					{
						return proxy_wrapper(context, "decode_fs"_sv, [&]() {
							assert_conditional(argc == 3, fmt::format("{} 3, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "decode_fs");
							auto source = JS::Converter::get_string(context, argv[0]);
							auto destination = JS::Converter::get_string(context, argv[1]);
							Sen::Kernel::Support::Miscellaneous::Custom::StreamCompressedGroup::Decode::process_fs(source, destination, JSON::object_to_json(context, argv[2]));
							return JS_UNDEFINED;
						});
					}

					inline static auto encode_fs(
						JSContext *context,
						JSValueConst this_val,
						int argc,
						JSValueConst *argv
					) -> JSElement::undefined
					{
						return proxy_wrapper(context, "encode_fs"_sv, [&]() {
							assert_conditional(argc == 3, fmt::format("{} 3, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "encode_fs");
							auto source = JS::Converter::get_string(context, argv[0]);
							auto destination = JS::Converter::get_string(context, argv[1]);
							Sen::Kernel::Support::Miscellaneous::Custom::StreamCompressedGroup::Encode::process_fs(source, destination, JSON::object_to_json(context, argv[2]));
							return JS_UNDEFINED;
						});
					}
				}

				namespace ResourceStreamBundle
				{

					inline static auto unpack_fs(
						JSContext *context,
						JSValueConst this_val,
						int argc,
						JSValueConst *argv
					) -> JSElement::undefined
					{
						return proxy_wrapper(context, "unpack_fs"_sv, [&]() {
							assert_conditional(argc == 3, fmt::format("{} 3, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "unpack_fs");
							auto source = JS::Converter::get_string(context, argv[0]);
							auto destination = JS::Converter::get_string(context, argv[1]);
							Sen::Kernel::Support::Miscellaneous::Custom::ResourceStreamBundle::Unpack::process_fs(source, destination, JSON::object_to_json(context, argv[2]));
							return JS_UNDEFINED; 
						});
					}

					inline static auto pack_fs(
						JSContext *context,
						JSValueConst this_val,
						int argc,
						JSValueConst *argv
					) -> JSElement::undefined
					{
						return proxy_wrapper(context, "pack_fs"_sv, [&]() {
							assert_conditional(argc == 3, fmt::format("{} 3, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "pack_fs");
							auto source = JS::Converter::get_string(context, argv[0]);
							auto destination = JS::Converter::get_string(context, argv[1]);
							Sen::Kernel::Support::Miscellaneous::Custom::ResourceStreamBundle::Pack::process_fs(source, destination, JSON::object_to_json(context, argv[2]));
							return JS_UNDEFINED; 
						});
					}
				}

			}

			namespace ReAnimation
			{

				using Platform = Sen::Kernel::Support::PopCap::ReAnimation::ReanimPlatform;


				inline static auto constexpr get_platform(
					std::string_view platform
				) -> Platform
				{
					if (platform == "pc"_sv)
					{
						return Platform::PC_Compile;
					}
					if (platform == "game-console"_sv)
					{
						return Platform::GameConsole_Compile;
					}
					if (platform == "phone-32"_sv)
					{
						return Platform::Phone32_Compile;
					}
					if (platform == "phone-64"_sv)
					{
						return Platform::Phone64_Compile;
					}
					if (platform == "tv"_sv)
					{
						return Platform::TV_Compile;
					}
					if (platform == "wp"_sv)
					{
						return Platform::WP_XNB;
					}
				}

				inline static auto decode_fs(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "decode_fs"_sv, [&]() {
						assert_conditional(argc == 3, fmt::format("{} 3, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "decode_fs");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						auto z_platform = JS::Converter::get_string(context, argv[2]);
						auto platform = get_platform(z_platform);
						Sen::Kernel::Support::PopCap::ReAnimation::Decode::process_fs(source, destination, platform);
						return JS_UNDEFINED; 
					});
				}

				inline static auto encode_fs(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "encode_fs"_sv, [&]() {
						assert_conditional(argc == 3, fmt::format("{} 3, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "encode_fs");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						auto z_platform = JS::Converter::get_string(context, argv[2]);
						auto platform = get_platform(z_platform);
						Sen::Kernel::Support::PopCap::ReAnimation::Encode::process_fs(source, destination, platform);
						return JS_UNDEFINED; 
					});
				}

				inline static auto to_xml(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "to_xml", [&]() {
						assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "to_xml");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						Sen::Kernel::Support::PopCap::ReAnimation::ToXML::process_fs(source, destination);
						return JS_UNDEFINED; 
					});
				}

				inline static auto from_xml(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "from_xml", [&]() {
						assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "from_xml");
							auto source = JS::Converter::get_string(context, argv[0]);
							auto destination = JS::Converter::get_string(context, argv[1]);
							Sen::Kernel::Support::PopCap::ReAnimation::FromXML::process_fs(source, destination);
							return JS_UNDEFINED; 
					});
				}

				namespace ToFlash
				{

					inline static auto convert_fs(
						JSContext *context,
						JSValueConst this_val,
						int argc,
						JSValueConst *argv
					) -> JSElement::undefined
					{
						return proxy_wrapper(context, "convert_fs"_sv, [&]() {
							assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "convert_fs");
							auto source = JS::Converter::get_string(context, argv[0]);
							auto destination = JS::Converter::get_string(context, argv[1]);
							Sen::Kernel::Support::PopCap::ReAnimation::Convert::ToFlash::process_fs(source, destination);
							return JS_UNDEFINED; 
						});
					}
				}

				namespace FromFlash
				{

					inline static auto convert_fs(
						JSContext *context,
						JSValueConst this_val,
						int argc,
						JSValueConst *argv
					) -> JSElement::undefined
					{
						return proxy_wrapper(context, "convert_fs"_sv, [&]() {
							assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "convert_fs");
							auto source = JS::Converter::get_string(context, argv[0]);
							auto destination = JS::Converter::get_string(context, argv[1]);
							Sen::Kernel::Support::PopCap::ReAnimation::Convert::FromFlash::process_fs(source, destination);
							return JS_UNDEFINED; 
						});
					}
				}

				namespace Instance
				{

					inline static auto to_flash(
						JSContext *context,
						JSValueConst this_val,
						int argc,
						JSValueConst *argv
					) -> JSElement::undefined
					{
						return proxy_wrapper(context, "to_flash"_sv, [&]() {
							assert_conditional(argc == 3, fmt::format("{} 3, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "to_flash");
							auto source = JS::Converter::get_string(context, argv[0]);
							auto destination = JS::Converter::get_string(context, argv[1]);
							auto z_platform = JS::Converter::get_string(context, argv[2]);
							auto platform = get_platform(z_platform);
							Sen::Kernel::Support::PopCap::ReAnimation::Convert::InstanceConvert::to_flash(source, destination, platform);
							return JS_UNDEFINED; 
						});
					}


					inline static auto from_flash(
						JSContext *context,
						JSValueConst this_val,
						int argc,
						JSValueConst *argv
					) -> JSElement::undefined
					{
						return proxy_wrapper(context, "from_flash"_sv, [&]() {
							assert_conditional(argc == 3, fmt::format("{} 3, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "from_flash");
							auto source = JS::Converter::get_string(context, argv[0]);
							auto destination = JS::Converter::get_string(context, argv[1]);
							auto z_platform = JS::Converter::get_string(context, argv[2]);
							auto platform = get_platform(z_platform);
							Sen::Kernel::Support::PopCap::ReAnimation::Convert::InstanceConvert::from_flash(source, destination, platform);
							return JS_UNDEFINED; 
						});
					}

				}
			}
		}

		namespace WWise
		{

			namespace SoundBank
			{

				namespace Miscellaneous
				{

					inline static auto add_music(
						JSContext *context,
						JSValueConst this_val,
						int argc,
						JSValueConst *argv
					) -> JSElement::undefined
					{
						return proxy_wrapper(context, "add_music"_sv, [&]() {
							assert_conditional(argc == 4, fmt::format("{} 4, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "add_music");
							auto source = JS::Converter::get_string(context, argv[0]);
							auto global_data_source = JS::Converter::get_string(context, argv[1]);
							auto media_source = JS::Converter::get_string(context, argv[2]);
							Kernel::Support::WWise::SoundBank::Miscellaneous::Support::add_music(source, global_data_source, media_source, JSON::object_to_json(context, argv[3]));
							return JS_UNDEFINED; 
						});
					}

					inline static auto create_soundbank(
						JSContext *context,
						JSValueConst this_val,
						int argc,
						JSValueConst *argv
					) -> JSElement::undefined
					{
						return proxy_wrapper(context, "create_soundbank"_sv, [&]() {
							assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "create_soundbank");
							auto source = JS::Converter::get_string(context, argv[0]);
							auto destination = JS::Converter::get_string(context, argv[1]);
							Kernel::Support::WWise::SoundBank::Miscellaneous::Support::create_soundbank(source, destination);
							return JS_UNDEFINED; 
						});
					}

				}

				inline static auto decode_fs(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "decode_fs"_sv, [&]() {
						assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "decode_fs");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						Kernel::Support::WWise::SoundBank::Decode::process_fs(source, destination);
						return JS_UNDEFINED; 
					});
				}

				inline static auto encode_fs(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					return proxy_wrapper(context, "encode_fs"_sv, [&]() {
						assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "encode_fs");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = JS::Converter::get_string(context, argv[1]);
						Kernel::Support::WWise::SoundBank::Encode::process_fs(source, destination);
						return JS_UNDEFINED; 
					});
				}

				inline static auto hash(
					JSContext *context,
					JSValueConst this_val,
					int argc,
					JSValueConst *argv
				) -> JSElement::undefined
				{
					using namespace Class::Number;
					using Uinteger32 = NumberID<std::uint32_t>;
					using Uinteger32C = Data<std::uint32_t>;
					return proxy_wrapper(context, "decode_fs"_sv, [&] {
						assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "hash");
						auto source = JS::Converter::get_string(context, argv[0]);
						auto destination = static_cast<Uinteger32C*>(JS_GetOpaque2(context, argv[1], Uinteger32::class_id));
						destination->value = Kernel::Support::WWise::SoundBank::Decode::fnv_hash(source);
						return JS_UNDEFINED; });
				}
			}
		}
	}

	namespace FileWatcher {

		using FileSystemWatcher = Kernel::FileSystem::FileSystemWatcher;

		struct JSCallback {
			JSValue jsCallback;                     
			std::function<void(JSValue)> function;  
		};

		class JSFileWatcher {
			public:
				JSFileWatcher(JSContext* ctx, const std::string& directory)
					: ctx(ctx), directory(directory), stopFlag(false), watcher(nullptr, Class::make_deleter<FileSystemWatcher>()) {}

				void start() {
					watcher.reset(new FileSystemWatcher(directory));
					watcher->start([this](const std::string& event, const std::string& filename) {
						this->onFileEvent(event, filename);
					});
				}

				void onFileEvent(const std::string& event, const std::string& filename) {
					auto lock = std::lock_guard<std::mutex>(mtx);
					auto it = eventCallbacks.find(event);
					if (it != eventCallbacks.end()) {
						auto jsFilename = JS_NewStringLen(ctx, filename.c_str(), filename.size());
						it->second.function(jsFilename);  
						JS_FreeValue(ctx, jsFilename);
					}
				}

				void on(const std::string& eventType, JSValue callback) {
					auto lock = std::lock_guard<std::mutex>(mtx);
					JS_DupValue(ctx, callback);
					auto cb = JSCallback{
						callback, 
						[this, callback](JSValue filename) {
							auto args = std::array<JSValue, 1>{ filename };
							auto value = JS_Call(ctx, callback, JS_UNDEFINED, args.size(), args.data());
							if (!JS_IsUndefined(value)) {
								JS_FreeValue(thiz.ctx, value);
							}
						}
					};
					eventCallbacks.insert(std::pair<std::string, JSCallback>(eventType, cb));
				}

				void stop() {
					auto lock = std::lock_guard<std::mutex>(mtx);
					stopFlag = true;
					for (auto& [event, callbackStruct] : eventCallbacks) {
						JS_FreeValue(ctx, callbackStruct.jsCallback); 
					}
					eventCallbacks.clear();
					watcher.reset();
				}

				~JSFileWatcher() {
					for (auto& [event, callbackStruct] : eventCallbacks) {
						JS_FreeValue(ctx, callbackStruct.jsCallback); 
					}
					stop();
				}

			private:
				JSContext* ctx;
				std::string directory;
				std::unique_ptr<FileSystemWatcher, decltype(Class::make_deleter<FileSystemWatcher>())> watcher;
				std::mutex mtx;
				std::unordered_map<std::string, JSCallback> eventCallbacks;
				bool stopFlag;
			};


		inline static JSClassID class_id;

		using Data = JSFileWatcher;

		inline static auto constructor(
			JSContext *ctx,
			JSValueConst new_target,
			int argc,
			JSValueConst *argv
		) -> JSElement::undefined
		{
			return proxy_wrapper(ctx, "constructor", [&](){
				auto s = std::unique_ptr<JSFileWatcher, decltype(Class::make_deleter<JSFileWatcher>())>(nullptr, Class::make_deleter<JSFileWatcher>());
				auto obj = JS_UNDEFINED;
				auto proto = JSElement::Prototype{};
				if (argc == 1) {
					auto source = JS::Converter::get_string(ctx, argv[0]);
					s.reset(new JSFileWatcher(ctx, source));
				}
				else {
					JS_ThrowInternalError(ctx, "FileWatcher cannot be initialized because argument does not satisfy constructor");
					return JS_EXCEPTION;
				}
				auto atom = Atom{ctx, "prototype"};
					proto = JS_GetProperty(ctx, new_target, atom.value);
				if (JS_IsException(proto)) {
					goto fail;
				}
				obj = JS_NewObjectProtoClass(ctx, proto, class_id);
				JS_FreeValue(ctx, proto);
				if (JS_IsException(obj)) {
					goto fail;
				}
				JS_SetOpaque(obj, s.release());
				return obj;
				fail:
					JS_FreeValue(ctx, obj);
					return JS_EXCEPTION; 
			});
		}

		inline static auto constexpr _class_name(

		) -> std::string_view
		{
			return "FileWatcher";
		}

		inline static auto this_class = Class::make_class_definition<Data>(
			_class_name(),
			class_id
		);

		inline static auto handle(
			JSContext *ctx,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv,
			std::string_view method_name,
			std::function<JSValue(Data*)> method
		) -> JSValue {
			return proxy_wrapper(ctx, method_name, [&](){
				auto s = static_cast<Data*>(JS_GetOpaque2(ctx, this_val, class_id));
				if (s == nullptr) {
					return JS_EXCEPTION;
				}
				return method(s);
			});
		}

		inline static auto on(
			JSContext* ctx,
			JSValueConst this_val,
			int argc,
			JSValueConst* argv
		) -> JSElement::undefined 
		{
			return handle(ctx, this_val, argc, argv, "on", [&](Data* s){
				assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "on");
				auto event = JS::Converter::get_string(ctx, argv[0]);
				auto callback = argv[1];
				s->on(event, callback);
				return JS_UNDEFINED;
			});
		}

		inline static auto start(
			JSContext* ctx,
			JSValueConst this_val,
			int argc,
			JSValueConst* argv
		) -> JSElement::undefined  
		{
			return handle(ctx, this_val, argc, argv, "start", [&](Data* s){
				s->start();
				return JS_UNDEFINED;
			});
		}

		inline static auto constexpr proto_functions = std::to_array<JSCFunctionListEntry>({
			JS_CPPFUNC_DEF("start", 0, start),
			JS_CPPFUNC_DEF("on", 2, on),
		});

		inline static auto register_class(
			JSContext *ctx
		) -> void
		{
			JS_NewClassID(JS_GetRuntime(ctx), &class_id);
			assert_conditional(JS_NewClass(JS_GetRuntime(ctx), class_id, &this_class) == 0, "FileWatcher class register failed", "register_class");
			auto class_name = _class_name();
			auto point_ctor = JS_NewCFunction2(ctx, constructor, class_name.data(), 2, JS_CFUNC_constructor, 0);
			auto proto = JS_NewObject(ctx);
			JS_SetPropertyFunctionList(ctx, proto, proto_functions.data(), proto_functions.size());
			JS_SetConstructor(ctx, point_ctor, proto);
			auto global_obj = JS_GetGlobalObject(ctx);
			auto obj1 = Class::make_instance_object(ctx, global_obj, "Sen");
			auto obj2 = Class::make_instance_object(ctx, obj1, "Kernel");
			auto atom = Atom(ctx, class_name);
			JS_DefinePropertyValue(ctx, obj2, atom.value, point_ctor, int{JS_PROP_C_W_E});
			JS_FreeValue(ctx, global_obj);
			JS_FreeValue(ctx, obj1);
			JS_FreeValue(ctx, obj2);
			JS_FreeValue(ctx, proto);
			return;
		}

	}

	namespace Clock {

		struct Data {
			public:
				explicit Data() : start_time_(), duration_(0), running_(false) {}

				inline auto start(

				) -> void
				{
					if (running_) {
						throw std::runtime_error("Clock has already been started");
					}
					start_time_ = std::chrono::steady_clock::now();
					running_ = true;
				}

				inline auto start_safe(

				) -> void
				{
					if (!running_) {
						start_time_ = std::chrono::steady_clock::now();
						running_ = true;
					}
				}

				inline auto stop(

				) -> void
				{
					if (!running_) {
						throw std::runtime_error("Clock has not been started");
					}
					auto end_time = std::chrono::steady_clock::now();
					duration_ += std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time_).count();
					running_ = false;
				}

				inline auto stop_safe (

				) -> void
				{
					if (running_) {
						auto end_time = std::chrono::steady_clock::now();
						duration_ += std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time_).count();
						running_ = false;
					}
				}

				inline auto reset(

				) -> void 
				{
					duration_ = 0;
					running_ = false;
				}

				inline auto get_duration(

				) -> int64_t const 
				{
					return duration_;
				}

				inline auto is_started(

				) -> bool const {
					return running_;
				}

				inline auto is_stopped(

				) -> bool const {
					return !running_;
				}

			private:
				std::chrono::time_point<std::chrono::steady_clock> start_time_;
				int64_t duration_;
				bool running_;
		};

		inline static JSClassID class_id;

		inline static auto constructor(
			JSContext *ctx, 
			JSValueConst new_target, 
			int argc, 
			JSValueConst *argv
		) -> JSValue
		{
			return proxy_wrapper(ctx, "constructor", [&](){
				auto s = std::make_unique<Data>();
				auto obj = JS_UNDEFINED;
				auto proto = JSElement::Prototype{};
				auto atom = Atom{ctx, "prototype"};
				proto = JS_GetProperty(ctx, new_target, atom.value);
				if (JS_IsException(proto)) {
					goto fail;
				}
				obj = JS_NewObjectProtoClass(ctx, proto, class_id);
				JS_FreeValue(ctx, proto);
				if (JS_IsException(obj)) {
					goto fail;
				}
				JS_SetOpaque(obj, s.release());
				return obj;
				fail:
					JS_FreeValue(ctx, obj);
					return JS_EXCEPTION; 
			});
		}

		inline static auto constexpr _class_name(

		) -> std::string_view
		{
			return "Clock";
		}

		inline static auto this_class = Class::make_class_definition<Data>(
			_class_name(),
			class_id
		);

		inline static auto handle(
			JSContext *ctx,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv,
			std::string_view method_name,
			std::function<JSValue(Data*)> method
		) -> JSValue {
			return proxy_wrapper(ctx, method_name, [&](){
				auto s = static_cast<Data*>(JS_GetOpaque2(ctx, this_val, class_id));
				if (s == nullptr) {
					return JS_EXCEPTION;
				}
				return method(s);
			});
		}

		inline static auto start(
			JSContext *ctx, 
			JSValueConst this_val, 
			int argc, 
			JSValueConst *argv
		) -> JSElement::undefined
		{
			return handle(ctx, this_val, argc, argv, "start", [&](Data *s) {
				try {
					s->start();
				} catch (const std::exception &e) {
					JS_ThrowInternalError(ctx, e.what());
					return JS_UNDEFINED;
				}
				return JS_UNDEFINED;
			});
		}

		inline static auto start_safe(
			JSContext *ctx, 
			JSValueConst this_val, 
			int argc, 
			JSValueConst *argv
		) -> JSElement::undefined 
		{
			return handle(ctx, this_val, argc, argv, "start_safe", [&](Data *s) {
				s->start_safe();
				return JS_UNDEFINED;
			});
		}

		inline static auto stop_safe(
			JSContext *ctx, 
			JSValueConst this_val, 
			int argc, 
			JSValueConst *argv
		) -> JSElement::undefined 
		{
			return handle(ctx, this_val, argc, argv, "stop_safe", [&](Data* clock){
				clock->stop_safe();
				return JS_UNDEFINED;
			});
		}

		inline static auto stop(
			JSContext *ctx, 
			JSValueConst this_val, 
			int argc, 
			JSValueConst *argv
		) -> JSElement::undefined 
		{
			return handle(ctx, this_val, argc, argv, "stop", [&](Data* clock){
				try {
					clock->stop();
				} catch (const std::exception &e) {
					JS_ThrowInternalError(ctx, e.what());
					return JS_UNDEFINED;
				}
				return JS_UNDEFINED;
			});
		}

		inline static auto reset(
			JSContext *ctx, 
			JSValueConst this_val, 
			int argc, 
			JSValueConst *argv
		) -> JSElement::undefined  
		{
			return handle(ctx, this_val, argc, argv, "reset", [&](Data* clock){
				clock->reset();
				return JS_UNDEFINED;
			});
		}

		inline static auto get_duration(
			JSContext *ctx, 
			JSValueConst this_val, 
			int argc, 
			JSValueConst *argv
		) -> JSElement::number 
		{
			return handle(ctx, this_val, argc, argv, "get_duration", [&](Data* clock){
				return JS_NewInt64(ctx, clock->get_duration());
			});
		}

		inline static auto get_duration_as_seconds(
			JSContext *ctx, 
			JSValueConst this_val, 
			int argc, 
			JSValueConst *argv
		) -> JSElement::number 
		{
			return handle(ctx, this_val, argc, argv, "get_duration_as_seconds", [&](Data* clock){
				return JS_NewFloat64(ctx, static_cast<double>(static_cast<double>(clock->get_duration()) / 1000.0));
			});
		}

		inline static auto is_started(
			JSContext *ctx, 
			JSValueConst this_val, 
			int argc, 
			JSValueConst *argv
		) -> JSElement::boolean  
		{
			return handle(ctx, this_val, argc, argv, "is_started", [&](Data* clock){
				return JS_NewBool(ctx, clock->is_started());
			});
		}

		inline static auto is_stopped(
			JSContext *ctx, 
			JSValueConst this_val, 
			int argc, 
			JSValueConst *argv
		) -> JSElement::boolean  
		{
			return handle(ctx, this_val, argc, argv, "is_stopped", [&](Data* clock){
				return JS_NewBool(ctx, clock->is_stopped());
			});
		}

		inline static auto constexpr proto_functions = std::to_array<JSCFunctionListEntry>({
			JS_CPPFUNC_DEF("start", 0, start),
			JS_CPPFUNC_DEF("stop", 0, stop),
			JS_CPPFUNC_DEF("start_safe", 0, start_safe),
			JS_CPPFUNC_DEF("stop_safe", 0, stop_safe),
			JS_CPPFUNC_DEF("reset", 0, reset),
			JS_CPPFUNC_DEF("duration", 0, get_duration),
			JS_CPPFUNC_DEF("duration_as_seconds", 0, get_duration_as_seconds),
			JS_CPPFUNC_DEF("isStarted", 0, is_started),
			JS_CPPFUNC_DEF("isStopped", 0, is_stopped),
		});

		inline static auto register_class(
			JSContext *ctx
		) -> void
		{
			JS_NewClassID(JS_GetRuntime(ctx), &class_id);
			assert_conditional(JS_NewClass(JS_GetRuntime(ctx), class_id, &this_class) == 0, "Clock class register failed", "register_class");
			auto class_name = _class_name();
			auto point_ctor = JS_NewCFunction2(ctx, constructor, class_name.data(), 2, JS_CFUNC_constructor, 0);
			auto proto = JS_NewObject(ctx);
			JS_SetPropertyFunctionList(ctx, proto, proto_functions.data(), proto_functions.size());
			JS_SetConstructor(ctx, point_ctor, proto);
			auto global_obj = JS_GetGlobalObject(ctx);
			auto obj1 = Class::make_instance_object(ctx, global_obj, "Sen");
			auto obj2 = Class::make_instance_object(ctx, obj1, "Kernel");
			auto atom = Atom(ctx, class_name);
			JS_DefinePropertyValue(ctx, obj2, atom.value, point_ctor, int{JS_PROP_C_W_E});
			JS_FreeValue(ctx, global_obj);
			JS_FreeValue(ctx, obj1);
			JS_FreeValue(ctx, obj2);
			JS_FreeValue(ctx, proto);
			return;
		}


	}


	namespace Miscellaneous
	{

		inline static auto deep_clone(
			JSContext *context,
			JSValueConst value
		) -> JSValue
		{
			switch (JS_VALUE_GET_TAG(value))
			{
				case JS_TAG_UNDEFINED:
				{
					return JS_UNDEFINED;
				}
				case JS_TAG_NULL:
				{
					return JS_NULL;
				}
				case JS_TAG_OBJECT:
				{
					if (JS_IsArray(context, value))
					{
						auto js_array = JS_NewArray(context);
						auto length = uint32_t{};
						auto atom = Atom{context, "length"};
						auto c_length = JS_GetPropertyUint32(context, value, atom.value);
						JS_ToUint32(context, &length, c_length);
						for (auto i : Range(length))
						{
							auto js_value = JS_GetPropertyUint32(context, value, i);
							JS_SetPropertyUint32(context, js_array, i, deep_clone(context, js_value));
							JS_FreeValue(context, js_value);
						}
						return js_array;
					}
					if (JS_IsObject(value))
					{
						auto json = JS_NewObject(context);
						auto *tab = static_cast<JSPropertyEnum *>(nullptr);
						auto tab_size = uint32_t{};
						if (JS_GetOwnPropertyNames(context, &tab, &tab_size, value, JS_GPN_STRING_MASK) == 0)
						{
							for (auto i : Range<uint32_t>(tab_size))
							{
								auto key = JS_AtomToCString(context, tab[i].atom);
								if (key == nullptr) {
									JS_FreeAtom(context, tab[i].atom);
									continue;
								}
								auto object_value = JS_GetProperty(context, value, tab[i].atom);
								JS_DefinePropertyValue(context, json, tab[i].atom, deep_clone(context, object_value), int{JS_PROP_C_W_E});
								JS_FreeAtom(context, tab[i].atom);
								JS_FreeValue(context, object_value);
								JS_FreeCString(context, key);
							}
							js_free(context, tab);
						}
						return json;
					}
					throw Exception("Unknown type", std::source_location::current(), "deep_clone");
				}
				case JS_TAG_STRING:
				{
					auto size = std::size_t{};
					auto c_str = JS_ToCStringLen(context, &size, value);
					auto destination = JS_NewStringLen(context, c_str, size);
					JS_FreeCString(context, c_str);
					return destination;
				}
				case JS_TAG_BOOL:
				{
					return JS_NewBool(context, JS_VALUE_GET_BOOL(value) != 0);
				}
				case JS_TAG_BIG_INT:
				{
					auto val = int64_t{};
					JS_ToBigInt64(context, &val, value);
					return JS_NewBigInt64(context, val);
				}
				case JS_TAG_FLOAT64:
				{
					return JS_NewFloat64(context, JS::Converter::get_float64(context, value));
				}
				case JS_TAG_INT:
				{
					return JS_NewFloat64(context, JS_VALUE_GET_INT(value));
				}
				default:
				{
					throw Exception("Unhandled JSValue", std::source_location::current(), "deep_clone");
				}
			}
		}

		inline static auto make_copy(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSValue
		{
			return proxy_wrapper(context, "make_copy", [&](){
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "make_copy");
				return deep_clone(context, argv[0]);
			});
		}

		inline static auto cast_ArrayBuffer_to_JS_String(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::string
		{
			return proxy_wrapper(context, "cast_ArrayBuffer_to_JS_String"_sv, [&]() {
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "cast_ArrayBuffer_to_JS_String");
				auto len = size_t{};
				auto buf = JS_GetArrayBuffer(context, &len, argv[0]);
				if (buf == nullptr) {
					throw Exception(fmt::format("{}", Kernel::Language::get("kernel.cast_ArrayBuffer_to_JS_String.failed_to_get_array_buffer")));
				}
				auto str = JS_NewStringLen(context, reinterpret_cast<const char*>(buf), len);
				return str; 
			});
		}

		inline static auto cast_movable_String_to_ArrayBuffer(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::ArrayBuffer
		{
			return proxy_wrapper(context, "cast_movable_String_to_ArrayBuffer"_sv, [&]() {
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "cast_movable_String_to_ArrayBuffer");
				auto buf = JS::Converter::get_string(context, argv[0]);
				return JS_NewArrayBufferCopy(context, reinterpret_cast<uint8_t*>(buf.data()), buf.size()); 
			});
		}

		inline static auto copyArrayBuffer(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::ArrayBuffer
		{
			return proxy_wrapper(context, "copyArrayBuffer"_sv, [&]() {
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "copyArrayBuffer");
				auto byte_len = std::size_t{};
				auto data = JS_GetArrayBuffer(context, &byte_len, argv[0]);
				assert_conditional(data != nullptr, "Cannot get ArrayBuffer from the first argument, it might not be an ArrayBuffer", "copyArrayBuffer");
				return JS_NewArrayBufferCopy(context, reinterpret_cast<uint8_t*>(data), byte_len); 
			});
		}

		inline static auto compareArrayBuffer(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::boolean
		{
			return proxy_wrapper(context, "compareArrayBuffer"_sv, [&]() {
				assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "compareArrayBuffer");
				auto size_1 = std::size_t{};
				auto data_1 = JS_GetArrayBuffer(context, &size_1, argv[0]);
				assert_conditional(data_1 != nullptr, "Cannot get ArrayBuffer from the first argument, it might not be an ArrayBuffer", "compareArrayBuffer");
				auto size_2 = std::size_t{};
				auto data_2 = JS_GetArrayBuffer(context, &size_2, argv[1]);
				assert_conditional(data_2 != nullptr, "Cannot get ArrayBuffer from the second argument, it might not be an ArrayBuffer", "compareArrayBuffer");
				if (size_1 != size_2) {
					return JS::Converter::to_bool(context, false);
				}
				auto is_same = true;
				for (auto i : Range<std::size_t>(size_1)) {
					if (data_1[i] != data_2[i]) {
						is_same = false;
						break;
					}
				}
				return JS::Converter::to_bool(context, is_same); 
			});
		}

		inline static auto cast_ArrayBuffer_to_JS_WideString(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::string
		{
			return proxy_wrapper(context, "cast_ArrayBuffer_to_JS_WideString"_sv, [&]() {
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "cast_ArrayBuffer_to_JS_WideString");
				auto len = size_t{};
				auto buf = JS_GetArrayBuffer(context, &len, argv[0]);
				if (buf == nullptr) {
					throw Exception(fmt::format("{}", Kernel::Language::get("kernel.cast_ArrayBuffer_to_JS_String.failed_to_get_array_buffer")));
				}
				auto utf16 = std::wstring(reinterpret_cast<wchar_t*>(buf), len / sizeof(wchar_t));
				auto converter = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>{};
				auto utf8 = converter.to_bytes(utf16);
				auto str = JS_NewStringLen(context, utf8.data(), utf8.size());
				return str; 
			});
		}

		inline static auto to_apng(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::undefined
		{
			return proxy_wrapper(context, "to_apng"_sv, [&]() {
				assert_conditional(argc == 3, fmt::format("{} 3, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "to_apng");
				auto a = JS::Converter::get_vector<std::string>(context, argv[0]);
				auto b = JS::Converter::get_string(context, argv[1]);
				auto s = static_cast<Class::APNGMakerSetting::Data*>(JS_GetOpaque2(context, argv[2], Class::APNGMakerSetting::class_id));
				if (s == nullptr) {
					return JS_EXCEPTION;
				}
				Kernel::Definition::APNGMaker::process_fs(a, b, s);
				return JS_UNDEFINED; 
			});
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
					if (key == "@attributes")
					{
						for (auto &[attribute_key, attribute_value] : value.items())
						{
							dynamic_cast<tinyxml2::XMLElement *>(node)->SetAttribute(attribute_key.data(), attribute_value.get<std::string>().data());
						}
					}
					else if (key == "@text")
					{
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
					}
					else
					{
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
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::Object
		{
			return proxy_wrapper(context, "deserialize"_sv, [&]() {
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "deserialize");
				auto doc = tinyxml2::XMLDocument{};
				auto source = JS::Converter::get_string(context, argv[0]);
				auto eResult = doc.Parse(source.data(), source.size());
				if (eResult != tinyxml2::XML_SUCCESS) {
					throw Exception(fmt::format("XML cannot be parsed, data: {}", source), std::source_location::current(), "deserialize");
				}
				auto j = nlohmann::ordered_json{};
				j[doc.RootElement()->Value()] = xml2json(doc.RootElement());
				return JSON::json_to_js(context, j); 
			});
		}

		inline static auto deserialize_fs(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::Object
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
				auto j = nlohmann::ordered_json{};
				j[doc.RootElement()->Value()] = xml2json(doc.RootElement());
				return JSON::json_to_js(context, j); 
			});
		}

		inline static auto serialize(
			JSContext *context,
			JSValueConst this_val,
			int argc,
			JSValueConst *argv
		) -> JSElement::string
		{
			return proxy_wrapper(context, "serialize"_sv, [&]() {
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "serialize");
				auto doc = tinyxml2::XMLDocument{};
				auto j = JSON::object_to_json(context, argv[0]);
				convert(j, doc);
				auto printer = tinyxml2::XMLPrinter{};
				doc.Print(&printer);
				return JS::Converter::to_string(context, std::string{printer.CStr(), static_cast<std::size_t>(printer.CStrSize() - 1)}); 
			});
		}

		inline static auto serialize_fs(
			JSElement::Context *context,
			JSElement::any this_value,
			JSElement::ParameterCount argc,
			JSElement::ParameterList argv
		) -> JSElement::undefined
		{
			return proxy_wrapper(context, "serialize_fs"_sv, [&]() {
				assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "serialize_fs");
				auto doc = XMLDocument{};
				auto source = JSON::object_to_json(context, argv[0]);
				convert(source, doc);
				auto printer = tinyxml2::XMLPrinter{};
				doc.Print(&printer);
				auto destination = JS::Converter::get_string(context, argv[1]);
				Kernel::FileSystem::write_file(destination, std::string{printer.CStr(), static_cast<std::size_t>(printer.CStrSize() - 1)});
				return JS_UNDEFINED; 
			});
		}

	}

}