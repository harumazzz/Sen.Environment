#pragma once

#include "kernel/support/popcap/crypt_data/common.hpp"

namespace Sen::Kernel::Support::PopCap::CryptData
{

	struct Encrypt : Common {

	    static auto process_whole(ReadMemoryStream &read_stream, WriteMemoryStream &write_stream, StringView const &key) -> void
	    {
	        const auto magic_string = String{k_magic_identifier.data(), k_magic_identifier.size()};
	        write_stream.string(magic_string);
	        write_stream.u64(read_stream.size());
	        if (read_stream.size() >= 0x100_ui)
	        {
	            auto index = k_begin_index;
	            const auto key_size = key.size();
	        	Math::for_each<0x100_size>([&]() -> void {
	        		write_stream.u8(read_stream.u8() ^ key[index++]);
					index %= key_size;
	        	});
	        }
	        auto byte = read_stream.bytes(read_stream.size() - read_stream.current_position());
	        write_stream.bytes(byte);
	    }


	    static auto process_fs(StringView const &source, StringView const &destination, StringView const& key) -> void
	    {
	        auto read_stream = ReadMemoryStream{source};
	        auto write_stream = WriteMemoryStream{};
	        process_whole(read_stream, write_stream, key);
	        FileSystem::write_file(destination, write_stream.view());
	    }

	};
}
