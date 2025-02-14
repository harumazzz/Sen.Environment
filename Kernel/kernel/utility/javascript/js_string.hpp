#pragma once

#include "kernel/utility/javascript/common.hpp"
#include "kernel/utility/container/string/basic_string.hpp"

namespace Sen::Kernel::Javascript {

    struct JSString : protected BasicString {

        protected:

            Pointer<Subprojects::quickjs::JSContext> m_context;

            Subprojects::quickjs::JSValue m_value;

        public:

            explicit JSString(

            ) : BasicString{}, m_context{nullptr}, m_value{Subprojects::quickjs::$JS_UNINITIALIZED}
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

            auto assign_from (
                const Value& other
            ) -> void;

            auto context (
            ) const -> Pointer<Subprojects::quickjs::JSContext> {
                return thiz.m_context;
            }

            using BasicString::contains;

            using BasicString::begin;

            using BasicString::end;

            using BasicString::data;

            using BasicString::cbegin;

            using BasicString::cend;

            using BasicString::copy;

            using BasicString::ends_with;

            using BasicString::find;

            using BasicString::find_all;

            using BasicString::length;

            using BasicString::rcontains;

            using BasicString::rfind;

            using BasicString::size;

            using BasicString::max_size;

            using BasicString::starts_with;

            using BasicString::string;

            using BasicString::sub;

            using BasicString::view;

            using BasicString::warray;

            using BasicString::wstring;

            using BasicString::operator==;

            using BasicString::operator[];

            auto as_string(
            ) const -> const BasicString& {
                return thiz;
            }

    };

}