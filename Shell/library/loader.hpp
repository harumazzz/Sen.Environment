#pragma once

#include "library/string.hpp"
#include "library/macro.hpp"
#include "library/standard.hpp"

namespace Sen::Shell {

    class Loader {

    protected:

        Pointer<void> handle;

    public:

        Loader() : handle{nullptr} {

        }

        constexpr auto is_open (
        ) const -> bool {
            return thiz.handle != nullptr;
        }

        constexpr auto is_closed (
        ) const -> bool {
            return thiz.handle == nullptr;
        }

        auto open (
            const std::string_view& path
        ) -> void {
            #if WINDOWS
            const auto absolute_path = u8_to_u16(path);
            thiz.handle = static_cast<void*>(LoadLibraryW(reinterpret_cast<LPCWSTR>(absolute_path.data())));
            #else
            thiz.handle = dlopen(path.data(), RTLD_LAZY | RTLD_LOCAL);
            #endif
            assert_conditional(thiz.is_open(), "Library cannot be opened");
        }

        inline auto close (
        ) const -> void {
            #if WINDOWS
            FreeLibrary(static_cast<HMODULE>(thiz.handle));
            #else
            dlclose(thiz.handle);
            #endif
            return;
        }

        template <typename Symbol>
        inline auto lookup (
            const std::string_view& name
        ) -> Symbol {
            auto address = std::add_pointer_t<void>{};
            #if WINDOWS
            address = reinterpret_cast<void*>(GetProcAddress(static_cast<HMODULE>(thiz.handle), name.data()));
            #else
            address = dlsym(thiz.handle, name.data());
            #endif
            assert_conditional(address != nullptr, "Address not found");
            return static_cast<Symbol>(address);
        }


    };

}
