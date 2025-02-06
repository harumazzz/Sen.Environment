#pragma once

#include "kernel/utility/algorithm/pointer.hpp"
#include "kernel/utility/algorithm/string_helper.hpp"
#include "kernel/utility/encoding/xml/node.hpp"

namespace Sen::Kernel::Encoding::XML {

    struct Read {

    protected:

        using Node = Node;

        template <typename T>
        using Pointer = T*;

    public:

        inline static auto process_node (
            const Pointer<Subprojects::tinyxml2::XMLNode const> & raw_node,
            Node & node
        ) -> void {
            if (auto raw_element = raw_node->ToElement()) {
                node.set_element();
                auto & current_node = node.get_element();
                current_node.set_name(String{raw_element->GetText()});
                {
                    auto attribute_count = 0_size;
                    for (auto raw_attribute = raw_element->FirstAttribute(); raw_attribute != nullptr; raw_attribute = raw_attribute->Next()) {
                        ++attribute_count;
                    }
                    current_node.allocate_attribute(attribute_count);
                    for (auto raw_attribute = raw_element->FirstAttribute(); raw_attribute; raw_attribute = raw_attribute->Next()) {
                        current_node.add_attribute(StringHelper::make_string(raw_attribute->Name()), StringHelper::make_string(raw_attribute->Value()));
                    }
                }
                {
                    auto child_count = 0_size;
                    for (auto raw_child = raw_element->FirstChild(); raw_child; raw_child = raw_child->NextSibling()) {
                        ++child_count;
                    }
                    current_node.allocate_child(child_count);
                    for (auto raw_child = raw_element->FirstChild(); raw_child != nullptr; raw_child = raw_child->NextSibling()) {
                        auto & current_child = current_node.append_child();
                        process_node(raw_child, current_child);
                    }
                }
            }
            else if (auto raw_text = raw_node->ToText()) {
                node.set_text();
                auto & current_node = node.get_text();
                current_node.value(StringHelper::make_string(raw_text->Value()));
                current_node.set_cdata(raw_text->CData());
            }
            else if (auto raw_comment = raw_node->ToComment()) {
                node.set_comment();
                auto & current_node = node.get_comment();
                current_node.value(StringHelper::make_string(raw_comment->Value()));
            }
            else {
                assert_conditional(false, "Invalid Node", "process_node");
            }
        }

        inline static auto process (
            const String & source,
            Node& destination
        ) -> void {
            auto raw_document = Subprojects::tinyxml2::XMLDocument{};
            raw_document.Parse(source.cbegin(), source.size());
            auto current_child = raw_document.FirstChild();
            assert_conditional(current_child != nullptr, "An XML document must have at least one child", "process");
            auto declaration = current_child->ToDeclaration();
            if (declaration != nullptr) {
                current_child = current_child->NextSibling();
            }
            assert_conditional(current_child != nullptr, "An XML document must have one root element", "process");
            auto root_element = current_child->ToElement();
            assert_conditional(root_element != nullptr, "An XML document must have one root element", "process");
            current_child = current_child->NextSibling();
            assert_conditional(current_child == nullptr, "An XML document must have only one root element in the tree", "process");
            process_node(root_element, destination);
        }

    };

}