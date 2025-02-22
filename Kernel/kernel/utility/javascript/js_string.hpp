#pragma once

#include "kernel/utility/javascript/common.hpp"
#include "kernel/utility/container/string/basic_string.hpp"
#include "kernel/utility/container/string_view/string_view.hpp"

namespace Sen::Kernel::Javascript {

    class NativeString final : public StringView {

        protected:

            Pointer<Subprojects::quickjs::JSContext> m_context;

        public:

            explicit NativeString(

            ) : StringView{}, m_context{nullptr}
            {
            }

            NativeString(
                Pointer<char> data,
                const std::size_t& size
            ) = delete;

            NativeString(
                const char* data,
                const std::size_t& size
            ) = delete;

            NativeString(
                const char* data
            ) = delete;

            NativeString(
                const std::string& other
            ) = delete;

            NativeString(
                const std::string_view& data
            ) = delete;

            NativeString(
                const BasicString& other
            ) = delete;

            explicit NativeString(
                const Size& size
            ) = delete;

            NativeString(
                const NativeString& other
            ) = delete;

            auto operator =(
                const NativeString& other
            ) -> NativeString & = delete;

            constexpr auto operator=(
                NativeString&& other
            ) noexcept -> NativeString& {
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

            NativeString (
                NativeString&& other
            ) noexcept : m_context{other.m_context} {
                if (this != &other) {
                    thiz._size = other._size;
                    thiz.value = other.value;
                    other.value = nullptr;
                    other._size = 0;
                    other.m_context = nullptr;
                }
            }

            ~NativeString(

            ) override
            {
                if (thiz.m_context != nullptr) {
                    Subprojects::quickjs::JS_FreeCString(thiz.m_context, thiz.value);
                }
            }

            auto assign_from (
                const Value& other
            ) -> void;

            [[nodiscard]] auto context (
            ) const -> Pointer<Subprojects::quickjs::JSContext> {
                return thiz.m_context;
            }

            friend auto operator <<(
                std::ostream &os,
                const NativeString& other
            ) -> std::ostream& {
                if (other.value != nullptr) {
                    os.write(other.value, static_cast<std::streamsize>(other._size));
                }
                return os;
            }

            [[nodiscard]] auto view (
            ) const -> std::string_view override {
                return std::string_view{ thiz.value, thiz._size };
            }

            constexpr auto operator==(const std::string_view& other) const noexcept -> bool {
                return this->size() == other.size() && std::memcmp(thiz.begin(), other.data(), this->size()) == 0;
            }

    };

}

template <>
struct Sen::Kernel::Transform<Sen::Kernel::StringView> {
    using type = Javascript::NativeString;
};
