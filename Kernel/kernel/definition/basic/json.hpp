#pragma once

#include "kernel/definition/library.hpp"
#include "kernel/definition/macro.hpp"

namespace Sen::Kernel::Definition {

	namespace Object {
		/**
		 * data: Object
		 * return: keys
		*/

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

		/**
		 * data: Object
		 * return: array of value
		*/

		inline static auto values(
			const nlohmann::ordered_json &data
		) -> nlohmann::ordered_json::array_t
		{
			auto result = nlohmann::ordered_json::array_t{};
			for(auto & [key, value] : data.items()){
				result.push_back(value);
			}
			return result;
		}
	}

	namespace Map {

		// U is useless, but we need to keep it
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

		// U is useless, but we need to keep it
		template <typename T, typename U> 
		inline static auto keys(
			std::unordered_map<T, U> & map
		) -> List<T>
		{
			auto keys = List<T>{};
			for (auto & [key, value] : map) {
				keys.emplace_back(key);
			}
			return keys;
		}

	}
}