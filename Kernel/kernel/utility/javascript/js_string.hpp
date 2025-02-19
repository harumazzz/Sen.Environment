#pragma once

#include "kernel/utility/javascript/common.hpp"
#include "kernel/utility/container/string/basic_string.hpp"
#include "kernel/utility/container/string_view/string_view.hpp"

namespace Sen::Kernel::Javascript {

    struct JSString : public StringView {

        protected:

            Pointer<Subprojects::quickjs::JSContext> m_context;

        public:

            explicit JSString(

            ) : StringView{}, m_context{nullptr}
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
                    thiz.m_context = other.m_context;
                    other.value = nullptr;
                    other._size = 0;
                    other.m_context = nullptr;
                }
                return thiz;
            }

            JSString (
                JSString&& other
            ) noexcept : m_context{other.m_context} {
                if (this != &other) {
                    thiz._size = other._size;
                    thiz.value = other.value;
                    other.value = nullptr;
                    other._size = 0;
                    other.m_context = nullptr;
                }
            }

            ~JSString(

            ) override
            {
                if (thiz.m_context != nullptr) {
                    Subprojects::quickjs::JS_FreeCString(thiz.m_context, thiz.value);
                }
            }

            auto assign_from (
                const Value& other
            ) -> void;

            auto context (
            ) const -> Pointer<Subprojects::quickjs::JSContext> {
                return thiz.m_context;
            }

            auto view (
            ) const -> std::string_view override {
                return std::string_view{ thiz.value, thiz._size };
            }

    };

}

template <>
struct Sen::Kernel::Transform<Sen::Kernel::StringView> {
    using type = Javascript::JSString;
};
