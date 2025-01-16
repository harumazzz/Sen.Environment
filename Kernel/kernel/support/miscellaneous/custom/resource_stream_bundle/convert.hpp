#pragma once

#include "kernel/support/miscellaneous/custom/resource_stream_bundle/definition.hpp"

namespace Sen::Kernel::JavaScript {

	using Setting = Kernel::Support::Miscellaneous::Custom::ResourceStreamBundle::Setting;

	template <>
    auto from_value<std::shared_ptr<Setting>>(
        JSContext* context,
        JSValue val
    ) -> std::shared_ptr<Setting>
    {
        auto destination = std::make_shared<Setting>();
		auto value = Value::as_new_reference(context, val);
		Kernel::Support::Miscellaneous::Custom::ResourceStreamBundle::from_object(value, destination.operator*());
        return destination;
    }

}