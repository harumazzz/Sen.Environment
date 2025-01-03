#pragma once

#include "kernel/interface/cli.hpp"
#include "kernel/interface/data.hpp"
#include "kernel/definition/library.hpp"
#include "kernel/definition/macro.hpp"
#include "kernel/support/utility.hpp"

namespace Sen::Kernel::Interface {

	struct StringView {
		public:
			size_t size;
			const char* value;
	};

	struct StringList {
		public:
			StringView* value;
			size_t size;

			~StringList(

			) 
			{
				delete[] thiz.value;
			}
	};

	using CStringView = StringView;
	
	using CStringList = StringList;

	typedef void (*ShellCallback)(CStringList* list, StringView* destination);

	// Construct CStringView from standard String

	inline static auto construct_string(
		const std::string & that
	) -> CStringView
	{
		return CStringView {
			.size = that.size(),
			.value = that.data(),
		};
	}

	// Construct std::string from CStringView

	inline static auto make_standard_string(
		CStringView* that
	) -> std::string
	{
		return std::string{that->value, that->size};
	}

	inline static auto construct_standard_string(
		const CStringView & that
	) -> std::string
	{
		return std::string{that.value, that.size};
	}

	inline static auto construct_string_list(
		const List<std::string>& that
	) -> std::shared_ptr<CStringList>
	{
		auto destination = std::make_shared<CStringList>(new StringView[that.size()], that.size());
		for (auto i = 0; i < that.size(); ++i) {
			destination->value[i] = construct_string(that.at(i));
		}
		return destination;
	}


	inline static auto destruct_string_list(
		CStringList* that
	) -> List<std::string>
	{
		auto destination = List<std::string>{};
		for (auto i : Range(static_cast<std::size_t>(that->size))) {
			destination.emplace_back(std::string{ that->value[i].value, static_cast<std::size_t>(that->value[i].size) });
		}
		return destination;
	}


	struct Shell {
		private:

			inline static auto shell_cb(
				CStringList* list,
				CStringView* destination
			) -> void
			{
				return;
			}

		public:

			inline static Interface::ShellCallback callback = shell_cb;
	};

	struct MShellAPI {

		public:
			unsigned int version;

			bool is_gui;
	};

	struct Additional {

		inline static std::unique_ptr<std::string> script{nullptr};
		
		inline static std::unique_ptr<List<std::string>> arguments{nullptr};

		static auto assign(
			const std::string& script_value,  
			const List<std::string>& arguments_value  
		) -> void
		{
			Additional::script = std::make_unique<std::string>(script_value);
			Additional::arguments = std::make_unique<List<std::string>>(arguments_value);
		}

	};
}