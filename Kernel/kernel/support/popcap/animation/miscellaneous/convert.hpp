#pragma once

#include "kernel/support/popcap/animation/miscellaneous/common.hpp"

namespace Sen::Kernel::JavaScript {

	template <>
    inline auto from_value<std::shared_ptr<Kernel::Support::PopCap::Animation::Miscellaneous::BasicDocument>>(
        JSContext* context,
        JSValue val
    ) -> std::shared_ptr<Kernel::Support::PopCap::Animation::Miscellaneous::BasicDocument>
    {
        auto destination = std::make_shared<Kernel::Support::PopCap::Animation::Miscellaneous::BasicDocument>();
		auto value = Value::as_new_reference(context, val);
		assert_conditional(value.is_object(), "Value must be object, but it isn't", "from_value");
		destination->media = JavaScript::from_value<List<std::string>>(context, value.get_property("media").value);
		destination->sprite = JavaScript::from_value<List<std::string>>(context, value.get_property("sprite").value);
		destination->image = JavaScript::from_value<List<std::string>>(context, value.get_property("image").value);
		destination->action = JavaScript::from_value<List<std::string>>(context, value.get_property("action").value);
        return destination;
    }

}