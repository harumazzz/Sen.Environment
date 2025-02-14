#pragma once

#include "kernel/interface/api/class.hpp"
#include "kernel/interface/version.hpp"
#include "kernel/utility/javascript/runtime.hpp"

namespace Sen::Kernel::Interface::API {

    template <typename T>
    using List = CList<T>;

    template <typename T>
    using Array = CArray<T>;

    inline auto version (
    ) -> usize {
        return Kernel::version;
    }

    namespace FileSystem {

        inline auto read_file (
            const String& source
        ) -> String {
            auto destination = String{};
            Kernel::FileSystem::read_file(source , destination);
            return destination;
        }

        inline auto write_file (
            const String& source,
            const String& destination
        ) -> void {
            Kernel::FileSystem::write_file(source, const_cast<String&>(destination));
        }

        template <auto mode> requires std::is_same_v<type_of<mode>, Path::PathType>
        inline auto read_current_directory (
            const String& source
        ) -> List<String> {
            auto destination = List<String>{};
            Kernel::FileSystem::read_directory<mode>(source , destination);
            return destination;
        }

        inline auto read_directory (
            const String& source
        ) -> List<String> {
            auto destination = List<String>{};
            Kernel::FileSystem::read_recursive_directory_file(source , destination);
            return destination;
        }

    }

    namespace Path {

        inline static auto join(
			List<String>& source
		) -> String
		{
			return Kernel::Path::join(source);
		}

    }

}