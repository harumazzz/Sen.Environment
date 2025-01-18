#pragma once

#include "kernel/interface/shell.hpp"
#include "kernel/utility/javascript/builder.hpp"

namespace Sen::Kernel::Interface::API {

	
	namespace Clock {

		template <size_t InstanceCount>
		inline auto register_class(
			JSContext* context,
			const std::array<std::string_view, InstanceCount>& instance_names
		) -> void
		{
			using Clock = Kernel::Clock;
			auto builder = JavaScript::ClassBuilder<Clock>{ context, "Clock" };
			builder.add_constructor<[]() {
				return new Clock();
			}>()
			.add_member_function<[](Clock* clock){
				return clock->start_safe();
			}, void>("start_safe")
			.add_member_function <[](Clock* clock) {
			return clock->stop_safe();
			}, void > ("stop_safe")
			.add_member_function <[](Clock* clock) {
			return clock->reset();
			}, void> ("reset")
			.add_member_function <[](Clock* clock) {
			return clock->get_duration();
			}, int64_t> ("get_duration")
			.add_member_function <[](Clock* clock) {
			return clock->is_started();
			}, bool > ("is_started")
			.add_member_function <[](Clock* clock) {
			return clock->is_stopped();
			}, bool > ("is_stopped")
			.build(instance_names);
			return;
		}

	}
	

}