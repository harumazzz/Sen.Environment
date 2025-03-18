#pragma once

#include "kernel/utility/algorithm/pointer.hpp"
#include "kernel/utility/algorithm/string_helper.hpp"
#include "kernel/utility/encoding/xml/node.hpp"

namespace Sen::Kernel::Encoding::XML {

    // struct Read {

    // protected:

    //     using Node = Node;

    //     template <typename T>
    //     using Pointer = T*;

    // public:

    //     inline static auto process_node (
    //         const pugi::xml_node & raw_node,
    //         Node & node
    //     ) -> void {
    //         if (raw_node.type() == pugi::node_element)
    //         {
    //             node.set_element();
    //             auto & current_node = node.get_element();
    //             current_node.set_name(String{raw_node.name()});
    //             {
    //                 auto attribute_count = 0_size;
    //                 for (auto raw_attribute : raw_node.children()) {
    //                     ++attribute_count;
    //                 }
    //                 current_node.allocate_attribute(attribute_count);
    //                 for (auto raw_attribute : raw_node.children()) {
    //                     current_node.add_attribute(StringHelper::make_string(raw_attribute.name()), StringHelper::make_string(raw_attribute.value()));
    //                 }
    //             }
    //             {
    //                 auto child_count = 0_size;
    //                 for (auto raw_child : raw_node.children()) {
    //                     ++child_count;
    //                 }
    //                 current_node.allocate_child(child_count);
    //                 for (auto raw_child: raw_node.children()) {
    //                     auto & current_child = current_node.append_child();
    //                     process_node(raw_child, current_child);
    //                 }
    //             }
    //         }
    //         else if (raw_node.type() == pugi::node_comment)
    //         {
    //             node.set_comment();
    //             auto & current_node = node.get_comment();
    //             current_node.value(StringHelper::make_string(raw_node.child_value()));
    //         }
    //         else if (raw_node.type() == pugi::node_cdata || raw_node.type() == pugi::node_pcdata)
    //         {
    //             node.set_text();
    //             auto & current_node = node.get_text();
    //             current_node.value(StringHelper::make_string(raw_node.child_value()));
    //             current_node.set_cdata(raw_node.child_value());
    //         }
    //         else
    //         {
    //             assert_conditional(false, "Invalid Node", "process_node");
    //         }
    //     }


    //     inline static auto process (
    //         const String & source,
    //         Node& destination
    //     ) -> void {
    //         auto raw_document = Subprojects::pugixml::xml_document{};
    //         raw_document.load_string(source.begin());
    //         auto current_child = raw_document.first_child();
    //         assert_conditional(current_child != nullptr, "An XML document must have at least one child", "process");
    //         if (current_child.type() == pugi::node_declaration) {
    //             current_child = current_child.next_sibling();
    //         }
    //         assert_conditional(current_child != nullptr, "An XML document must have one root element", "process");
    //         auto root_element = current_child.root();
    //         assert_conditional(root_element != nullptr, "An XML document must have one root element", "process");
    //         current_child = current_child.next_sibling();
    //         assert_conditional(current_child == nullptr, "An XML document must have only one root element in the tree", "process");
    //         process_node(root_element, destination);
    //     }

    // };

}