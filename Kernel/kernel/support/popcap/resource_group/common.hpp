#pragma once

#include "kernel/utility/utility.hpp"

namespace Sen::Kernel::Support::PopCap::ResourceGroup {

	using namespace nlohmann;

	struct Common {

		inline static auto rewrite_slot_count(
			nlohmann::ordered_json& resource
		) -> void
		{
			auto slot_group = tsl::ordered_map<std::string, size_t>{};
			for (auto& e : resource["groups"]) {
				if (e.find("resources") == e.end()) {
					continue;
				}
				for (auto& c : e["resources"]) {
					if (slot_group.find(c["id"]) == slot_group.end()) {
						c["slot"] = slot_group.size();
						slot_group.insert({ c["id"], slot_group.size() });
					}
					else {
						c["slot"] = slot_group[c["id"]];
					}
				}
			}
			resource["slot_count"] = slot_group.size();
			return;
		}

		constexpr explicit Common(

		) noexcept = default;

		constexpr ~Common(

		) = default;

		inline static auto constexpr DefaultCoordinateOffset = 0;

		inline static auto constexpr DefaultLayoutOffset = 1;

		inline static auto const WindowStyle = std::string{"\\"};

		inline static auto const PosixStyle = std::string{"/"};

		inline static auto const Array = std::string{"array"};

		inline static auto const String = std::string{"string"};

		inline static auto const emptyString = std::string{""};

		inline static auto const emptyType = std::string{"0"};
	};

	enum class PathStyle : uint8_t
	{

		ArrayStyle,

		WindowStyle,

	};

}