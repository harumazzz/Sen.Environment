#pragma once

#include "kernel/interface/shell.hpp"
#include "kernel/utility/javascript/builder.hpp"

namespace Sen::Kernel::Interface::API {

	
	#pragma region Clock

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
				return new Clock{};
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

	#pragma endregion
	
	#pragma region JsonWriter

	namespace JsonWriter {

		template <size_t InstanceCount>
		inline auto register_class(
			JSContext* context,
			const std::array<std::string_view, InstanceCount>& instance_names
		) -> void
		{
			using JsonWriter = Kernel::JsonWriter;
			auto builder = JavaScript::ClassBuilder<Kernel::JsonWriter>{ context, "JsonWriter" };
			builder.add_constructor<[]() {
				return new JsonWriter{};
			}>()
			.add_member_function<[](JsonWriter* writer){
			return writer->Clear();
			}, void> ("clear")
			.add_member_function<[](JsonWriter* writer) {
				return writer->ToString();
			}, std::string>("toString")
			.add_member_function<[](JsonWriter* writer){
				return writer->WriteStartArray();
			}, void>("writeStartArray")
			.add_member_function<[](JsonWriter* writer){
				return writer->WriteEndArray();
			}, void>("writeEndArray")
			.add_member_function<[](JsonWriter* writer){
				return writer->WriteStartObject();
			}, void>("writeStartObject")
			.add_member_function<[](JsonWriter* writer){
				return writer->WriteEndObject();
			}, void>("writeEndObject")
			.add_member_function<[](JsonWriter* writer, bool value){
				return writer->WriteBoolean(value);
			}, void, bool>("writeBoolean")
			.add_member_function<[](JsonWriter* writer){
				return writer->WriteNull();
			}, void>("writeNull")
			.add_member_function<[](JsonWriter* writer, std::string& property_name){
				return writer->WritePropertyName(property_name);
			}, void, std::string&>("writePropertyName")
			.add_member_function<[](JsonWriter* writer, std::string& value){
				return writer->WriteValue(value);
			}, void, std::string&>("writeString")
			.add_member_function<[](JsonWriter* writer, int64_t& value){
				return writer->WriteValue(value);
			}, void, int64_t&>("writeBigInt")
			.add_member_function<[](JsonWriter* writer, double& value){
				return writer->WriteValue(value);
			}, void, double&>("writeNumber")
			.add_getter_setter<bool, [](JsonWriter* writer){
				return writer->WriteIndent;
			}, [](JsonWriter* writer, bool& value){
				writer->WriteIndent = value;
			}>("writeIndent").build(instance_names);
			return;
		}

	}

	#pragma endregion

}