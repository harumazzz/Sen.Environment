#pragma once

#include "kernel/utility/utility.hpp"
#include "kernel/support/popcap/zlib/compress.hpp"

namespace Sen::Kernel::Support::PopCap::CompiledText {


	struct Encode {

	    template <auto use_variant> requires std::is_same_v<type_of<use_variant>, bool>
        static auto process_whole(Uint8Array &data, Uint8Array & value, StringView const &key, StringView const& iv) -> void {
	        auto dest = UCharacterArray{};
	        Encryption::Base64::Encode::process(data, dest);
	        // TODO: wait Rjindael.

	    }

        static auto process_fs(
        	StringView const& source,
        	StringView const &destination,
        	StringView const &key,
        	StringView const& iv,
        	bool const &use_variant
        ) -> void
	    {
	        auto data = Uint8Array{};
	        FileSystem::read_file(source, data);
	        auto value = Uint8Array{};
	        if (use_variant) {
	            process_whole<true>(data, value, key, iv);
	        } else {
	            process_whole<false>(data, value, key, iv);
	        }
	        FileSystem::write_file(destination, value);
	    }
	};
}