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

    template <>
    inline auto from_value<std::shared_ptr<Kernel::Support::PopCap::Animation::Miscellaneous::Sprite>>(
        JSContext* context,
        JSValue val
    ) -> std::shared_ptr<Kernel::Support::PopCap::Animation::Miscellaneous::Sprite>
    {
        using Matrix = std::array<double, 6>;
	    using Color = std::array<double, 4>;
        auto destination = std::make_shared<Kernel::Support::PopCap::Animation::Miscellaneous::Sprite>();
		auto value = Value::as_new_reference(context, val);
		assert_conditional(value.is_object(), "Value must be object, but it isn't", "from_value");
		destination->name = JavaScript::from_value<std::string>(context, value.get_property("name").value);
		destination->link = JavaScript::from_value<std::string>(context, value.get_property("link").value);
		destination->transform = JavaScript::from_value<Matrix>(context, value.get_property("transform").value);
		destination->color = JavaScript::from_value<Color>(context, value.get_property("color").value);
        return destination;
    }

    template <>
    inline auto from_value<std::shared_ptr<Kernel::Support::PopCap::Animation::Miscellaneous::Image>>(
        JSContext* context,
        JSValue val
    ) -> std::shared_ptr<Kernel::Support::PopCap::Animation::Miscellaneous::Image>
    {
        using Matrix = std::array<double, 6>;
        auto destination = std::make_shared<Kernel::Support::PopCap::Animation::Miscellaneous::Image>();
		auto value = Value::as_new_reference(context, val);
		assert_conditional(value.is_object(), "Value must be object, but it isn't", "from_value");
		destination->name = JavaScript::from_value<std::string>(context, value.get_property("name").value);
		destination->id = JavaScript::from_value<std::string>(context, value.get_property("id").value);
		destination->transform = JavaScript::from_value<Matrix>(context, value.get_property("transform").value);
        return destination;
    }

}