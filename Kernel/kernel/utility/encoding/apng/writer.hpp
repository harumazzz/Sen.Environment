#pragma once

#include "kernel/utility/encoding/apng/setting.hpp"

namespace Sen::Kernel::Encoding::APNG {

	using DefaultSetting = Setting<uint32_t>;

	struct Writer : public Common {

		protected:

			using Common = Common;

		public:

			inline static auto find_idat(
				DataStreamViewBigEndian &stream,
				List<List<std::uint32_t>> &pixel_list
			) -> void
			{

				auto pixel = List<uint32_t>{};
				pixel.reserve(1024_size);
				auto found = 0;
				while (stream.read_pos < stream.size() - 12ull)
				{
					auto read_num = stream.readUint32();
					if (read_num == idat_uint32)
					{
						if (found > 0) {
							pixel.pop_back(); 
							pixel_list.emplace_back(pixel);
						}
						pixel.clear();
						++found;
					}
					else {
						pixel.emplace_back(read_num);
					}
				}
				pixel_list.emplace_back(pixel);
				return;
			}

			inline static auto load_image_path(
				const List<std::string> &image_path_list,
				List<ImageData> &image_data_list
			) -> void
			{
				for (const auto &image_path : image_path_list)
				{
					auto stream = DataStreamViewBigEndian{image_path};
					#if WINDOWS
					assert_conditional(stream.readString(4, 0xC_size) == "IHDR", String::format(fmt::format("{}", Language::get("mismatch_image_magic")), String::to_posix_style(image_path)), "load_image_path");
					#else
					assert_conditional(stream.readString(4, 0xC_size) == "IHDR", String::format(fmt::format("{}", Language::get("mismatch_image_magic")), image_path), "load_image_path");
					#endif
					auto image_data = ImageData{
						.meta_data = ImageHeader{
							stream.readUint32(),
							stream.readUint32(),
							0,
							0,
							stream.readUint8(),
							stream.readUint8(),
							stream.readUint8(),
							stream.readUint8(),
							stream.readUint8(),
							}
						};
						image_data.pixel_list.reserve(256);
					find_idat(stream, image_data.pixel_list);
					image_data_list.push_back(image_data);
				}
				return;
			};

			inline static auto process(
				const List<ImageData>& image_data_list,
				DataStreamViewBigEndian& stream,
				Pointer<DefaultSetting> setting
			) -> void
			{
				if (setting->width == 0 || setting->height == 0)
				{
					auto max_width = List<uint32_t>{};
					auto max_height = List<uint32_t>{};
					max_width.reserve(image_data_list.size());
					max_height.reserve(image_data_list.size());

					for (const auto& image_data : image_data_list)
					{
						max_width.emplace_back(image_data.meta_data.width);
						max_height.emplace_back(image_data.meta_data.height);
					}

					if (setting->width == 0)
					{
						setting->width = std::max_element(max_width.begin(), max_width.end()).operator*();
					}

					if (setting->height == 0)
					{
						setting->height = std::max_element(max_height.begin(), max_height.end()).operator*();
					}
				}

				stream.writeUint64(file_signature);
				stream.writeUint32(13);
				stream.writeString("IHDR");
				stream.writeUint32(setting->width);
				stream.writeUint32(setting->height);
				stream.writeUint8(8);
				stream.writeUint8(6);
				stream.writeNull(3);
				stream.writeUint32(Encryption::CRC32::Normal::compute(0, stream.readBytes(17, 12_size)));

				auto is_animated = image_data_list.size() > 1;
				if (is_animated)
				{
					stream.writeUint32(8);
					stream.writeString("acTL");
					stream.writeUint32(image_data_list.size());
					stream.writeUint32(setting->loop);
					stream.writeUint32(Encryption::CRC32::Normal::compute(0, stream.readBytes(12, 37_size)));
				}

				auto frame_index = 0;
				for (auto i : Range{image_data_list.size()})
				{
					const auto& image_data = image_data_list[i];
					if (is_animated)
					{
						stream.writeUint32(26);
						auto pos = stream.write_pos;
						stream.writeString("fcTL");
						stream.writeUint32(frame_index++);
						stream.writeUint32(image_data.meta_data.width);
						stream.writeUint32(image_data.meta_data.height);
						stream.writeUint32(image_data.meta_data.x);
						stream.writeUint32(image_data.meta_data.y);
						stream.writeUint16(static_cast<uint16_t>(setting->delay_frames_list.at(i)));
						stream.writeUint16(1000);
						stream.writeNull(2);
						stream.writeUint32(Encryption::CRC32::Normal::compute(0, stream.readBytes(30, static_cast<std::size_t>(pos))));
					}

					if (i > 0)
					{
						for (auto pixel : image_data.pixel_list)
						{
							auto data_stream = DataStreamViewBigEndian{};
							auto data_size = static_cast<uint32_t>(pixel.size() * 4);
							stream.writeUint32(data_size);
							data_stream.writeString("fdAT");
							data_stream.writeUint32(frame_index++);
							for (auto k : Range{pixel.size() - 1}) {
								data_stream.writeUint32(pixel[k]);
							}
							auto data = data_stream.toBytes();
							stream.writeBytes(data);
							stream.writeUint32(Encryption::CRC32::Normal::compute(0, data));
						}
					}
					else
					{
						for (auto& pixel : image_data.pixel_list)
						{
							stream.writeUint32(static_cast<uint32_t>((pixel.size() - 1) * 4));
							stream.writeString("IDAT");
							for (auto& pixel_num : pixel)
							{
								stream.writeUint32(pixel_num);
							}
						}
					}
				}

				stream.writeNull(4);
				stream.writeUint64(image_trailer_end);
			}

			inline static auto process_fs(
				const List<std::string>& image_path_list,
				std::string_view destination,
				Pointer<DefaultSetting> setting
			) -> void
			{
				auto image_data_list = List<ImageData>{};
				image_data_list.reserve(image_path_list.size());
				load_image_path(image_path_list, image_data_list);
				auto stream = DataStreamViewBigEndian{};
				process(image_data_list, stream, setting);
				stream.out_file(destination);
			}

	};

}