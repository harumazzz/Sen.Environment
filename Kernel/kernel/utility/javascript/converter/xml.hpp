#pragma once

#include "kernel/utility/javascript/common.hpp"
#include "kernel/utility/javascript/value.hpp"
#include "kernel/utility/javascript/runtime.hpp"

namespace Sen::Kernel::JavaScript::XML {

	struct Read {

		inline static auto process_attributes (
			JSContext* context,
			const tinyxml2::XMLElement* element
		) -> JSValue
		{
			auto destination = JS_NewObject(context);
			for (auto raw_element = element->FirstAttribute(); raw_element; raw_element = raw_element->Next()) {
				auto atom = JS_NewAtomLen(context, raw_element->Name(), std::strlen(raw_element->Name()));
				JS_DefinePropertyValue(context, destination, atom, JS_NewStringLen(context, raw_element->Value(), std::strlen(raw_element->Value())), int{JS_PROP_C_W_E});
				JS_FreeAtom(context, atom);
			}
			return destination;
		}

		inline static auto process_text(
			JSContext* context,
			const tinyxml2::XMLText* node
		) -> JSValue 
		{
			auto destination = JS_NewObject(context);
			auto atom = JS_NewAtomLen(context, "value", std::strlen("value"));
			auto cdata = JS_NewAtomLen(context, "cdata", std::strlen("cdata"));
			JS_DefinePropertyValue(context, destination, atom, JS_NewStringLen(context, node->Value(), std::strlen(node->Value())), int{JS_PROP_C_W_E});
			JS_DefinePropertyValue(context, destination, cdata, JS_NewBool(context, static_cast<int>(node->CData())), int{JS_PROP_C_W_E});
			JS_FreeAtom(context, atom);
			JS_FreeAtom(context, cdata);
			return destination;
		}

		inline static auto process_comment (
			JSContext* context,
			const tinyxml2::XMLComment* node
		) -> JSValue 
		{
			auto destination = JS_NewObject(context);
			auto atom = JS_NewAtomLen(context, "value", std::strlen("value"));
			JS_DefinePropertyValue(context, destination, atom, JS_NewStringLen(context, node->Value(), std::strlen(node->Value())), int{JS_PROP_C_W_E});
			JS_FreeAtom(context, atom);
			return destination;
		}

		inline static auto process_child_node (
			JSContext* context, 
			const tinyxml2::XMLNode* node	
		) -> JSValue 
		{
			auto children_object = JS_NewObject(context);
			for (auto child = node->FirstChild(); child; child = child->NextSibling()) {
				auto child_value = xml2js(context, child);
				auto child_name = child->Value();
				auto atom = JS_NewAtomLen(context, child_name, std::strlen(child_name));

				auto existing_value = JS_GetProperty(context, children_object, atom);

				if (JS_IsArray(context, existing_value)) {
					auto atom_length = JS_NewAtomLen(context, "length", std::strlen("length"));
					auto length_value = JS_GetProperty(context, existing_value, atom_length);
					auto length = uint32_t{};
					JS_ToUint32(context, &length, length_value);
					JS_FreeAtom(context, atom_length);
					JS_FreeValue(context, length_value);
					auto index_atom = JS_NewAtomUInt32(context, length);
					JS_SetProperty(context, existing_value, index_atom, JS_DupValue(context, child_value));
					JS_FreeAtom(context, index_atom);
				} 
				else if (JS_IsObject(existing_value)) {
					auto array = JS_NewArray(context);
					{
						auto index_atom = JS_NewAtomUInt32(context, 0);
						JS_SetProperty(context, array, index_atom, JS_DupValue(context, existing_value));
						JS_FreeAtom(context, index_atom);
					}
					{
						auto index_atom = JS_NewAtomUInt32(context, 1);
						JS_SetProperty(context, array, index_atom, JS_DupValue(context, child_value));
						JS_FreeAtom(context, index_atom);
					}
					JS_SetProperty(context, children_object, atom, array);
				} 
				else {
					JS_SetProperty(context, children_object, atom, child_value);
				}
				JS_FreeAtom(context, atom);
				JS_FreeValue(context, existing_value);
			}

			return children_object;
		}


		inline static auto xml2js(
			JSContext* context, 
			const tinyxml2::XMLNode* node
		) -> JSValue 
		{
			auto object = JS_NewObject(context);
			{
				auto text_atom = JS_NewAtomLen(context, "@text", std::strlen("@text"));
				JS_SetProperty(context, object, text_atom, JS_NewObject(context));
				JS_FreeAtom(context, text_atom);

				auto comment_atom = JS_NewAtomLen(context, "@comment", std::strlen("@comment"));
				JS_SetProperty(context, object, comment_atom, JS_NewObject(context));
				JS_FreeAtom(context, comment_atom);
			}
			if (auto element = node->ToElement()) {
				{
					auto attributes = process_attributes(context, element);
					auto atom = JS_NewAtomLen(context, "@attributes", std::strlen("@attributes"));
					JS_SetProperty(context, object, atom, attributes);
					JS_FreeAtom(context, atom);
				}
				{
					auto atom = JS_NewAtomLen(context, "@children", std::strlen("@children"));
					auto children = process_child_node(context, element);
					JS_SetProperty(context, object, atom, children);
					JS_FreeAtom(context, atom);
				}
			}
			else if (auto raw_text = node->ToText()) {
				auto atom = JS_NewAtomLen(context, "@text", std::strlen("@text"));
				auto text_value = process_text(context, raw_text);
				if (JS_HasProperty(context, object, atom)) {
					JS_DeleteProperty(context, object, atom, int{JS_PROP_THROW});
				}
				JS_SetProperty(context, object, atom, text_value);
				JS_FreeAtom(context, atom);
			} 
			else if (auto raw_comment = node->ToComment()) {
				auto atom = JS_NewAtomLen(context, "@comment", std::strlen("@comment"));
				auto comment_value = process_comment(context, raw_comment);
				if (JS_HasProperty(context, object, atom)) {
					JS_DeleteProperty(context, object, atom, int{JS_PROP_THROW});
				}
				JS_SetProperty(context, object, atom, comment_value);
				JS_FreeAtom(context, atom);
			}
			else {
				assert_conditional(false, "undefined convert", "xml2js");
			}
			return object;
		}


	};

	struct Write {

		inline static auto process (
			tinyxml2::XMLNode* node,
			Value& value
		) -> void
		{
			return;
		}

	};
	

}