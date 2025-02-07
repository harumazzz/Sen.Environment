#pragma once

#include "kernel/interface/data.hpp"
#include "kernel/utility/library.hpp"
#include "kernel/utility/macro.hpp"
#include "kernel/support/utility.hpp"

namespace Sen::Kernel::Interface {

	template <typename T>
	using Pointer = T*;

	struct Shell {

		inline static Callback callback{nullptr};

	};

	struct Executor {

		inline static Pointer<CStringView> script{nullptr};
		
		inline static Pointer<CStringList> arguments{nullptr};

		inline static auto register_external (
			const Pointer<CStringView>& script,
			const Pointer<CStringList>& arguments
		) -> void
		{
			Executor::script = script;
			Executor::arguments = arguments;
			return;
		}

		inline static auto unregister_external (

		) -> void
		{
			script = nullptr;
			arguments = nullptr;
		}

	};
}