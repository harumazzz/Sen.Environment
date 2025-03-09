#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include "subprojects/quickjs/quickjs.h"

namespace Sen::Kernel::Subprojects::quickjs {

	using ::JSClassDef;

	using ::JS_NewObjectClass;

	using ::JS_Throw;

	using ::JS_AtomToCString;

	using ::JSPropertyEnum;

	using ::JS_SetContextOpaque;

	using ::JS_GetContextOpaque;

	using ::JS_FreePropertyEnum;

	using ::JS_NewDate;

	using ::JS_GetOwnPropertyNames;

	using ::JS_Call;

	using ::JS_NewPromiseCapability;

	using ::JS_NewArrayBuffer;

	using ::JS_GetProperty;

	using ::JS_HasProperty;

	using ::JS_DeleteProperty;

	using ::JS_NewClassID;

	using ::JS_ToUint32;

	using ::JS_DefinePropertyGetSet;

	using ::JS_NewClass;

	using ::JS_SetConstructor;

	using ::JSRuntime;

	using ::JSValue;

	using ::JS_GetArrayBuffer;

	using ::JSContext;

	using ::JS_FreeRuntime;

	using ::JSCFunctionListEntry;

	using ::JS_SetPropertyFunctionList;

	using ::JS_FreeContext;

	using ::JS_FreeAtom;

	using ::JS_FreeValue;

	using ::JS_GetOpaque;

	using ::JS_GetOpaque2;

	using ::JS_NewRuntime;

	using ::JS_NewContext;

	using ::JS_NewAtomLen;

	using ::JS_IsPromise;

	using ::JS_Eval;

	using ::JS_FreeCString;

	using ::JS_ToString;

	using ::JS_ToCStringLen;

	using ::JS_GetRuntime;

	using ::JS_DupValue;

	using ::JS_IsUninitialized;

	using ::JS_IsUndefined;

	using ::JS_IsNull;

	using ::JS_IsArrayBuffer;

	using ::JS_IsBigInt;

	using ::JS_IsObject;

	using ::JS_IsObjectOfObject;

	using ::JS_IsString;

	using ::JS_IsFunction;

	using ::JS_IsException;

	using ::JS_IsNumber;

	using ::JS_IsBool;

	using ::JS_IsArray;

	using ::JS_ToFloat64;

	using ::JS_ToBigInt64;

	using ::JS_ToBigUint64;

	using ::JS_NewFloat64;

	using ::JS_NewBigInt64;

	using ::JS_NewBigUint64;

	using ::JS_NewStringLen;

	using ::JS_ToBool;

	using ::JS_NewBool;

	using ::JS_NewArray;

	using ::JS_NewArrayFrom;

	using ::JS_NewArrayBuffer;

	using ::JS_NewArrayBufferCopy;

	using ::JS_NewObject;

	using ::JS_DefinePropertyValue;

	using ::JS_NewAtomUInt32;

	using ::JS_SetProperty;

	using ::JSMemoryUsage;

	using ::JS_ComputeMemoryUsage;

	using ::JS_DumpMemoryUsage;

	using ::JS_IsJobPending;

	using ::JS_ExecutePendingJob;

	using ::JS_ThrowInternalError;

	using ::JSModuleDef;

	using ::JS_ThrowReferenceError;

	using ::JSClassID;

	using ::JS_GetImportMeta;

	using ::JS_GetGlobalObject;

	using ::JS_GetException;

	using ::JS_GetClassProto;

	using ::JS_SetClassProto;

	using ::JS_SetOpaque;

	using ::JS_NewCFunction2;

	using ::JSValueConst;

	using ::JS_NewObjectFrom;

	inline static auto constexpr $JS_EVAL_TYPE_MODULE = int{JS_EVAL_TYPE_MODULE};

	inline static auto constexpr $JS_EVAL_FLAG_COMPILE_ONLY = int{JS_EVAL_TYPE_MODULE};

	inline static auto constexpr $JS_EVAL_FLAG_STRICT = int{JS_EVAL_FLAG_STRICT};

	inline static auto constexpr $JS_PROP_C_W_E = int{JS_PROP_C_W_E};

	inline static auto constexpr $JS_EVAL_TYPE_GLOBAL = int{JS_EVAL_TYPE_GLOBAL};

	inline static auto constexpr $JS_PROP_THROW = int{JS_PROP_THROW};

	inline static auto constexpr $JS_INVALID_CLASS_ID = int{JS_INVALID_CLASS_ID};

	inline static const auto $JS_UNINITIALIZED = JS_UNINITIALIZED;

	inline static const auto $JS_UNDEFINED = JS_UNDEFINED;

	inline static const auto $JS_NULL = JS_NULL;

	inline static auto constexpr $JS_GPN_STRING_MASK = JS_GPN_STRING_MASK;

	inline static constexpr auto $JS_CFUNC_constructor = JS_CFUNC_constructor;

	inline static constexpr auto $JS_CFUNC_generic = JS_CFUNC_generic;

	inline static const auto $JS_EXCEPTION = JS_EXCEPTION;

	inline static auto constexpr $JS_TAG_INT = int64_t{JS_TAG_INT};

	inline static auto constexpr $JS_TAG_BOOL = int64_t{JS_TAG_BOOL};

	inline static auto constexpr $JS_TAG_NULL = int64_t{JS_TAG_NULL};

	inline static auto constexpr $JS_TAG_UNDEFINED = int64_t{JS_TAG_UNDEFINED};

	inline static auto constexpr $JS_TAG_UNINITIALIZED = int64_t{JS_TAG_UNINITIALIZED};

	inline static auto constexpr $JS_TAG_FLOAT64 = int64_t{JS_TAG_FLOAT64};

	inline static auto constexpr $JS_TAG_BIG_INT = int64_t{JS_TAG_BIG_INT};

	inline static auto constexpr $JS_TAG_STRING = int64_t{JS_TAG_STRING};

	inline static auto constexpr $JS_TAG_OBJECT = int64_t{JS_TAG_OBJECT};

	using ::JS_IsStrictEqual;

	inline auto constexpr $JS_VALUE_GET_TAG (
		const JSValue value
	) -> int {
		return JS_VALUE_GET_TAG(value);
	}

	inline auto constexpr $JS_CFUNC_DEF (
		char const* name,
		uint8_t length,
		decltype(JSCFunctionType::generic) func1
	) -> JSCFunctionListEntry {
		return JS_CFUNC_DEF(name, length, func1);
	}

	inline auto constexpr JS_VALUE_GET_POINTER (
		JSValue value
	) -> void* {
		return JS_VALUE_GET_PTR(value);
	}


}