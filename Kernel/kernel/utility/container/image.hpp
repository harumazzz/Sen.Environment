#pragma once

#include "kernel/utility/assert.hpp"
#include "kernel/utility/library.hpp"
#include "kernel/utility/macro.hpp"
#include "kernel/utility/container/array.hpp"
#include "kernel/utility/trait/trait.hpp"

namespace Sen::Kernel {

	template <typename T> requires IsCategoryOfInteger and Integral<T>

	struct Dimension {

		public:

			// width and height

			T width{};

			T height{};

			// constructor

			explicit Dimension(

			) = default;

			// constructor

			Dimension(
				T width,
				T height
			) : width{width}, height{height}
			{

			}

			// destructor

			~Dimension(

			) = default;

			// get circumference

			inline auto circumference(

			) const noexcept -> T
			{
				return (thiz.width + thiz.height) * 2;
			}
			
			// get area

			inline auto area(

			) const noexcept -> T
			{
				return thiz.width * thiz.height;
			}
	};

	/**
	 * Rectangle Struct
	*/

	template <typename T> requires IsCategoryOfInteger and Integral<T>

	struct Rectangle : public Dimension<T> {

		public:

			// position in image

			T x{};

			T y{};

			/**
			 * constructor
			*/

			constexpr Rectangle(

			) = default;

			/**
			 * destructor
			*/

			~Rectangle(

			) = default;

			/**
			 * constructor
			*/

			Rectangle(
				T x, 
				T y,
				T width,
				T height
			) : x{x}, y{y}, Dimension<T>{width, height}
			{

			}

			/**
			 * constructor
			*/

			Rectangle(
				const Dimension<T> &that,
				T x, 
				T y
			) : x(x), y(y), Dimension<T>(that.width, that.height)
			{

			}

			/**
			 * constructor
			*/

			Rectangle(
				const Rectangle &that
			) : x(that.x), y(that.y), Dimension<T>(that.width, that.height)
			{

			}

			Rectangle(
				Rectangle&& that
			) : x(that.x), y(that.y), Dimension<T>(that.width, that.height)
			{

			}
	};

	/**
	 * Image color
	*/

	struct Color {

		public:

			// RGBA 
			
			// 0 - 1 - 2 - 3

			// red (i = 0)

			List<unsigned char> red{};

			// green (i = 1)

			List<unsigned char> green{};

			// blue (i = 2)

			List<unsigned char> blue{};

			// alpha (i = 3)

			List<unsigned char> alpha{};
			
			// destructor

			~Color(

			) = default;

			// constructor

			explicit constexpr Color(

			) = default;
	};

	template <typename T> requires IsCategoryOfInteger && Integral<T>
	struct Image : public Rectangle<T> {

		private:
			
			List<unsigned char> mutable _data{};

		public:

			// easy accessible data
			T bit_depth{};
			T color_type{};
			T interlace_type{};
			T channels{};
			T rowbytes{};

			explicit constexpr Image(
				T x,
				T y,
				T width, 
				T height, 
				T bit_depth, 
				T color_type, 
				T interlace_type, 
				T channels, 
				T rowbytes, 
				List<unsigned char> && data
			) : 
			Rectangle<T>{x, y, width, height}, 
			bit_depth{bit_depth},
			color_type{color_type}, 
			interlace_type{interlace_type}, 
			channels{channels}, 
			rowbytes{rowbytes}, 
			_data(std::move(data))
			{

			}

			Image(
				Image&& that
			) noexcept
				: Rectangle<T>(std::move(that)),
				bit_depth(std::move(that.bit_depth)),
				color_type(std::move(that.color_type)),
				interlace_type(std::move(that.interlace_type)),
				channels(std::move(that.channels)),
				rowbytes(std::move(that.rowbytes)),
				_data(std::move(that._data))
			{

			}

			explicit constexpr Image(
				T x,
				T y,
				T width, 
				T height, 
				T bit_depth, 
				T color_type, 
				T interlace_type, 
				T channels, 
				T rowbytes, 
				const List<unsigned char> & data
			) : 
			Rectangle<T>(x, y, width, height), 
			bit_depth(bit_depth),
			color_type(color_type), 
			interlace_type(interlace_type), 
			channels(channels), 
			rowbytes(rowbytes), 
			_data(std::move(data))
			{

			}

			explicit constexpr Image(
				const Image &that
			) : Image<T>{that.x, that.y, that.width, that.height, that.bit_depth, that.color_type, that.interlace_type, that.channels, that.rowbytes, that.data()}
			{

			}

			Image& operator=(
				Image&& that
			) noexcept
			{
				if (this != &that) {
					Rectangle<T>::operator=(std::move(that));
					bit_depth = std::move(that.bit_depth);
					color_type = std::move(that.color_type);
					interlace_type = std::move(that.interlace_type);
					channels = std::move(that.channels);
					rowbytes = std::move(that.rowbytes);
					_data = std::move(that._data);
				}
				return *this;
			}

			inline auto data(

			) const -> const List<unsigned char> & 
			{
				return thiz._data;
			}

			inline auto set_data(
				List<unsigned char> && data
			) const -> void
			{
				thiz._data = std::move(data);
				return;
			}

			inline auto copy_data(
				const List<unsigned char> & data
			) const -> void
			{
				thiz._data = std::move(data);
				return;
			}

			inline auto color(

			) const -> Color
			{
				auto c = Color{};
				auto size = thiz._data.size() / 4;
				c.red.reserve(size);
				c.alpha.reserve(size);
				c.blue.reserve(size);
				c.green.reserve(size);
				for (auto i = static_cast<size_t>(0); i < thiz._data.size(); i += 4)
				{
					c.red.push_back(thiz._data.at(i));
					c.green.push_back(thiz._data.at(i + 1));
					c.blue.push_back(thiz._data.at(i + 2));
					c.alpha.push_back(thiz._data.at(i + 3));
				}
				return c;
			}

			inline auto is_rgb (

			) -> bool
			{
				return thiz.channels == 3;
			}

			Image(
				T x,
				T y,
				T width,
				T height,
				const List<unsigned char>& data
			) : Rectangle<T>{x, y, width, height}, _data(std::move(data))
			{
			}

			explicit Image(

			) = default;

			~Image(

			) = default;

			using ZList = List<uint8_t>;

			inline auto dimension(

			) const -> Dimension<T>&
			{
				return thiz;
			}

			inline static auto cut(
				const Image<int>& image,
				const Rectangle<int>& rectangle
			) -> Image<int>
			{
				auto data = ZList{};
				data.reserve(static_cast<ZList::size_type>(rectangle.area()) * 4);
				for (auto j : Range{ rectangle.y, rectangle.y + rectangle.height, 1 }) {
					for (auto i : Range{ rectangle.x, rectangle.x + rectangle.width, 1 }) {
						auto index = (j * image.width + i) * 4;
						data.insert(data.end(), &image.data()[index], &image.data()[static_cast<List<uint8_t>::size_type>(index) + 4]);
					}
				}
				return Image<int>{0, 0, rectangle.width, rectangle.height, std::move(data)};
			}

			inline static auto transparent(
				const Dimension<T>& dimension
			) -> Image<T>
			{
				return Image<T>{0, 0, dimension.width, dimension.height, List<unsigned char>(dimension.area() * 4, 0x00)};
			}

			inline static auto join(
				Image<T>& source,
				const List<Image<T>>& data
			) -> void
			{
				auto source_data = source.data();
				for (auto& img : data) {
					if (!(img.width + img.x <= source.width and img.height + img.y <= source.height)) {
						throw Exception(fmt::format("{}", Language::get("image.does_not_fit_current_image")), std::source_location::current(), "join");
					}
					auto& image_data = img.data();
					for (auto j : Range<T>{ img.height }) {
						for (auto i : Range<T>{ img.width }) {
							auto source_index = ((j + img.y) * source.width + (i + img.x)) * 4;
							auto img_index = (j * img.width + i) * 4;
							source_data[source_index] = image_data[img_index];
							source_data[source_index + 1] = image_data[img_index + 1];
							source_data[source_index + 2] = image_data[img_index + 2];
							source_data[source_index + 3] = image_data[img_index + 3];
						}
					}
				}
				source.set_data(std::move(source_data));
				return;
			}

			inline auto normalize_32bit(
				std::string_view source
			) -> void
			{
				if (thiz.is_rgba()) {
					return;
				}
				else if (thiz.is_rgb()) {
					auto &data = thiz.data();
					auto color = ZList(static_cast<ZList::size_type>((data.size() / 3) * 4_size));
					for (auto i = 0_size, j = 0_size; i < data.size(); i += 3, j += 4) {
						color[j] = data[i];
						color[j + 1] = data[i + 1];
						color[j + 2] = data[i + 2];
						color[j + 3] = 0xFF_ui;
					}
					copy_data(color);
					thiz.channels = 4;
				}
				else if (thiz.is_grayscale_alpha()) {
					auto &data = thiz.data();
					auto color = ZList(static_cast<ZList::size_type>((data.size() / 2) * 4));
					for (auto i = 0_size, j = 0_size; i < data.size(); i += 2, j += 4) {
						color[j] = data[i];           
						color[j + 1] = data[i];       
						color[j + 2] = data[i];       
						color[j + 3] = data[i + 1];   
					}
					copy_data(color);
					thiz.channels = 4;
				} 
				else if (thiz.is_grayscale()) {
					auto &data = thiz.data();
					auto color = ZList(static_cast<ZList::size_type>(data.size() * 4));
					for (auto i = 0_size, j = 0_size; i < data.size(); ++i, j += 4) {
						color[j] = data[i];       
						color[j + 1] = data[i];   
						color[j + 2] = data[i];   
						color[j + 3] = 0xFF_ui;
					}
					copy_data(color);
					thiz.channels = 4;
				}
				else {
					assert_conditional(false, fmt::format("{}: {}", Language::get("image_is_broken"), String::to_posix_style(std::string{source.data(), source.size()})), "normalize_32bit");
				}
				return;
			}

			inline auto is_grayscale_alpha(

			) -> bool
			{
				return thiz.channels == 2;
			}

			inline auto is_grayscale(

			) -> bool
			{
				return thiz.channels == 1;
			}

			inline auto is_rgba(

			) -> bool
			{
				return thiz.channels == 4;
			}

			inline static auto join_extend(
				Image<T>& source,
				List<Image<T>>& data
			) -> void
			{
				auto source_data = source.data();
				for (auto& img : data) {
					if (!(img.width + img.x <= source.width and img.height + img.y <= source.height)) {
						throw Exception(fmt::format("{}", Language::get("image.does_not_fit_current_image")), std::source_location::current(), "join");
					}
					auto& image_data = img.data();
					auto extend_image = resize(img, img.width + 2, img.height + 2);
					auto &extend_data = extend_image.data();
					for (auto j : Range<T>{ extend_image.height }) {
						for (auto i : Range<T>{ extend_image.width }) {
							auto source_index = ((j + img.y - 1) * source.width + (i + img.x - 1)) * 4;
							auto img_index = (j * extend_image.width + i) * 4;
							source_data[source_index] = extend_data[img_index];
							source_data[source_index + 1] = extend_data[img_index + 1];
							source_data[source_index + 2] = extend_data[img_index + 2];
							source_data[source_index + 3] = extend_data[img_index + 3];
						}
					}
					for (auto j : Range<T>{ img.height }) {
						for (auto i : Range<T>{ img.width }) {
							auto source_index = ((j + img.y) * source.width + (i + img.x)) * 4;
							auto img_index = (j * img.width + i) * 4;
							source_data[source_index] = image_data[img_index];
							source_data[source_index + 1] = image_data[img_index + 1];
							source_data[source_index + 2] = image_data[img_index + 2];
							source_data[source_index + 3] = image_data[img_index + 3];
						}
					}
				}
				source.set_data(std::move(source_data));
				return;
			}

			inline static auto resize(
				const Image<T>& source,
				float percent
			) -> Image<T>
			{
				auto new_width = static_cast<int>(Math::ceil(source.width * percent));
				auto new_height = static_cast<int>(Math::ceil(source.height * percent));
				auto resized_image_data = List<unsigned char>(static_cast<size_t>(new_width * new_height * 4));
				for (auto j : Range{ new_height }) {
					for (auto i : Range{ new_width }) {
						auto old_i = static_cast<int>(i / percent);
						auto old_j = static_cast<int>(j / percent);
						auto old_index = (old_j * source.width + old_i) * 4;
						auto new_index = (j * new_width + i) * 4;
						std::copy(&source.data()[old_index], &source.data()[old_index + 4], &resized_image_data[new_index]);
					}
				}
				return Image<T>{0, 0, new_width, new_height, std::move(resized_image_data)};
			}

			inline static auto resize(
				const Image<T>& source,
				int new_width,
				int new_height
			) -> Image<T>
			{
				auto width_percent = static_cast<float>(new_width) / static_cast<float>(source.width);
				auto height_percent = static_cast<float>(new_height) / static_cast<float>(source.height);
				auto resized_image_data = List<unsigned char>(static_cast<size_t>(new_width * new_height * 4));
				for (auto j : Range{ new_height }) {
					for (auto i : Range{ new_width }) {
						auto old_i = static_cast<int>(i / width_percent);
						auto old_j = static_cast<int>(j / height_percent);
						auto old_index = (old_j * source.width + old_i) * 4;
						auto new_index = (j * new_width + i) * 4;
						std::copy(&source.data()[old_index], &source.data()[old_index + 4], &resized_image_data[new_index]);
					}
				}
				return Image<T>{0, 0, new_width, new_height, std::move(resized_image_data)};
			}

			inline static auto rotate(
				const Image<T>& image, 
				double angle
			) -> Image<T> const
			{
				auto new_width = static_cast<int>(std::abs(image.width * std::cos(angle)) + std::abs(image.height * std::sin(angle)));
				auto new_height = static_cast<int>(std::abs(image.height * std::cos(angle)) + std::abs(image.width * std::sin(angle)));
				auto data = List<unsigned char>(static_cast<size_t>(new_width * new_height * 4, 0));
				auto center_x = static_cast<double>(image.width) / 2.0;
				auto center_y = static_cast<double>(image.height) / 2.0;
				for (auto j : Range{ new_height }) {
					for (auto i : Range{ new_width }) {
						auto old_i = static_cast<int>((i - new_width / 2.0) * std::cos(angle) + (j - new_height / 2.0) * std::sin(angle) + center_x);
						auto old_j = static_cast<int>((j - new_height / 2.0) * std::cos(angle) - (i - new_width / 2.0) * std::sin(angle) + center_y);
						if (old_i >= 0 && old_i < image.width && old_j >= 0 && old_j < image.height) {
							auto old_index = (old_j * image.width + old_i) * 4;
							auto new_index = (j * new_width + i) * 4; 
							std::copy(&image.data()[old_index], &image.data()[old_index + 4], &data[new_index]);
						}
					}
				}
				return Image<T>{0, 0, new_width, new_height, std::move(data)};
			}

			inline static auto scale(
				const Image<int> & source,
				float percentage
			) -> Image<int> const
			{
				auto new_width = static_cast<int>(source.width * percentage);
				auto new_height = static_cast<int>(source.height * percentage);
				auto area = (new_width * new_height * 4);
				auto data = std::make_unique<uint8_t[]>(area);
				avir::CImageResizer<>{8}.resizeImage(
					source.data().data(), 
					static_cast<int>(source.width), 
					static_cast<int>(source.height), 
					0, 
					data.get(), 
					static_cast<int>(new_width), 
					static_cast<int>(new_height), 
					4, 
					0.0, 
					nullptr
				);
				auto vec = List<uint8_t>{ data.get(), data.get() + area };
				return Image<int>{0, 0, new_width, new_height, std::move(vec)};
			}
	};

	template <typename T> requires Integral<T>

	struct RectangleFileIO : Rectangle<T> {
		
		public:

			std::string destination;


			RectangleFileIO(

			) = default;


			RectangleFileIO(
				T x,
				T y,
				T width,
				T height
			) : Rectangle<T>(x, y, width, height)
			{
			}


			RectangleFileIO(
				T x,
				T y,
				T width,
				T height,
				const std::string& destination
			) : Rectangle<T>(x, y, width, height), destination(destination)
			{
			}

			RectangleFileIO(
				const RectangleFileIO& that
			) : Rectangle<T>(that.x, that.y, that.width, that.height), destination(that.destination)
			{
			}

			RectangleFileIO(
				const Rectangle<T> &that,
				const std::string& destination
			) : Rectangle<T>(that), destination(destination)
			{
			}

			~RectangleFileIO(

			) = default;
	};


	struct ImageIO {

		public:

			inline static auto png_error(
				png_structp png_ptr,
				png_const_charp error_message
			) -> void {
				// TODO : improve this
				assert_conditional(false, fmt::format("png has error: {}", error_message), "png_error");
				return;
			}

			inline static auto png_warning(
				png_structp png_ptr,
				png_const_charp error_message
			) -> void {
				assert_conditional(false, fmt::format("png has error: {}", error_message), "png_warning");
				return;
			}

			inline static auto read_png(
				std::string_view source
			) -> Image<int> 
			{
				auto open_file = [](std::string_view path) -> std::unique_ptr<FILE, decltype(Language::close_file)> {
				#if WINDOWS
					auto windows_path = String::utf8view_to_utf16(fmt::format("\\\\?\\{}", String::to_windows_style(path.data())));
					return { _wfopen(windows_path.data(), L"rb"), Language::close_file };
				#else
					return { std::fopen(path.data(), "rb"), Language::close_file };
				#endif
				};
				auto file = open_file(source);
				#if WINDOWS
				assert_conditional(file != nullptr, fmt::format("{}: {}", Language::get("image.open_png_failed"), String::to_posix_style(std::string{ source.data(), source.size() })), "read_png");
				#else
				assert_conditional(file != nullptr, fmt::format("{}: {}", Language::get("image.open_png_failed"), source), "read_png");
				#endif
				auto png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
				png_set_error_fn(png_ptr, nullptr, &png_error, &png_warning);
				if(png_ptr == nullptr){
					throw Exception(fmt::format("{}: {}", Language::get("image.png_pointer_init_failed"), source), std::source_location::current(), "read_png");
				}
				auto info_ptr = png_create_info_struct(png_ptr);  
				if(info_ptr == nullptr){
					png_destroy_read_struct(&png_ptr, nullptr, nullptr);
					throw Exception(fmt::format("{}: {}", Language::get("image.info_pointer_init_failed"), source), std::source_location::current(), "read_png");
				}
				if (setjmp(png_jmpbuf(png_ptr))) {
					png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
					throw Exception(fmt::format("{}: {}", Language::get("image.unknown_error"), source), std::source_location::current(), "read_png");
				}
				png_init_io(png_ptr, file.get());
				png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, nullptr);
				auto row_pointers = png_get_rows(png_ptr, info_ptr);
				auto data = List<unsigned char>{};
				auto width = static_cast<int>(png_get_image_width(png_ptr, info_ptr));
				auto height = static_cast<int>(png_get_image_height(png_ptr, info_ptr));
				auto bit_depth = static_cast<int>(png_get_bit_depth(png_ptr, info_ptr));
				auto channels = static_cast<int>(png_get_channels(png_ptr, info_ptr));
				{
					auto row_size = width * bit_depth * channels / 8;
					data.reserve(height * row_size);
				}
				for (auto y : Range{height}) {
					for (auto x : Range{width * bit_depth * channels / 8}) {
						data.emplace_back(row_pointers[y][x]);
					}
				};
				auto color_type = static_cast<int>(png_get_color_type(png_ptr, info_ptr));
				auto interlace_type = static_cast<int>(png_get_interlace_type(png_ptr, info_ptr));
				auto rowbytes = static_cast<int>(png_get_rowbytes(png_ptr, info_ptr));
				png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
				auto image = Image<int>{
					0,
					0,
					width, 
					height, 
					bit_depth,
					color_type,
					interlace_type,
					channels,
					rowbytes,
					std::move(data)
				};
				image.normalize_32bit(source);
				return image;
			}

			inline static auto write_png(
				std::string_view destination, 
				const Image<int> &data
			) -> void
			{
				auto open_file = [](std::string_view path) -> std::unique_ptr<FILE, decltype(Language::close_file)> {
				#if WINDOWS
					auto windows_path = String::utf8view_to_utf16(fmt::format("\\\\?\\{}", String::to_windows_style(path.data())));
					return { _wfopen(windows_path.data(), L"wb"), Language::close_file };
				#else
					return { std::fopen(path.data(), "wb"), Language::close_file };
				#endif
				};
				auto file = open_file(destination);
				auto png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
				if(png_ptr == nullptr){
					throw Exception(fmt::format("{}: {}", Language::get("image.png_pointer_init_failed"), destination), std::source_location::current(), "write_png");
				}
				auto info_ptr = png_create_info_struct(png_ptr);
				if (info_ptr == nullptr) {
					png_destroy_write_struct(&png_ptr, nullptr);
					throw Exception(fmt::format("{}: {}", Language::get("image.info_pointer_init_failed"), destination), std::source_location::current(), "write_png");
				}
				if (_setjmp((*png_set_longjmp_fn((png_ptr), longjmp, (sizeof(jmp_buf)))))) {
					png_destroy_write_struct(&png_ptr, &info_ptr); 
					throw Exception(fmt::format("{}: {}", Language::get("image.unknown_error"), destination), std::source_location::current(), "write_png");
				};
				png_init_io(png_ptr, file.get());
				png_set_IHDR(
					png_ptr,
					info_ptr,
					data.width,
					data.height,
					8,
					PNG_COLOR_TYPE_RGB_ALPHA,
					PNG_INTERLACE_NONE,
					PNG_COMPRESSION_TYPE_DEFAULT, 
					PNG_FILTER_TYPE_DEFAULT
				);
				auto row_pointers = List<png_bytep>(data.height);
				for (auto y : Range{ data.height }) {
					row_pointers[y] = const_cast<unsigned char*>(&data.data()[static_cast<std::vector<uint8_t, std::allocator<uint8_t>>::size_type>(y) * data.width * 4]);
				}
				png_write_info(png_ptr, info_ptr);
				png_write_image(png_ptr, row_pointers.data());
				png_write_end(png_ptr, nullptr);
				png_destroy_write_struct(&png_ptr, &info_ptr);
				png_write_end(png_ptr, nullptr);
				png_destroy_write_struct(&png_ptr, &info_ptr);
				return;
			}

			inline static auto cut_fs(
				std::string_view source,
				std::string_view destination,
				const Rectangle<int> & rectangle
			) -> void
			{
				ImageIO::write_png(destination, Image<int>::cut(ImageIO::read_png(source), rectangle));
				return;
			}

			inline static auto cut_pngs(
				std::string_view source,
				const List<RectangleFileIO<int>> & data
			) -> void
			{
				auto image = ImageIO::read_png(source);
				std::for_each(data.begin(), data.end(), [&](auto c)
				{ 
					ImageIO::write_png(c.destination, Image<int>::cut(image, c));
				});
				return;
			}

			inline static auto cut_pngs_asynchronous(
				std::string_view source,
				const List<RectangleFileIO<int>> & data
			) -> void
			{
				auto image = ImageIO::read_png(source);
				auto process = List<std::future<void>>{};
				process.reserve(data.size());
				for (auto &c : data) {
					process.emplace_back(std::async(std::launch::async, [&]{
						ImageIO::write_png(c.destination, Image<int>::cut(image, c));
					}));
				}
				for(auto &f : process) {
					f.get();
				}
				return;
			}


			inline static auto transparent_png(
				std::string_view destination,
				int width,
				int height
			) -> void
			{
				ImageIO::write_png(destination, Image<int>::transparent(Dimension<int>(width, height)));
				return;
			}

			inline static auto join_png(
				std::string_view destination,
				const Dimension<int> & dimension,
				const List<Image<int>> & data
			) -> void
			{
				auto source = Image<int>::transparent(dimension);
				Image<int>::join(source, data);
				ImageIO::write_png(destination, source);
				return;
			}

			inline static auto join(
				Image<int> & destination,
				const List<Image<int>>& data
			) -> void
			{
				Image<int>::join(destination, data);
				return;
			}

			inline static auto join_extend(
				Image<int> & destination,
				List<Image<int>>& data
			) -> void
			{
				Image<int>::join_extend(destination, data);
				return;
			}

			inline static auto resize_png(
				std::string_view source,
				std::string_view destination,
				float percent
			) -> void
			{
				ImageIO::write_png(destination, Image<int>::resize(ImageIO::read_png(source), percent));
				return;
			}

			inline static auto rotate_png(
				std::string_view source,
				std::string_view destination,
				double angle
			) -> void
			{
				ImageIO::write_png(destination, Image<int>::rotate(ImageIO::read_png(source), angle));
				return;
			}

			inline static auto scale_png(
				std::string_view source,
				std::string_view destination,
				float percent
			) -> void
			{
				ImageIO::write_png(destination, Image<int>::scale(ImageIO::read_png(source), percent));
				return;
			}

	};
}