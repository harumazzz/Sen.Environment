#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"

#include "quickjs.hpp"
#include "subprojects/quickjs/quickjs.h"

namespace Sen::Kernel::Subprojects::quickjs {

	inline static constexpr auto $JS_UNINITIALIZED = JS_UNINITIALIZED;

	using ::JSRuntime;

	using ::JSValue;

	using ::JSContext;

	using ::JS_FreeRuntime;

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

	inline auto JS_VALUE_GET_POINTER (
		JSValue value
	) -> void* {
		return JS_VALUE_GET_PTR(value);
	}

	using ::JS_ThrowReferenceError;

	using ::JS_GetImportMeta;

	inline static auto constexpr $JS_EVAL_TYPE_MODULE = int{JS_EVAL_TYPE_MODULE};

	inline static auto constexpr $JS_EVAL_FLAG_COMPILE_ONLY = int{JS_EVAL_TYPE_MODULE};

	inline static auto constexpr $JS_EVAL_FLAG_STRICT = int{JS_EVAL_FLAG_STRICT};

	inline static auto constexpr $JS_PROP_C_W_E = int{JS_PROP_C_W_E};


}