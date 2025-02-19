#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/zlib/common.hpp"

namespace Sen::Kernel::Support::PopCap::Zlib {

	struct Compress : Common {

	    template <auto use_variant> requires std::is_same_v<type_of<use_variant>, bool>
        static auto process_whole(WriteMemoryStream &stream, Uint8Array &value) -> void
	    {
	        stream.u32(k_magic_identifier);
	        if constexpr (use_variant) {
	            stream.u32(0_ui);
	        }
	        stream.u32(value.size());
	        if constexpr (use_variant) {
	            stream.u32(0_ui);
	        }
	        auto destination = Uint8Array{};
	        Compression::Zlib::Compress::process(value, destination);
	        stream.bytes(destination);
	    }

        static auto process_fs(
            StringView const &source,
            StringView const &destination,
			bool const &use_variant
        ) -> void
	    {
	        auto data = Uint8Array{};
	        FileSystem::read_file(source, data);
	        auto stream = WriteMemoryStream{data.size()};
	    	if (use_variant) {
				process_whole<true>(stream, data);
			} else {
				process_whole<false>(stream, data);
			}
	        FileSystem::write_file(destination, stream.view());
	    }
	};

}