#pragma once

#include "kernel/support/miscellaneous/project/resource_stream_bundle/definition.hpp"

namespace Sen::Kernel::JavaScript {

	template <>
    auto from_value<std::shared_ptr<Kernel::Support::Miscellaneous::Project::ResourceStreamBundle::Setting>>(
        JSContext* context,
        JSValue val
    ) -> std::shared_ptr<Kernel::Support::Miscellaneous::Project::ResourceStreamBundle::Setting>
    {
        auto destination = std::make_shared<Kernel::Support::Miscellaneous::Project::ResourceStreamBundle::Setting>();
		auto value = Value::as_new_reference(context, val);
		Kernel::Support::Miscellaneous::Project::ResourceStreamBundle::from_object(value, destination.operator*());
        return destination;
    }

}