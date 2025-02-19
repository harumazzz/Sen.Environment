#pragma once

#include "kernel/support/popcap/crypt_data/common.hpp"
#include "kernel/utility/algorithm/math.hpp"

namespace Sen::Kernel::Support::PopCap::CryptData
{

	struct Decrypt : Common {

	    static auto process_whole(ReadMemoryStream &read_stream, WriteMemoryStream &write_stream, StringView const &key) -> void
	    {
	        const auto magic_string = String{k_magic_identifier.data(), k_magic_identifier.size()};
	        assert_conditional((read_stream.string(k_magic_identifier.size()) == magic_string), fmt::format("{}", Kernel::Language::get("popcap.crypt_data.decrypt.mismatch_magic")), "process_whole");
	        write_stream.allocate_full(read_stream.u64());
	    	if (read_stream.size() > 0x100_size)
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
