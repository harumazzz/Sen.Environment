#pragma once

#include "kernel/utility/javascript/value.hpp"
#include "kernel/utility/javascript/runtime.hpp"

namespace Sen::Kernel::JavaScript::Converter {

    namespace Detail {

        inline auto throw_exception(
			JSContext* context, 
			const JSValue& value, 
			std::string_view type
		) -> void 
		{
			assert_conditional(false, fmt::format("Failed to convert JS value to {}", type), "throw_exception");
        }

    }

	inline static auto get_string (
		JSContext* context, 
		const JSValue& value
	) -> std::string {
		assert_conditional(static_cast<bool>(JS_IsString(value)), "Value must be string, but it isn't", "get_string");
        auto size = std::size_t{};
        auto c_str = JS_ToCStringLen(context, &size, value);
        auto str = std::string { c_str, size };
        JS_FreeCString(context, c_str);
        return str;
    }

    inline static auto get_int32(JSContext* context, const JSValue& value) -> int32_t {
		assert_conditional(static_cast<bool>(JS_IsNumber(value)), "Value must be number, but it isn't", "get_int32");
		auto result = int32_t{};
		JS_ToInt32(context, &result, value);
		return result;
	}

	inline static auto get_float64(JSContext* context, const JSValue& value) -> double {
		assert_conditional(static_cast<bool>(JS_IsNumber(value)), "Value must be number, but it isn't", "get_float64");
		auto result = double{};
		JS_ToFloat64(context, &result, value);
		return result;
	}

	inline static auto get_float32(JSContext* context, const JSValue& value) -> float {
		assert_conditional(static_cast<bool>(JS_IsNumber(value)), "Value must be number, but it isn't", "get_float64");
		auto result = double{};
		JS_ToFloat64(context, &result, value);
		return static_cast<float>(result);
	}

	inline static auto get_bigint64(JSContext* context, const JSValue& value) -> int64_t {
		assert_conditional(static_cast<bool>(JS_IsBigInt(context, value)), "Value must be bigint, but it isn't", "get_bigint64");
		auto result = int64_t{};
		JS_ToBigInt64(context, &result, value);
		return result;
	}

	inline static auto get_uint32(JSContext* context, const JSValue& value) -> uint32_t {
		assert_conditional(static_cast<bool>(JS_IsNumber(value)), "Value must be number, but it isn't", "get_uint32");
		auto result = uint32_t{};
		JS_ToUint32(context, &result, value);
		return result;
	}

	inline static auto get_uint64(JSContext* context, const JSValue& value) -> uint64_t {
		assert_conditional(static_cast<bool>(JS_IsBigInt(context, value)), "Value must be number, but it isn't", "get_uint64");
		auto result = uint64_t{};
		JS_ToBigUint64(context, &result, value);
		return result;
	}

    inline static auto get_bool(JSContext* context, const JSValue& value) -> bool {
		assert_conditional(static_cast<bool>(JS_IsBool(value)), "Value must be boolean, but it isn't", "get_bool");
        return static_cast<bool>(JS_ToBool(context, value));
    }

    inline static auto to_string(JSContext* context, std::string_view content) -> JSValue {
        return JS_NewStringLen(context, content.data(), content.size());
    }

    inline static auto to_bool(JSContext* context, bool value) -> JSValue {
        return JS_NewBool(context, value ? 1 : 0);
    }

    inline static auto to_number(JSContext* context, int32_t value) -> JSValue {
        return JS_NewInt32(context, value);
    }

    inline static auto to_number(JSContext* context, double value) -> JSValue {
        return JS_NewFloat64(context, value);
    }

    inline static auto to_number(JSContext* context, uint32_t value) -> JSValue {
        return JS_NewUint32(context, value);
    }

    inline static auto to_number(JSContext* context, int64_t value) -> JSValue {
        return JS_NewInt64(context, value);
    }

    inline static auto to_number(JSContext* context, uint64_t value) -> JSValue {
        return JS_NewBigUint64(context, value);
    }

	template <typename T>
    inline static auto to_bigint(JSContext* context, T value) -> JSValue {
        if constexpr (std::is_same<T, uint64_t>::value) {
            return JS_NewBigUint64(context, value);
        } else if constexpr (std::is_integral<T>::value) {
            return JS_NewBigInt64(context, static_cast<int64_t>(value));
        }
    }

	#if WINDOWS
    inline static auto get_undefined() -> JSValue { return JS_UNDEFINED; }
    inline static auto get_null() -> JSValue { return JS_NULL; }
    #else
    inline static constexpr auto get_undefined() -> JSValue { return JS_UNDEFINED; }
    inline static constexpr auto get_null() -> JSValue { return JS_NULL; }
    #endif

	template <typename T>
	inline static auto get_vector(JSContext* context, const JSValue& that) -> List<T> {
		auto atom = Atom{context, "length"};
		auto length_value = JS_GetProperty(context, that, atom.value);
		auto length = get_uint32(context, length_value);
		JS_FreeValue(context, length_value);
		auto m_list = List<T>{};
		m_list.reserve(static_cast<std::size_t>(length));
		for (auto i : Range<int>(length)) {
			auto atom = JS_NewAtomUInt32(context, i);
			auto value = JS_GetProperty(context, that, atom);
			if constexpr (std::is_same<T, int>::value) {
				m_list.emplace_back(get_int32(context, value));
			} else if constexpr (std::is_same<T, bool>::value) {
				m_list.emplace_back(get_bool(context, value));
			} else if constexpr (std::is_same<T, long long>::value) {
				m_list.emplace_back(get_bigint64(context, value));
			} else if constexpr (std::is_same<T, uint32_t>::value) {
				m_list.emplace_back(get_uint32(context, value));
			} else if constexpr (std::is_same<T, uint64_t>::value) {
				m_list.emplace_back(get_uint64(context, value));
			} else if constexpr (std::is_same<T, float>::value) {
				m_list.emplace_back(get_float32(context, value));
			} else if constexpr (std::is_same<T, double>::value) {
				m_list.emplace_back(get_float64(context, value));
			} else if constexpr (std::is_same<T, std::string>::value) {
				m_list.emplace_back(get_string(context, value));
			} else {
				m_list.emplace_back(value);
			}
			JS_FreeValue(context, value);
			JS_FreeAtom(context, atom);
		}
		return m_list;
	}

	template <typename T>
	inline static auto get_array(JSContext* context, const JSValue& that) -> List<T> {
		auto atom = Atom{context, "length"};
		auto length_value = JS_GetProperty(context, that, atom.value);
		auto length = get_uint32(context, length_value);
		JS_FreeValue(context, length_value);
		auto m_list = List<T>{};
		m_list.reserve(static_cast<std::size_t>(length));
		for (auto i : Range<int>(length)) {
			auto atom = JS_NewAtomUInt32(context, i);
			auto value = JS_GetProperty(context, that, atom);
			if constexpr (std::is_same<T, int>::value) {
				m_list.emplace_back(static_cast<T>(get_bigint64(context, value)));
			} else if constexpr (std::is_same<T, bool>::value) {
				m_list.emplace_back(get_bool(context, value));
			} else if constexpr (std::is_same<T, long long>::value) {
				m_list.emplace_back(static_cast<T>(get_bigint64(context, value)));
			} else if constexpr (std::is_same<T, uint32_t>::value) {
				m_list.emplace_back(static_cast<T>(get_bigint64(context, value)));
			} else if constexpr (std::is_same<T, uint64_t>::value) {
				m_list.emplace_back(static_cast<T>(get_bigint64(context, value)));
			} else if constexpr (std::is_same<T, float>::value) {
				m_list.emplace_back(static_cast<T>(get_float32(context, value)));
			} else if constexpr (std::is_same<T, double>::value) {
				m_list.emplace_back(static_cast<T>(get_float64(context, value)));
			} else {
				m_list.emplace_back(value);
			}

			JS_FreeValue(context, value);
			JS_FreeAtom(context, atom);
		}
		return m_list;
	}

	template <>
	auto get_vector<std::string>(JSContext* context, const JSValue& that) -> List<std::string> {
		auto atom = Atom{context, "length"};
		auto len_val = JS_GetProperty(context, that, atom.value);
		auto length = get_uint32(context, len_val);
		JS_FreeValue(context, len_val);
		auto m_list = List<std::string>{};
		m_list.reserve(static_cast<std::size_t>(length));
		for (auto i : Range(length)) {
			auto atom = JS_NewAtomUInt32(context, i);
			auto val = JS_GetProperty(context, that, atom);
			m_list.emplace_back(get_string(context, val));
			JS_FreeValue(context, val);
			JS_FreeAtom(context, atom);
		}
		return m_list;
	}

	template <typename T>
	auto to_array(JSContext* context, const List<T>& vec) -> JSValue {
		auto js_array = JS_NewArray(context);
		for (auto i : Range<std::size_t>(vec.size())) {
			auto atom = JS_NewAtomUInt32(context, i);
			if constexpr (std::is_same<T, int>::value) {
				JS_SetProperty(context, js_array, atom, JS_NewInt32(context, vec[i]));
			} else if constexpr (std::is_same<T, uint32_t>::value) {
				JS_SetProperty(context, js_array, atom, JS_NewBigInt64(context, static_cast<int64_t>(vec[i])));
			} else if constexpr (std::is_same<T, float>::value) {
				JS_SetProperty(context, js_array, atom, JS_NewFloat64(context, static_cast<double>(vec[i])));
			} else if constexpr (std::is_same<T, double>::value) {
				JS_SetProperty(context, js_array, atom, JS_NewFloat64(context, vec[i]));
			} else if constexpr (std::is_same<T, uint64_t>::value) {
				JS_SetProperty(context, js_array, atom, JS_NewBigUint64(context, vec[i]));
			} else if constexpr (std::is_same<T, bool>::value) {
				JS_SetProperty(context, js_array, atom, JS_NewBool(context, vec[i]));
			} else if constexpr (std::is_same<T, long long>::value) {
				JS_SetProperty(context, js_array, atom, JS_NewBigInt64(context, vec[i]));
			} else if constexpr (std::is_same<T, std::string>::value) {
				JS_SetProperty(context, js_array, atom, JS_NewStringLen(context, vec[i].data(), vec[i].size()));
			}
			JS_FreeAtom(context, atom);
		}
		return js_array;
	}


	inline static auto constexpr close_buffer = [](auto buffer)
	{ 
		std::free(buffer); 
		return; 
	};

	inline static auto read_file_as_js_arraybuffer(
		JSContext *context, 
		std::string_view source
	) -> JSValue
	{
		#if WINDOWS
		auto fp = std::unique_ptr<FILE, decltype(Language::close_file)>(_wfopen(String::utf8_to_utf16(source.data()).data(), L"rb"), Language::close_file);
		#else
		auto fp = std::unique_ptr<FILE, decltype(Language::close_file)>(std::fopen(source.data(), "rb"), Language::close_file);
		#endif
		if (fp == nullptr) {
			#if WINDOWS
			throw Exception(fmt::format("{}: {}", Language::get("cannot_read_file"), String::to_posix_style(std::string{source.data(), source.size()})), std::source_location::current(), "read_file_as_js_arraybuffer");
			#else
			throw Exception(fmt::format("{}: {}", Language::get("cannot_read_file"), source), std::source_location::current(), "read_file_as_js_arraybuffer");
			#endif
		}
		#if WINDOWS
		auto file_size = std::filesystem::file_size(std::filesystem::path{String::utf8_to_utf16(source.data()) });
		#else
		auto file_size = std::filesystem::file_size(std::filesystem::path{ source });
		#endif
		auto buffer = std::unique_ptr<char[], decltype(close_buffer)>(reinterpret_cast<char*>(std::malloc(file_size * sizeof(char))), close_buffer);
		if (buffer == nullptr) {
			#if WINDOWS
			throw Exception(fmt::format("C malloc allocating memory failed, source file: {}", String::to_posix_style({source.data(), source.size()})), std::source_location::current(), "read_file_as_js_arraybuffer");
			#else
			throw Exception(fmt::format("C malloc allocating memory failed, source file: {}", source), std::source_location::current(), "read_file_as_js_arraybuffer");
			#endif
		}
		auto result = std::fread(buffer.get(), 1, file_size, fp.get());
		if (result != file_size) {
			throw Exception(fmt::format("{}: {}", Language::get("cannot_read_file"), String::to_posix_style({source.data(), source.size()})), std::source_location::current(), "read_file_as_js_arraybuffer");
		}
		auto array_buffer = JS_NewArrayBufferCopy(context, reinterpret_cast<uint8_t*>(buffer.get()), file_size);
		return array_buffer;
	}

	inline static auto write_file_as_arraybuffer(
		JSContext* context,
		std::string_view destination,
		const JSValue& that
	) -> void
	{
		auto size = size_t{};
		auto data = JS_GetArrayBuffer(context, &size, that);
		if (data == nullptr) {
			throw Exception(fmt::format("{}", Language::get("js.converter.failed_to_get_js_array_buffer")), std::source_location::current(), "write_file_as_arraybuffer");
		}
		#if WINDOWS
			auto ofs = std::unique_ptr<FILE, decltype(Language::close_file)>(_wfopen(String::utf8_to_utf16(fmt::format("\\\\?\\{}", String::to_windows_style(destination.data()))).data(), L"wb"), Language::close_file);
		#else
			auto ofs = std::unique_ptr<FILE, decltype(Language::close_file)>(std::fopen(destination.data(), "wb"), Language::close_file);
		#endif
		if (ofs.get() == nullptr) {
			#if WINDOWS
			throw Exception(fmt::format("{}", Language::get("open_write_failed"), String::to_posix_style({destination.data(), destination.size()})), std::source_location::current(), "write_file_as_arraybuffer");
				#else
			throw Exception(fmt::format("{}", Language::get("open_write_failed"), destination),std::source_location::current(), "write_file_as_arraybuffer");
			#endif
		}
		std::fwrite(reinterpret_cast<char*>(data), 1, size, ofs.get());
		return;
		}

		inline static auto to_binary_list(
			JSContext* context,
			JSValue array_buffer
		) -> List<uint8_t>
		{
			auto size = std::size_t{};
			auto data = JS_GetArrayBuffer(context, &size, array_buffer);
			assert_conditional(data != nullptr, "Cannot get ArrayBuffer from value", "to_binary_list");
			return make_list(data, size);
		}


		inline static auto toArrayBuffer(
			JSContext* context,
			const List<uint8_t> & v
		) -> JSValue
		{
			return JS_NewArrayBufferCopy(context, v.data(), v.size());
		}

		template <typename T>
		inline static auto get_property(
			JSContext* context, 
			JSValueConst obj, 
			JSAtom atom, 
			const char* type_name, 
			T(*converter)(JSContext*, const JSValue&)
		) -> T 
		{
			auto value = JS_GetProperty(context, obj, atom);
			if (JS_IsException(value)) {
				Detail::throw_exception(context, obj, type_name);
			}
			auto result = converter(context, value);
			JS_FreeValue(context, value);
			return result;
		}

		inline static auto get_property_string(JSContext* context, JSValueConst obj, JSAtom atom) -> std::string {
			return get_property(context, obj, atom, "string", get_string);
		}

		inline static auto get_property_int32(JSContext* context, JSValueConst obj, JSAtom atom) -> int32_t {
			return get_property(context, obj, atom, "int32", get_int32);
		}

		inline static auto get_property_float64(JSContext* context, JSValueConst obj, JSAtom atom) -> double {
			return get_property(context, obj, atom, "double", get_float64);
		}

		inline static auto get_property_float32(JSContext* context, JSValueConst obj, JSAtom atom) -> float {
			return get_property(context, obj, atom, "float", get_float32);
		}

		inline static auto get_property_bigint64(JSContext* context, JSValueConst obj, JSAtom atom) -> int64_t {
			return get_property(context, obj, atom, "bigint64", get_bigint64);
		}

		inline static auto get_property_uint32(JSContext* context, JSValueConst obj, JSAtom atom) -> uint32_t {
			return get_property(context, obj, atom, "uint32", get_uint32);
		}

		inline static auto get_property_uint64(JSContext* context, JSValueConst obj, JSAtom atom) -> uint64_t {
			return get_property(context, obj, atom, "uint64", get_uint64);
		}

		inline static auto get_property_bool(JSContext* context, JSValueConst obj, JSAtom atom) -> bool {
			return get_property(context, obj, atom, "bool", get_bool);
		}
}