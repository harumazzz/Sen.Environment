#pragma once

#include "kernel/utility/library.hpp"
#include "kernel/utility/macro.hpp"

namespace Sen::Kernel {

	struct Object {

		inline static auto keys(
			const nlohmann::ordered_json &data
		) -> List<std::string>
		{
			auto result = List<std::string>();
			for(auto &[key, value] : data.items()){
				result.push_back(key);
			}
			return result;
		}
	};

	struct StandardMap {

		template <typename T, typename U> 
		inline static auto keys(
			std::map<T, U> & map
		) -> List<T>
		{
			auto keys = List<T>{};
			for (auto & [key, value] : map) {
				keys.emplace_back(key);
			}
			return keys;
		}

	};

	template <typename T, typename U>
	using Map = tsl::ordered_map<T, U>;
}