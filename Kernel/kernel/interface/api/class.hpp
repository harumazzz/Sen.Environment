#pragma once

#include "kernel/interface/shell.hpp"
#include "kernel/utility/javascript/builder.hpp"

namespace Sen::Kernel::Interface::API {

	#pragma region DataStreamView

	namespace DataStreamView {

		inline auto toArrayBuffer(
			Pointer<Kernel::DataStreamView> stream
		) -> std::shared_ptr<JavaScript::ArrayBuffer> {
			auto bytes = stream->toBytes();
			return std::make_shared<JavaScript::ArrayBuffer>(bytes.data(), bytes.size());
		}

		inline auto getArrayBuffer(
			Pointer<Kernel::DataStreamView> stream,
			size_t& from,
			size_t& to
		) -> std::shared_ptr<JavaScript::ArrayBuffer> {
			auto bytes = stream->get(from, to);
			return std::make_shared<JavaScript::ArrayBuffer>(bytes.data(), bytes.size());
		}

		inline auto register_class(
			Pointer<JSContext> context,
			JavaScript::NamespaceBuilder& class_builder
		) -> void
		{
			using DataStreamView = Kernel::DataStreamView;
			auto builder = JavaScript::ClassBuilder<DataStreamView>{ context, "DataStreamView" };
			builder.add_constructor<[](std::string& source) -> Pointer<DataStreamView> {
				return new DataStreamView{source};
			}, std::string&>()
			.template add_getter_setter<size_t, [](Pointer<DataStreamView> stream){
				return stream->read_pos;
			}, [](Pointer<DataStreamView> stream, size_t& read_position){
				stream->read_pos = read_position;
			}>("read_position")
			.template add_getter_setter<size_t, [](Pointer<DataStreamView> stream){
				return stream->write_pos;
			}, [](Pointer<DataStreamView> stream, size_t& write_position){
				stream->write_pos = write_position;
			}>("read_position")
			.template add_member_function<[](Pointer<DataStreamView> stream){
				return stream->size();
			}, size_t>("size")
			.template add_member_function<[](Pointer<DataStreamView> stream, std::string& source){
				return stream->fromString(source);
			}, void, std::string&>("fromString")
			.template add_member_function<[](Pointer<DataStreamView> stream){
				return stream->capacity();
			}, size_t>("capacity")
			.template add_member_function<[](Pointer<DataStreamView> stream, size_t& size){
				return stream->reserve(size);
			}, void, size_t&>("reserve")
			.template add_member_function<toArrayBuffer, std::shared_ptr<JavaScript::ArrayBuffer>>("toArrayBuffer")
			.template add_member_function<getArrayBuffer, std::shared_ptr<JavaScript::ArrayBuffer>, size_t&, size_t&>("getArrayBuffer")
			.template add_member_function<[](Pointer<DataStreamView> stream){
				return stream->toString();
			}, std::string>("toString")
			.template add_member_function<[](Pointer<DataStreamView> stream, std::string& destination){
				return stream->out_file(destination);
			}, void, std::string&>("out_file")
			.template add_member_function<[](Pointer<DataStreamView> stream, size_t& size){
				return stream->allocate(size);
			}, void, size_t&>("allocate")
			.template add_member_function<[](Pointer<DataStreamView> stream, uint8_t value){
				return stream->writeUint8(value);
			}, void, uint8_t>("writeUint8")

			.build(class_builder);
			return;
		}

	}

	#pragma endregion

	
	#pragma region Clock

	namespace Clock {

		inline auto register_class(
			Pointer<JSContext> context,
			JavaScript::NamespaceBuilder& class_builder
		) -> void
		{
			using Clock = Kernel::Clock;
			auto builder = JavaScript::ClassBuilder<Clock>{ context, "Clock" };
			builder.add_constructor<[]() -> Pointer<Clock> {
				return new Clock{};
			}>()
			.template add_member_function<[](Pointer<Clock> clock){
				return clock->start_safe();
			}, void>("start_safe")
			.template add_member_function <[](Pointer<Clock> clock) {
			return clock->stop_safe();
			}, void > ("stop_safe")
			.template add_member_function <[](Pointer<Clock> clock) {
			return clock->reset();
			}, void> ("reset")
			.template add_member_function <[](Pointer<Clock> clock) {
			return clock->get_duration();
			}, int64_t> ("get_duration")
			.template add_member_function <[](Pointer<Clock> clock) {
			return clock->is_started();
			}, bool > ("is_started")
			.template add_member_function <[](Pointer<Clock> clock) {
			return clock->is_stopped();
			}, bool > ("is_stopped")
			.template add_member_function <[](Pointer<Clock> clock) {
			return static_cast<double>(clock->get_duration() / 1000.0);
			}, double> ("duration_as_seconds").build(class_builder);
			return;
		}

	}

	#pragma endregion
	
	#pragma region JsonWriter

	namespace JsonWriter {

		inline auto register_class(
			Pointer<JSContext> context,
			JavaScript::NamespaceBuilder& class_builder
		) -> void
		{
			using JsonWriter = Kernel::JsonWriter;
			auto builder = JavaScript::ClassBuilder<JsonWriter>{ context, "JsonWriter" };
			builder.add_constructor<[]() -> Pointer<JsonWriter> {
				return new JsonWriter{};
			}>()
			.template add_member_function<[](Pointer<JsonWriter> writer){
				return writer->clear();
			}, void> ("clear")
			.template add_member_function<[](Pointer<JsonWriter> writer) {
				return writer->to_string();
			}, std::string>("toString")
			.template add_member_function<[](Pointer<JsonWriter> writer){
				return writer->write_start_array();
			}, void>("writeStartArray")
			.template add_member_function<[](Pointer<JsonWriter> writer){
				return writer->write_end_array();
			}, void>("writeEndArray")
			.template add_member_function<[](Pointer<JsonWriter> writer){
				return writer->write_start_object();
			}, void>("writeStartObject")
			.template add_member_function<[](Pointer<JsonWriter> writer){
				return writer->write_end_object();
			}, void>("writeEndObject")
			.template add_member_function<[](Pointer<JsonWriter> writer, bool value){
				return writer->write_boolean(value);
			}, void, bool>("writeBoolean")
			.template add_member_function<[](Pointer<JsonWriter> writer){
				return writer->write_null();
			}, void>("writeNull")
			.template add_member_function<[](Pointer<JsonWriter> writer, std::string& property_name){
				return writer->write_property_name(property_name);
			}, void, std::string&>("writePropertyName")
			.template add_member_function<[](Pointer<JsonWriter> writer, std::string& value){
				return writer->write_value(value);
			}, void, std::string&>("writeString")
			.template add_member_function<[](Pointer<JsonWriter> writer, int64_t& value){
				return writer->write_value(value);
			}, void, int64_t&>("writeBigInt")
			.template add_member_function<[](Pointer<JsonWriter> writer, double& value){
				return writer->write_value(value);
			}, void, double&>("writeNumber")
			.template add_getter_setter<bool, [](Pointer<JsonWriter> writer){
				return writer->WriteIndent;
			}, [](Pointer<JsonWriter> writer, bool& value){
				writer->WriteIndent = value;
			}>("writeIndent").build(class_builder);
			return;
		}

	}

	#pragma endregion

	#pragma region Canvas

	namespace Canvas {

		inline auto register_class(
			Pointer<JSContext> context,
			JavaScript::NamespaceBuilder& class_builder
		) -> void
		{
			using Canvas = canvas_ity::canvas;
			using BrushType = canvas_ity::brush_type;
			auto builder = JavaScript::ClassBuilder<Canvas>{ context, "Canvas" };
			builder.add_constructor<[](int64_t& width, int64_t& height) -> Pointer<Canvas> {
				return new Canvas{ static_cast<int>(width), static_cast<int>(height) };
			}, int64_t&, int64_t&>()
			.template add_member_function<[](Pointer<Canvas> canvas, float& x, float& y){
				return canvas->scale(x, y);
			}, void, float&, float&> ("scale")
			.template add_member_function<[](Pointer<Canvas> canvas, float& angle){
				return canvas->rotate(angle);
			}, void, float&> ("rotate")
			.template add_member_function<[](Pointer<Canvas> canvas, float& x, float& y){
				return canvas->translate(x, y);
			}, void, float&, float&> ("translate")
			.template add_member_function<[](Pointer<Canvas> canvas, float& a, float& b, float& c, float& d, float& e, float& f){
				return canvas->transform(a, b, c, d, e, f);
			}, void, float&, float&, float&, float&, float&, float&> ("transform")
			.template add_member_function<[](Pointer<Canvas> canvas, float& a, float& b, float& c, float& d, float& e, float& f){
				return canvas->set_transform(a, b, c, d, e, f);
			}, void, float&, float&, float&, float&, float&, float&> ("set_transform")
			.template add_member_function<[](Pointer<Canvas> canvas, float& alpha){
				return canvas->set_global_alpha(alpha);
			}, void, float&> ("set_global_alpha")
			.template add_member_function<[](Pointer<Canvas> canvas, float& red, float& green, float& blue, float& alpha){
				return canvas->set_shadow_color(red, green, blue, alpha);
			}, void, float&, float&, float&, float&> ("set_shadow_color")
			.template add_member_function<[](Pointer<Canvas> canvas, float& width){
				return canvas->set_line_width(width);
			}, void, float&> ("set_line_width")
			.template add_member_function<[](Pointer<Canvas> canvas, float& level){
				return canvas->set_shadow_blur(level);
			}, void, float&> ("set_shadow_blur")
			.template add_member_function<[](Pointer<Canvas> canvas, float& limit){
				return canvas->set_miter_limit(limit);
			}, void, float&> ("set_miter_limit")
			.template add_member_function<[](Pointer<Canvas> canvas, int32_t& type, float& red, float& green, float& blue, float& alpha){
				return canvas->set_color(static_cast<BrushType>(type), red, green, blue, alpha);
			}, void, int32_t&, float&, float&, float&, float&> ("set_color")
			.template add_member_function<[](Pointer<Canvas> canvas, int32_t& type, float& start_x, float& start_y, float& end_x, float& end_y){
				return canvas->set_linear_gradient(static_cast<BrushType>(type), start_x, start_y, end_x, end_y);
			}, void, int32_t&, float&, float&, float&, float&> ("set_linear_gradient")
			.template add_member_function<[](Pointer<Canvas> canvas, int32_t& type, float& start_x, float& start_y, float& start_radius, float& end_x, float& end_y, float& end_radius){
				return canvas->set_radial_gradient(static_cast<BrushType>(type), start_x, start_y, start_radius, end_x, end_y, end_radius);
			}, void, int32_t&, float&, float&, float&, float&, float&, float&> ("set_radial_gradient")
			.template add_member_function<[](Pointer<Canvas> canvas, int32_t& type, float& offset, float& red, float& green, float& blue, float& alpha){
				return canvas->add_color_stop(static_cast<BrushType>(type), offset, red, green, blue, alpha);
			}, void, int32_t&, float&, float&, float&, float&, float&> ("add_color_stop")
			.template add_member_function<[](Pointer<Canvas> canvas){
				return canvas->begin_path();
			}, void> ("begin_path")
			.template add_member_function<[](Pointer<Canvas> canvas, float& x, float& y){
				return canvas->move_to(x, y);
			}, void, float&, float&> ("move_to")
			.template add_member_function<[](Pointer<Canvas> canvas){
				return canvas->close_path();
			}, void> ("close_path")
			.template add_member_function<[](Pointer<Canvas> canvas, float& x, float& y){
				return canvas->line_to(x, y);
			}, void, float&, float&> ("line_to")
			.template add_member_function<[](Pointer<Canvas> canvas, float& control_x, float& control_y, float& x, float& y){
				return canvas->quadratic_curve_to(control_x, control_y, x, y);
			}, void, float&, float&, float&, float&> ("quadratic_curve_to")
			.template add_member_function<[](Pointer<Canvas> canvas, float& control_1x, float& control_1y, float& control_2x, float& control_2y, float& x, float& y){
				return canvas->bezier_curve_to(control_1x, control_1y, control_2x, control_2y, x, y);
			}, void, float&, float&, float&, float&, float&, float&> ("bezier_curve_to")
			.template add_member_function<[](Pointer<Canvas> canvas, float& vertex_x, float& vertex_y, float& x, float& y, float& radius){
				return canvas->arc_to(vertex_x, vertex_y, x, y, radius);
			}, void, float&, float&, float&, float&, float&> ("arc_to")
			.template add_member_function<[](Pointer<Canvas> canvas, float& red, float& green, float& blue, float& alpha){
				return canvas->set_image_color(red, green, blue, alpha);
			}, void, float&, float&, float&, float&> ("set_image_color")
			.template add_member_function<[](Pointer<Canvas> canvas, float& x, float& y, float& start_angle, float& end_angle, bool counter_clockwise){
				return canvas->arc(x, y, start_angle, end_angle, counter_clockwise);
			}, void, float&, float&, float&, float&, bool> ("arc")
			.template add_member_function<[](Pointer<Canvas> canvas, float& x, float& y, float& width, float& height){
				return canvas->rectangle(x, y, width, height);
			}, void, float&, float&, float&, float&> ("rectangle")
			.template add_member_function<[](Pointer<Canvas> canvas){
				return canvas->fill();
			}, void> ("fill")
			.template add_member_function<[](Pointer<Canvas> canvas){
				return canvas->stroke();
			}, void> ("stroke")
			.template add_member_function<[](Pointer<Canvas> canvas, float& x, float& y){
				return canvas->is_point_in_path(x, y);
			}, bool, float&, float&> ("is_point_in_path")
			.template add_member_function<[](Pointer<Canvas> canvas, float& x, float& y, float& width, float& height){
				return canvas->clear_rectangle(x, y, width, height);
			}, void, float&, float&, float&, float&> ("clear_rectangle")
			.template add_member_function<[](Pointer<Canvas> canvas, float& x, float& y, float& width, float& height){
				return canvas->fill_rectangle(x, y, width, height);
			}, void, float&, float&, float&, float&> ("fill_rectangle")
			.template add_member_function<[](Pointer<Canvas> canvas, std::shared_ptr<JavaScript::ArrayBuffer>& data, int64_t& bytes){
				return canvas->set_font(reinterpret_cast<unsigned char*>(data->value), static_cast<int>(bytes), static_cast<float>(data->size));
			}, bool, std::shared_ptr<JavaScript::ArrayBuffer>&, int64_t&> ("set_font")
			.template add_member_function<[](Pointer<Canvas> canvas, std::shared_ptr<JavaScript::ArrayBuffer>& data, int64_t& width, int64_t& height, int64_t& stride, float& x, float& y, float& to_width, float& to_height){
				return canvas->draw_image(reinterpret_cast<unsigned char*>(data->value), static_cast<int>(width), static_cast<int>(height), static_cast<int>(stride), x, y, to_width, to_height);
			}, void, std::shared_ptr<JavaScript::ArrayBuffer>&, int64_t&, int64_t&, int64_t&, float&, float&, float&, float&> ("draw_image")
			.template add_member_function<[](Pointer<Canvas> canvas, std::shared_ptr<JavaScript::ArrayBuffer>& data, int64_t& width, int64_t& height, int64_t& stride, float& x, float& y){
				return canvas->get_image_data(data->value, static_cast<int>(width), static_cast<int>(height), static_cast<int>(stride), x, y);
			}, void, std::shared_ptr<JavaScript::ArrayBuffer>&, int64_t&, int64_t&, int64_t&, float&, float&> ("get_image_data")
			.template add_member_function<[](Pointer<Canvas> canvas, std::shared_ptr<JavaScript::ArrayBuffer>& data, int64_t& width, int64_t& height, int64_t& stride, int64_t& x, int64_t& y){
				return canvas->put_image_data(data->value, static_cast<int>(width), static_cast<int>(height), static_cast<int>(stride), static_cast<int>(x), static_cast<int>(y));
			}, void, std::shared_ptr<JavaScript::ArrayBuffer>&, int64_t&, int64_t&, int64_t&, int64_t&, int64_t&> ("put_image_data")
			.template add_member_function<[](Pointer<Canvas> canvas){
				return canvas->save();
			}, void> ("save")
			.template add_member_function<[](Pointer<Canvas> canvas){
				return canvas->restore();
			}, void> ("restore").build(class_builder);
			return;
		}

	}

	#pragma endregion

	#pragma region ImageView

	namespace ImageView {

		using Image = Kernel::Image <int32_t>;

		inline auto get_data(
			Image* image
		) -> std::shared_ptr<JavaScript::ArrayBuffer>
		{
			return std::make_shared<JavaScript::ArrayBuffer>(const_cast<uint8_t*>(image->data().data()), image->data().size());
		}

		inline auto set_data(
			Image* image,
			std::shared_ptr<JavaScript::ArrayBuffer>& data
		) -> void
		{
			image->set_data(std::move(List<uint8_t>{data->value, data->value + data->size}));
			return;
		}

		inline auto cut (
			Pointer<Image> image,
			std::shared_ptr<JavaScript::Rectangle>& area
		) -> Pointer<Image> {
			auto data = Image::cut(*image, area.operator*());
			return new Image {
				data.x,
				data.y,
				data.width,
				data.height,
				data.bit_depth,
				data.color_type,
				data.interlace_type,
				data.channels,
				data.rowbytes,
				data.data(),
			};
		}

		inline auto resize (
			Pointer<Image> image,
			float& percentage
		) -> Pointer<Image> {
			auto data = Image::resize(*image, percentage);
			return new Image{
				data.x,
				data.y,
				data.width,
				data.height,
				data.bit_depth,
				data.color_type,
				data.interlace_type,
				data.channels,
				data.rowbytes,
				data.data(),
			};
		}

		inline auto scale (
			Pointer<Image> image,
			float& percentage
		) -> Pointer<Image> {
			auto data = Image::scale(*image, percentage);
			return new Image{
				data.x,
				data.y,
				data.width,
				data.height,
				data.bit_depth,
				data.color_type,
				data.interlace_type,
				data.channels,
				data.rowbytes,
				data.data(),
			};
		}

		inline auto rotate(
			Pointer<Image> image,
			double& angle
		) -> Pointer<Image> {
			auto data = Image::rotate(*image, angle);
			return new Image{
				data.x,
				data.y,
				data.width,
				data.height,
				data.bit_depth,
				data.color_type,
				data.interlace_type,
				data.channels,
				data.rowbytes,
				data.data(),
			};
		}

		inline auto write_fs (
			std::string& destination,
			Pointer<Image> image
		) -> void {
			Kernel::ImageIO::write_png(destination, *image);
		}

		inline auto instance (
			int32_t& width,
			int32_t& height,
			std::shared_ptr<JavaScript::ArrayBuffer>& data
		) -> Pointer<Image> {
			return new Image{
				0,
				0,
				width,
				height,
				std::move(List<uint8_t>{data->value, data->value + data->size}),
			};
		}

		inline auto read_fs(
			std::string& source
		) -> Pointer<Image> {
			auto data = Kernel::ImageIO::read_png(source);
			return new Image{
				data.x,
				data.y,
				data.width,
				data.height,
				data.bit_depth,
				data.color_type,
				data.interlace_type,
				data.channels,
				data.rowbytes,
				data.data(),
			};
		}

		inline auto constructor(
			std::shared_ptr<JavaScript::ImageView>& data
		) -> Pointer<Image> {
			return new Image{ 0, 0, data->width, data->height, data->bit_depth, data->color_type, data->interlace_type, data->channels, data->rowbytes, std::move(List<uint8_t>{data->data.value, data->data.value + data->data.size}) };
		}

		inline auto register_class(
			Pointer<JSContext> context,
			JavaScript::NamespaceBuilder& class_builder
		) -> void
		{
			using ImageView = Image;
			auto builder = JavaScript::ClassBuilder<ImageView>{ context, "ImageView" };
			builder.add_constructor<constructor, std::shared_ptr<JavaScript::ImageView>&>()
			.template add_getter_setter<int, [](Pointer<ImageView> image){
				return image->width;
			}, [](Pointer<ImageView> image, int& width) {
				image->width = width;
			}>("width")
			.template add_getter_setter<int, [](Pointer<ImageView> image) {
			return image->height;
			}, [](Pointer<ImageView> image, int& height) {
			image->height = height;
			}>("height")
			.template add_getter_setter<int, [](Pointer<ImageView> image) {
				return image->bit_depth;
			}, [](Pointer<ImageView> image, int& bit_depth) {
				image->bit_depth = bit_depth;
			}>("bit_depth")
			.template add_getter_setter<int, [](Pointer<ImageView> image) {
				return image->bit_depth;
			}, [](Pointer<ImageView> image, int& color_type) {
				image->color_type = color_type;
			}>("color_type")
			.template add_getter_setter<int, [](Pointer<ImageView> image) {
				return image->interlace_type;
			}, [](Pointer<ImageView> image, int& interlace_type) {
				image->interlace_type = interlace_type;
			}>("interlace_type")
			.template add_getter_setter<int, [](Pointer<ImageView> image) {
				return image->channels;
			}, [](Pointer<ImageView> image, int& channels) {
				image->channels = channels;
			}>("channels")
			.template add_getter_setter<int, [](Pointer<ImageView> image) {
				return image->rowbytes;
			}, [](Pointer<ImageView> image, int& rowbytes) {
				image->rowbytes = rowbytes;
			}>("rowbytes")
			.template add_getter_setter<std::shared_ptr<JavaScript::ArrayBuffer>, get_data, set_data>("data")
			.template add_static_function<cut, Pointer<ImageView>, Pointer<ImageView>, std::shared_ptr<JavaScript::Rectangle>&>("cut")
			.template add_static_function<resize, Pointer<ImageView>, Pointer<ImageView>, float&>("resize")
			.template add_static_function<scale, Pointer<ImageView>, Pointer<ImageView>, float&>("scale")
			.template add_static_function<rotate, Pointer<ImageView>, Pointer<ImageView>, double&>("rotate")
			.template add_static_function<read_fs, Pointer<ImageView>, std::string&>("read_fs")
			.template add_static_function<write_fs, void, std::string&, Pointer<ImageView>>("write_fs")
			.template add_static_function<instance, Pointer<ImageView>, int32_t&, int32_t&, std::shared_ptr<JavaScript::ArrayBuffer>&>("instance")
			.build(class_builder);
		}

	}

	#pragma endregion

	#pragma region APNGMakerSetting

	namespace APNGMakerSetting {

		inline auto register_class(
			Pointer<JSContext> context,
			JavaScript::NamespaceBuilder& class_builder
		) -> void
		{
			using APNGMakerSetting = Kernel::APNGMakerSetting;
			auto builder = JavaScript::ClassBuilder<Kernel::APNGMakerSetting>{ context, "APNGMakerSetting" };
			builder.add_constructor<[](List<uint32_t>& delay_frames_list, uint32_t& loop, uint32_t& width, uint32_t& height, bool trim){
				return new APNGMakerSetting{ delay_frames_list , loop, width, height, trim };
			}, List<uint32_t>&, uint32_t&, uint32_t&, uint32_t&, bool>()
			.template add_getter_setter<List<uint32_t>, [](Pointer<APNGMakerSetting> setting){
				return setting->delay_frames_list;
			}, [](APNGMakerSetting* setting, List<uint32_t>& delay_frames_list) {
				setting->delay_frames_list = delay_frames_list;
			}>("delay_frames_list")
			.template add_getter_setter<uint32_t, [](Pointer<APNGMakerSetting> setting) {
				return setting->loop;
			}, [](Pointer<APNGMakerSetting> setting, uint32_t& loop) {
				setting->loop = loop;
			}> ("loop")
			.template add_getter_setter<uint32_t, [](Pointer<APNGMakerSetting> setting){
				return setting->width;
			}, [](Pointer<APNGMakerSetting> setting, uint32_t& width) {
				setting->width = width;
			}>("width")
			.template add_getter_setter<uint32_t, [](Pointer<APNGMakerSetting> setting) {
				return setting->height;
			}, [](Pointer<APNGMakerSetting> setting, uint32_t& height) {
				setting->height = height;
			}>("height")
			.template add_getter_setter<bool, [](Pointer<APNGMakerSetting> setting) {
				return setting->trim;
			}, [](Pointer<APNGMakerSetting> setting, bool& trim) {
				setting->trim = trim;
			}>("trim").build(class_builder);
			return;
		}

	}

	#pragma endregion

	#pragma region Support

	namespace Support {

		#pragma region Popcap

		namespace PopCap {

			#pragma region Animation

			namespace Animation {

				#pragma region Miscellaneous

				namespace Miscellaneous {

					#pragma region Image 

					namespace Image {

						inline auto register_class(
							Pointer<JSContext> context,
							JavaScript::NamespaceBuilder& class_builder
						) -> void
						{
							using Image = Kernel::Support::PopCap::Animation::Miscellaneous::Image;
							using Transform = std::array<double, 6>;
							auto builder = JavaScript::ClassBuilder<Image>{ context, "Image" };
							builder.add_constructor<[](std::string& name, std::string& id, Transform& transform) {
								return new Image{ name, id, transform };
							}, std::string&, std::string&, Transform&>()
							.template add_getter_setter<std::string, [](Pointer<Image> image){
								return image->name;
							}, [](Pointer<Image> image, std::string& name){
								image->name = name;
							}>("name")
							.template add_getter_setter<std::string, [](Pointer<Image> image){
								return image->id;
							}, [](Pointer<Image> image, std::string& id){
								image->id = id;
							}>("id")
							.template add_getter_setter<Transform, [](Pointer<Image> image){
								return image->transform;
							}, [](Pointer<Image> image, Transform& transform){
								image->transform = transform;
							}>("transform").build(class_builder);
							return;
						}

					}

					#pragma endregion

					#pragma region Sprite 

					namespace Sprite {

						inline auto register_class(
							Pointer<JSContext> context,
							JavaScript::NamespaceBuilder& class_builder
						) -> void
						{
							using Sprite = Kernel::Support::PopCap::Animation::Miscellaneous::Sprite;
							using Transform = std::array<double, 6>;
							using Color = std::array<double, 4>;
							auto builder = JavaScript::ClassBuilder<Sprite>{ context, "Sprite" };
							builder.add_constructor<[](std::string& name, std::string& link, Transform& transform, Color& color) {
								return new Sprite{ name, link, transform, color };
							}, std::string&, std::string&, Transform&, Color&>()
							.template add_getter_setter<std::string, [](Pointer<Sprite> sprite){
								return sprite->name;
							}, [](Pointer<Sprite> sprite, std::string& name){
								sprite->name = name;
							}>("name")
							.template add_getter_setter<std::string, [](Pointer<Sprite> sprite){
								return sprite->link;
							}, [](Pointer<Sprite> sprite, std::string& link){
								sprite->link = link;
							}>("link")
							.template add_getter_setter<Transform, [](Pointer<Sprite> sprite){
								return sprite->transform;
							}, [](Pointer<Sprite> sprite, Transform& transform){
								sprite->transform = transform;
							}>("transform")
							.template add_getter_setter<Color, [](Pointer<Sprite> sprite){
								return sprite->color;
							}, [](Pointer<Sprite> sprite, Color& color){
								sprite->color = color;
							}>("color").build(class_builder);
							return;
						}

					}

					#pragma endregion

				}

				#pragma endregion

			}

			#pragma endregion

		}

		#pragma endregion

	}

	#pragma endregion
}