#pragma once

#include "kernel/utility/encoding/xml/node.hpp"

namespace Sen::Kernel::Encoding::XML {

    struct Write {

        protected:

            using Node = Node;

            template <typename T>
            using Pointer = T*;

        public:
            /*
            inline static auto process_node (
                Subprojects::tinyxml2::XMLDocument & raw_document,
                Node & node
            ) -> Pointer<Subprojects::tinyxml2::XMLNode> {
                auto raw_node = std::add_pointer_t<Subprojects::tinyxml2::XMLNode>{nullptr};
                switch (node.get_type()) {
                    case Type::Element: {
                        auto & current_node = node.get_element();
                        raw_node = raw_document.NewElement(current_node.name().data());
                        auto raw_element = raw_node->ToElement();
                        for (auto & [key, value] : current_node.attribute()) {
                            raw_element->SetAttribute(key.begin(), value.data());
                        }
                        for (auto & child : current_node.child()) {
                            raw_element->InsertEndChild(process_node(raw_document, child));
                        }
                        break;
                    }
                    case Type::Text: {
                        auto & current_node = node.get_text();
                        raw_node = raw_document.NewText(current_node.value().begin());
                        auto raw_text = raw_node->ToText();
                        raw_text->SetCData(current_node.is_cdata());
                        break;
                    }
                    case Type::Comment: {
                        auto & current_node = node.get_comment();
                        raw_node = raw_document.NewComment(current_node.value().begin());
                        break;
                    }
                    default: {
                        assert_conditional(false, "Invalid node", "process_node");
                    }
                }
                return raw_node;
            }
            */

            inline static auto process (
                Node & source
            ) -> String {
                /*
                auto raw_document = Subprojects::tinyxml2::XMLDocument{};
                raw_document.InsertEndChild(process_node(raw_document, source));
                auto printer = Subprojects::tinyxml2::XMLPrinter{};
                raw_document.Print(&printer);
                auto destination = String{printer.CStr(), static_cast<usize>(printer.CStrSize() - 1)};
                */
                auto destination = String{};
                return destination;
            }
    };

}