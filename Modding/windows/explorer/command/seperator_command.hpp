#pragma once

#include "explorer/command/base_command.hpp"

namespace Sen::Environment {

	class SeparatorCommand final : public BaseCommand {

		public:

		auto flags (
		) -> EXPCMDFLAGS override {
			return ECF_ISSEPARATOR;
		}

	};

}