#pragma once

#include "kernel/interface/shell.hpp"
#include "kernel/utility/javascript/builder.hpp"

namespace Sen::Kernel::Interface::Script
{

	namespace Localization = Kernel::Language;

	namespace JS = Kernel::JavaScript;

	using Value = JS::Value;

	template <typename Callable> requires std::is_invocable<Callable>::value
	inline static auto proxy_wrapper(
		JSContext* context,
		std::string_view func_name,
		Callable&& callable
	) -> JSValue
	{
		auto result = JS_UNDEFINED;
		try {
			result = JS::Builder<Callable>::make_function_declaration(context, std::forward<Callable>(callable));
		}
		catch (...) {
			auto exception = parse_exception();
			if (exception.function_name.empty()) {
				exception.function_name = func_name;
			}
			result = JS::throw_exception(context, exception.message(), exception.source, exception.function_name);
		}
		return result;
	}

	namespace XML
	{

		using XMLDocument = tinyxml2::XMLDocument;

		inline static auto xml2json(
			const tinyxml2::XMLNode *node) -> nlohmann::ordered_json
		{
			auto j = nlohmann::ordered_json{};
			auto element = node->ToElement();
			if (element != nullptr)
			{
				for (auto attr = element->FirstAttribute(); attr; attr = attr->Next())
				{
					j["@attributes"][attr->Name()] = attr->Value();
				}
			}
			if (node->ToText())
			{
				j["@text"] = {
					{"value", node->Value()},
					{"is_cdata", node->ToText()->CData()},
				};
			}
			else
			{
				for (auto child = node->FirstChild(); child; child = child->NextSibling())
				{
					auto child_json = xml2json(child);
					if (j.contains(child->Value()))
					{
						if (j[child->Value()].is_array())
						{
							j[child->Value()].push_back(child_json);
						}
						else
						{
							j[child->Value()] = {j[child->Value()], child_json};
						}
					}
					else
					{
						if (child_json.find("@text") != child_json.end())
						{
							if (std::strcmp(child_json["@text"]["value"].get<std::string>().data(),
											child->Value()) == 0)
							{
								j = child_json;
							}
							else
							{
								j[child->Value()] = child_json;
							}
						}
						else
						{
							j[child->Value()] = child_json;
						}
					}
				}
			}
			return j;
		}


		inline static auto json2xml(
			const nlohmann::ordered_json &j,
			tinyxml2::XMLNode *node,
			tinyxml2::XMLDocument &doc
		) -> void
		{
			if (j.is_object())
			{
				for (auto &[key, value] : j.items())
				{
					switch (hash_string(key)) {
						case hash_string("@attributes"_sv): {
							for (auto &[attribute_key, attribute_value] : value.items())
							{
								dynamic_cast<tinyxml2::XMLElement *>(node)->SetAttribute(attribute_key.data(), attribute_value.get<std::string>().data());
							}
							break;
						}
						case hash_string("@text"_sv): {
							if (!value["is_cdata"].is_null() and value["is_cdata"])
							{
								auto cdata = doc.NewText(value["value"].get<std::string>().data());
								cdata->SetCData(true);
								node->InsertEndChild(cdata);
							}
							else
							{
								dynamic_cast<tinyxml2::XMLElement *>(node)->SetText(value["value"].get<std::string>().data());
							}
							break;
						}
						default: {
							if (value.is_object())
							{
								auto child = doc.NewElement(key.data());
								node->InsertEndChild(child);
								json2xml(value, child, doc);
							}
							else if (value.is_array())
							{
								for (auto &element : value)
								{
									auto child = doc.NewElement(key.data());
									node->InsertEndChild(child);
									json2xml(element, child, doc);
								}
							}
							else if (value.is_null()) {
								auto child = doc.NewElement(key.data());
								node->InsertEndChild(child);
							}
							else
							{
								auto child = doc.NewElement(node->Value());
								node->Parent()->InsertEndChild(child);
							}
							break;
						}

					}
				}
			}
			else if (j.is_array())
			{
				for (auto &element : j)
				{
					auto child = doc.NewElement(node->Value());
					node->Parent()->InsertEndChild(child);
					json2xml(element, child, doc);
				}
			}
			else if (j.is_string())
			{
				node->InsertEndChild(doc.NewText(j.get<std::string>().data()));
			}
			else
			{
				auto child = doc.NewElement(node->Value());
				node->Parent()->InsertEndChild(child);
			}
			return;
		}

		inline static auto convert(
			const nlohmann::ordered_json &j,
			tinyxml2::XMLDocument &doc
		) -> void
		{
			auto &root_name = j.begin().key();
			auto root = doc.NewElement(root_name.data());
			doc.InsertEndChild(root);
			json2xml(j.begin().value(), root, doc);
			return;
		}

		inline static auto deserialize(
			JSContext *context,
			JSValue value,
			int argc,
			JSValue* argv
		) -> JSValue
		{
			return proxy_wrapper(context, "deserialize"_sv, [&]() {
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "deserialize");
				auto doc = tinyxml2::XMLDocument{};
				auto source = JS::Converter::get_string(context, argv[0]);
				auto eResult = doc.Parse(source.data(), source.size());
				if (eResult != tinyxml2::XML_SUCCESS) {
					throw Exception(fmt::format("XML cannot be parsed, data: {}", source), std::source_location::current(), "deserialize");
				}
				auto value = nlohmann::ordered_json{};
				value[doc.RootElement()->Value()] = xml2json(doc.RootElement());
				auto destination = JS::to(context, value);
				return destination; 
			});
		}

		inline static auto deserialize_fs(
			JSContext *context,
			JSValue value,
			int argc,
			JSValue* argv
		) -> JSValue
		{
			return proxy_wrapper(context, "deserialize_fs"_sv, [&]() {
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "deserialize_fs");
				auto doc = tinyxml2::XMLDocument{};
				auto source = JS::Converter::get_string(context, argv[0]);
				auto view = Kernel::FileSystem::read_file(source);
				auto eResult = doc.Parse(view.data(), view.size());
				if (eResult != tinyxml2::XML_SUCCESS) {
					throw Exception(fmt::format("XML cannot be loaded, data: {}", source), std::source_location::current(), "deserialize");
				}
				auto value = nlohmann::ordered_json{};
				value[doc.RootElement()->Value()] = xml2json(doc.RootElement());
				auto destination = JS::to(context, value); 
				return destination;
			});
		}

		inline static auto serialize(
			JSContext *context,
			JSValue value,
			int argc,
			JSValue* argv
		) -> JSValue
		{
			return proxy_wrapper(context, "serialize"_sv, [&]() {
				assert_conditional(argc == 1, fmt::format("{} 1, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "serialize");
				auto doc = tinyxml2::XMLDocument{};
				auto value = Value::as_new_reference(context, argv[0]);
				auto j = JS::from(value);
				convert(j, doc);
				auto printer = tinyxml2::XMLPrinter{};
				doc.Print(&printer);
				return std::string{printer.CStr(), static_cast<std::size_t>(printer.CStrSize() - 1)}; 
			});
		}

		inline static auto serialize_fs(
			JSContext *context,
			JSValue valueue,
			int argc,
			JSValue* argv
		) -> JSValue
		{
			return proxy_wrapper(context, "serialize_fs"_sv, [&]() {
				assert_conditional(argc == 2, fmt::format("{} 2, {}: {}", Kernel::Language::get("kernel.argument_expected"), Kernel::Language::get("kernel.argument_received"), argc), "serialize_fs");
				auto doc = XMLDocument{};
				auto value = Value::as_new_reference(context, argv[0]);
				auto source = JS::from(value);
				convert(source, doc);
				auto printer = tinyxml2::XMLPrinter{};
				doc.Print(&printer);
				auto destination = JS::Converter::get_string(context, argv[1]);
				Kernel::FileSystem::write_file(destination, std::string{printer.CStr(), static_cast<std::size_t>(printer.CStrSize() - 1)});
			});
		}

	}

}