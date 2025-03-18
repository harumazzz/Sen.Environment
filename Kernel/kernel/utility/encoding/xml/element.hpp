#pragma once

#include "kernel/utility/encoding/xml/node.hpp"

namespace Sen::Kernel::Encoding::XML {

    // struct Element {

    // protected:

    //     String m_name{};

    //     HashMap<String, String> m_attribute{};

    //     CList<Node> m_child{};

    // public:

    //     auto has_attribute (
    //         const String& key
    //     ) -> bool {
    //         return thiz.m_attribute.find(key) != thiz.m_attribute.end();
    //     }

    //     auto add_child (
    //         Node&& node
    //     ) -> void {
    //         thiz.m_child.append(std::move(node));
    //     }

    //     auto remove_child (
    //         const usize& index
    //     ) -> void {
    //         thiz.m_child.pop(index);
    //     }

    //     auto add_attribute (
    //         String&& key,
    //         String&& value
    //     ) -> void {
    //         thiz.m_attribute.try_emplace(std::move(key), std::move(value));
    //     }

    //     auto get_attribute (
    //         const String& key
    //     ) -> String& {
    //         assert_conditional(thiz.has_attribute(key), fmt::format("Attribute {} not found in XML", key.view()), "get_attribute");
    //         return thiz.m_attribute.at(key);
    //     }

    //     auto try_get_attribute (
    //         const String& key
    //     ) -> std::optional<std::reference_wrapper<String>> {
    //         if (thiz.has_attribute(key)) {
    //             return thiz.m_attribute.at(key);
    //         }
    //         return std::nullopt;
    //     }

    //     auto find_child_by_name (
    //         const String& name
    //     ) -> std::optional<std::reference_wrapper<Node>>;

    //     constexpr auto child_count (
    //     ) const -> usize {
    //         return thiz.m_child.size();
    //     }

    //     auto allocate_child (
    //         const usize& size
    //     ) -> void {
    //         thiz.m_child.allocate(size);
    //     }

    //     auto allocate_attribute (
    //         const usize& size
    //     ) -> void {
    //         thiz.m_attribute.reserve(size);
    //     }

    //     auto attribute_count (
    //     ) const -> usize {
    //         return thiz.m_attribute.size();
    //     }

    //     auto has_children (
    //     ) const -> bool {
    //         return !thiz.m_child.empty();
    //     }

    //     auto has_attribute (
    //     ) const -> bool {
    //         return !thiz.m_attribute.empty();
    //     }

    //     auto name (
    //     ) -> String& {
    //         return thiz.m_name;
    //     }

    //     auto attribute (
    //     ) -> HashMap<String, String>& {
    //         return thiz.m_attribute;
    //     }

    //     auto child (
    //     ) -> CList<Node>& {
    //         return thiz.m_child;
    //     }

    //     auto get_child (
    //         const usize& index
    //     ) -> Node& {
    //         return thiz.m_child[index];
    //     }

    //     auto set_name (
    //         String&& value
    //     ) -> void {
    //         thiz.m_name = as_move(value);
    //     }

    //     auto set_name (
    //         const String& value
    //     ) -> void {
    //         thiz.m_name = value;
    //     }

    //     auto append_child (
    //     ) -> Node& {
    //         thiz.m_child.append();
    //         return thiz.m_child.last();
    //     }

    //     auto find_child_by_name(
    //         const String& name
    //     ) -> std::optional<std::reference_wrapper<Node>> {
    //         for (auto& m_node : thiz.m_child) {
    //             if (m_node.is_element() && m_node.get_element().name() == name) {
    //                 return m_node;
    //             }
    //         }
    //         return std::nullopt;
    //     }

    // };

}