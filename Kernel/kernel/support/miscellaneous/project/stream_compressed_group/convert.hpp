#pragma once

#include "kernel/support/miscellaneous/project/stream_compressed_group/definition.hpp"

namespace Sen::Kernel::JavaScript {

	template <>
    auto from_value<std::shared_ptr<Kernel::Support::Miscellaneous::Project::StreamCompressedGroup::Setting>>(
        JSContext* context,
        JSValue val
    ) -> std::shared_ptr<Kernel::Support::Miscellaneous::Project::StreamCompressedGroup::Setting>
    {
        auto destination = std::make_shared<Kernel::Support::Miscellaneous::Project::StreamCompressedGroup::Setting>();
		auto value = Value::as_new_reference(context, val);
		Kernel::Support::Miscellaneous::Project::StreamCompressedGroup::from_object(value, destination.operator*());
        return destination;
    }

}