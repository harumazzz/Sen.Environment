#pragma once

#include "kernel/utility/javascript/common.hpp"

namespace Sen::Kernel::Javascript {

    struct JSString : BasicString {

        protected:

            using Base = BasicString;

            Pointer<Subprojects::quickjs::JSContext> m_context;

            Subprojects::quickjs::JSValue m_value;

        public:

            explicit JSString(

            ) : Base{}, m_context{nullptr}, m_value{Subprojects::quickjs::$JS_UNINITIALIZED}
            {
            }

            explicit JSString(
                const Size& size
            ) = delete;

            explicit JSString(
                const char* data
            ) = delete;

            explicit JSString(
                char* data,
                const Size& size
            ) = delete;

            explicit JSString(
                const char* data,
                const Size& size
            ) = delete;

            JSString(
                const JSString& other
            ) = delete;

            auto allocate(
                Size const& size
            ) -> void  override = delete;

            auto operator =(
                const JSString& other
            ) -> JSString & = delete;

            constexpr auto operator=(
                JSString&& other
            ) noexcept -> JSString& {
                if (this != &other) {
                    thiz._size = other._size;
                    thiz.value = other.value;
                    thiz.m_value = other.m_value;
                    thiz.m_context = other.m_context;
                    other.value = nullptr;
                    other._size = 0;
                    other.m_value = Subprojects::quickjs::$JS_UNINITIALIZED;
                    other.m_context = nullptr;
                }
                return thiz;
            }

            JSString (
                JSString&& other
            ) noexcept : m_context{other.m_context}, m_value{other.m_value} {
                if (this != &other) {
                    thiz._size = other._size;
                    thiz.value = other.value;
                    other.value = nullptr;
                    other._size = 0;
                    other.m_value = Subprojects::quickjs::$JS_UNINITIALIZED;
                    other.m_context = nullptr;
                }
            }

            ~JSString(

            ) override
            {
                if (thiz.m_context != nullptr) {
                    Subprojects::quickjs::JS_FreeCString(thiz.m_context, thiz.value);
                    Subprojects::quickjs::JS_FreeValue(thiz.m_context, thiz.m_value);
                }
            }

            auto take_ownership (
                JSString& other
            ) -> void = delete;

            auto insert(
                const Size& index,
                Pointer<const char> str,
                const Size& len
            ) -> void override = delete;

            auto insert(
                const Size& index,
                const Character& ch
            ) -> void override = delete;

            auto insert(
                const Size& index,
                const JSString& other
            ) -> void = delete;

            auto erase(
                const Size& index,
                const Size& len
            ) -> JSString& override = delete;

            auto erase(
                Iterator first,
                Iterator last
            ) -> JSString& override = delete;

            auto push(
                const Character& c
            ) -> void override = delete;

            auto pop(
            ) -> void override = delete;

            auto append(
                const Size& count,
                Character ch
            ) -> JSString& override = delete;

            auto append(
                Pointer<const Character> s,
                const Size& count
            ) -> JSString& override = delete;

            auto append(
                Pointer<const Character> s
            ) -> JSString& override = delete;

            auto append(
                const JSString& str
            ) -> JSString& = delete;

            auto append(
                const JSString& str,
                const Size& pos,
                const Size& count = none
            ) -> JSString& = delete;

            auto append(
                std::initializer_list<Character> _
            ) -> JSString& override = delete;

            auto replace(
                const Size& pos,
                const Size& count,
                const JSString& str
            ) -> JSString& = delete;

            auto replace(
                Pointer<Character> first,
                Pointer<Character> last,
                const JSString& str
            ) -> JSString& = delete;

            auto replace(
                const Size& pos,
                const Size& count,
                const JSString& str,
                const Size& pos2,
                const Size& count2 = none
            ) -> JSString& = delete;

            auto replace(
                const Size& pos,
                const Size& count,
                Pointer<const Character> cstr,
                const Size& count2
            ) -> JSString& override = delete;

            auto replace(
                Pointer<Character> first,
                Pointer<Character> last,
                Pointer<const Character> cstr,
                const Size& count2
            ) -> JSString& override = delete;

            auto replace(
                const Size& pos,
                const Size& count,
                Pointer<const Character> cstr
            ) -> JSString& override = delete;

            auto replace(
                const Size& pos,
                const Size& count,
                const Size& count2,
                Character ch
            ) -> JSString& override = delete;

            auto replace(
                Pointer<Character> first,
                Pointer<Character> last,
                const Size& count2,
                Character ch
            ) -> JSString& override = delete;

            template <typename It>
            auto replace(
                Pointer<Character> first,
                Pointer<Character> last,
                It first2,
                It last2
            ) -> JSString& = delete;

            auto replace(
                Pointer<Character> first,
                Pointer<Character> last,
                std::initializer_list<Character> _
            ) -> JSString& override = delete;

            auto operator+=(
                const JSString& other
            ) -> JSString& = delete;

            auto operator+=(
                const char* other
            ) -> JSString& override = delete;

            auto operator+=(
                Character other
            ) -> JSString&  override = delete;

            auto operator+=(
                std::initializer_list<Character> _
            ) -> JSString& override = delete;

            template <typename JSStringViewLike> requires requires (JSStringViewLike str){
                { str.data() } -> std::same_as<Pointer<const char>>;
                { str.size() } -> std::same_as<Size>;
            }
            auto operator+=(
                const JSStringViewLike& str
            ) -> JSString& = delete;

            auto substring (
                const Size& from,
                const Size& to
            ) -> void override = delete;

            auto substring (
                const Size& from
            ) -> void override = delete;

            auto trim_left(
            ) -> void override = delete;

            auto trim_right(

            ) -> void override = delete;

            auto trim (

            ) -> void override = delete;

            auto replace_all(
                const char* target,
                const size_t& target_length,
                const char* replacement,
                const size_t& replacement_length
            ) -> void override = delete;

            auto replace_all(
                const std::string_view& target,
                const std::string_view& replacement
            ) -> void override = delete;

            auto replace_all(
                const JSString& target,
                const JSString& replacement
            ) -> void = delete;

            auto replace_all(
                const char* target,
                const char* replacement
            ) -> void override = delete;

            auto replace_all(
                char target,
                char replacement
            ) const -> void override = delete;

            auto assign (
                JSString& other
            ) -> void = delete;

            auto assign_from (
                const Value& other
            ) -> void;

            auto context (
            ) const -> Pointer<Subprojects::quickjs::JSContext> {
                return thiz.m_context;
            }

    };

}