#pragma once

#include "kernel/interface/cli.hpp"
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

		inline static List<Pointer<CStringList>> allocated{};

		inline static auto register_external (
			const Pointer<CStringView>& script,
			const Pointer<CStringList>& arguments
		) -> void
		{
			Executor::script = script;
			Executor::arguments = arguments;
			allocated.reserve(1000);
			return;
		}

		inline static auto clean_arguments (

		) -> void
		{
			for (auto & e : allocated) {
				if (e != nullptr) {
					if (e->value != nullptr) {
						for (auto i : Range(e->size)) {
							if (e->value[i].value != nullptr) {
								delete[] e->value[i].value;
								e->value[i].value = nullptr;
							}
						}
						delete[] e->value;
						e->value = nullptr;
						delete e;
						e = nullptr;
					}
				}
			}
			allocated.clear();
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