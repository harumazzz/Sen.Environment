#pragma once

#include "kernel/utility/encoding/xml/common.hpp"
#include "kernel/utility/encoding/xml/text.hpp"
#include "kernel/utility/encoding/xml/comment.hpp"

namespace Sen::Kernel::Encoding::XML {

    // struct Node {

    // private:

    //     using Text = Text;

    //     using Comment = Comment;

    //     using Element = Element;

    //     using Type = Type;

    // protected:

    //     std::variant<Text, Element, Comment> m_value{};

    //     Type m_type{Type::Unknown};

    // public:

    //     Node(
    //     ) = default;

    //     Node (
    //         const Node& other
    //     ) = delete;

    //     Node (
    //         Node && other
    //     ) noexcept : m_type{other.m_type} {
    //         thiz.m_value.swap(other.m_value);
    //         other.m_type = Type::Unknown;
    //     }

    //     ~Node(
    //     ) = default;

    //     auto operator =(
    //         const Node& other
    //     ) -> Node& = delete;

    //     auto operator = (
    //         Node&& other
    //     ) noexcept -> Node& {
    //         thiz.m_value.swap(other.m_value);
    //         thiz.m_type = other.m_type;
    //         other.m_type = Type::Unknown;
    //         return thiz;
    //     }

    // protected:

    //     template <typename T> requires std::is_same_v<Text, T> or std::is_same_v<Comment, T> or std::is_same_v<Element, T>
    //     auto is (
    //     ) const -> bool {
    //         return std::holds_alternative<T>(thiz.m_value);
    //     }

    // public:

    //     auto is_comment (
    //     ) const -> bool {
    //         return thiz.is<Comment>();
    //     }

    //     auto is_element (
    //     ) const -> bool {
    //         return thiz.is<Element>();
    //     }

    //     auto is_text (
    //     ) const -> bool {
    //         return thiz.is<Text>();
    //     }

    // protected:

    //     template <typename T, typename... Args> requires std::is_same_v<Text, T> or std::is_same_v<Comment, T> or std::is_same_v<Element, T>
    //     auto set (
    //         Args&&... args
    //     ) -> void {
    //         thiz.m_value.emplace<T>(std::forward<Args>(args)...);
    //     }

    // public:

    //     template <typename... Args>
    //     auto set_element (
    //         Args... args
    //     ) -> void {
    //         thiz.m_type = Type::Element;
    //         return thiz.set<Element>(std::forward<Args>(args)...);
    //     }

    //     template <typename... Args>
    //     auto set_comment (
    //         Args... args
    //     ) -> void {
    //         thiz.m_type = Type::Comment;
    //         return thiz.set<Comment>(std::forward<Args>(args)...);
    //     }

    //     template <typename... Args>
    //     auto set_text (
    //         Args... args
    //     ) -> void {
    //         thiz.m_type = Type::Text;
    //         return thiz.set<Text>(std::forward<Args>(args)...);
    //     }

    // protected:

    //     template <typename T> requires std::is_same_v<Text, T> or std::is_same_v<Comment, T> or std::is_same_v<Element, T>
    //     auto get (
    //     ) -> T& {
    //         return std::get<T>(thiz.m_value);
    //     }

    // public:

    //     auto get_comment (
    //     ) -> Comment& {
    //         assert_conditional(thiz.is_comment(), "Expected an comment node, but the current node is not", "get_comment");
    //         return thiz.get<Comment>();
    //     }

    //     auto get_element (
    //     ) -> Element& {
    //         assert_conditional(thiz.is_comment(), "Expected an element node, but the current node is not", "get_element");
    //         return thiz.get<Element>();
    //     }

    //     auto get_text (
    //     ) -> Text& {
    //         assert_conditional(thiz.is_comment(), "Expected an text node, but the current node is not", "get_text");
    //         return thiz.get<Text>();
    //     }

    //     auto get_type (
    //     ) const -> Type {
    //         return thiz.m_type;
    //     }

    // };

}