#pragma once

#include "kernel/definition/utility.hpp"
#include "kernel/support/popcap/re_animation/definition.hpp"

namespace Sen::Kernel::Support::PopCap::ReAnimation::Convert
{

	class FromFlash
	{
	private:
		using XMLDocument = tinyxml2::XMLDocument;

		using XMLElement = tinyxml2::XMLElement;

	public:
		inline auto process(
			std::string_view source,
			ReanimInfo &reanim) -> void
		{
			auto document = XMLDocument{};
			FileSystem::read_xml(fmt::format("{}/DOMDocument.xml", source), &document);
			auto timelines_elements = document.FirstChildElement("DOMDocument")->FirstChildElement("timelines");
			assert_conditional(timelines_elements != nullptr, fmt::format("{}", Language::get("popcap.reanim.from_flash.has_no_timelines")), "process");
			auto DOMTimeline = timelines_elements->FirstChildElement("DOMTimeline");
			assert_conditional(DOMTimeline != nullptr, fmt::format("{}", Language::get("popcap.reanim.from_flash.has_no_DOMTimeline")), "process");
			auto layers = DOMTimeline->FirstChildElement("layers");
			assert_conditional(layers != nullptr, fmt::format("{}", Language::get("popcap.reanim.from_flash.has_no_layers")), "process");
			auto &tracks = reanim.tracks;
			for (auto DOMLayer = layers->LastChildElement("DOMLayer"); DOMLayer != nullptr; DOMLayer = DOMLayer->PreviousSiblingElement("DOMLayer"))
			{
				auto name_element = DOMLayer->FindAttribute("name");
				assert_conditional(name_element != nullptr, fmt::format("{}", Language::get("popcap.reanim.from_flash.invalid_name")), "process");
				auto name = std::string{name_element->Value()};
				auto frames = DOMLayer->FirstChildElement("frames");
				assert_conditional(frames != nullptr, fmt::format("{}", Language::get("popcap.reanim.from_flash.document_has_no_frame")), "process");
				auto initial_transform = ReanimTransform{0, 0, 1, 1, 0, 0, 0, 1};
				auto frame_switch = false;
				auto track = ReanimTrack{};
				track.name = name;
				for (auto DOMFrame = frames->FirstChildElement("DOMFrame"); DOMFrame != nullptr; DOMFrame = DOMFrame->NextSiblingElement("DOMFrame"))
				{
					auto frame_index = Converter::to_int32(DOMFrame->FindAttribute("index")->Value(), String::format(fmt::format("{}", Language::get("popcap.reanim.invalid_argument")), std::string{"index"}));
					auto elements = DOMFrame->FirstChildElement("elements");
					if (elements == nullptr)
					{
						continue;
					}
					auto DOMSymbolInstance = elements->FirstChildElement("DOMSymbolInstance");
					auto this_transform = ReanimTransform{};
					if (DOMSymbolInstance == nullptr)
					{
						if (!frame_switch)
						{
							this_transform.f = -1.0f;
							frame_switch = true;
						}
						track.transforms.emplace_back(this_transform);
						break;
					}
					auto name_item_element = DOMSymbolInstance->FindAttribute("libraryItemName");
					assert_conditional(name_item_element != nullptr, fmt::format("{}", Language::get("popcap.reanim.from_flash.invalid_name_item")), "process");
					auto name_item = std::string{name_item_element->Value()};
					if (initial_transform.i != name_item) {
						initial_transform.i = name_item;
						this_transform.i = name_item;
					}
					auto matrix = DOMSymbolInstance->FirstChildElement("matrix");
					if (matrix != nullptr)
					{
						auto Matrix = matrix->FirstChildElement("Matrix");
						assert_conditional(Matrix != nullptr, fmt::format("{}", Language::get("popcap.reanim.from_flash.invalid_matrix")), "process");
						auto a = Matrix->FindAttribute("a");
						auto b = Matrix->FindAttribute("b");
						auto c = Matrix->FindAttribute("c");
						auto d = Matrix->FindAttribute("d");
						auto tx = Matrix->FindAttribute("tx");
						auto ty = Matrix->FindAttribute("ty");
						const auto &a_v = Converter::to_float64((a != nullptr ? a->Value() : "1"), String::format(fmt::format("{}", Language::get("popcap.animation.from_flash.matrix_is_not_a_valid_number")), std::string{"a"}, std::string{a->Value()}));
						const auto &b_v = Converter::to_float64((b != nullptr ? b->Value() : "0"), String::format(fmt::format("{}", Language::get("popcap.animation.from_flash.matrix_is_not_a_valid_number")), std::string{"b"}, std::string{b->Value()}));
						const auto &c_v = Converter::to_float64((c != nullptr ? c->Value() : "0"), String::format(fmt::format("{}", Language::get("popcap.animation.from_flash.matrix_is_not_a_valid_number")), std::string{"c"}, std::string{c->Value()}));
						const auto &d_v = Converter::to_float64((d != nullptr ? d->Value() : "1"), String::format(fmt::format("{}", Language::get("popcap.animation.from_flash.matrix_is_not_a_valid_number")), std::string{"d"}, std::string{d->Value()}));
						const auto &tx_v = Converter::to_float32((tx != nullptr ? tx->Value() : "0"), String::format(fmt::format("{}", Language::get("popcap.animation.from_flash.matrix_is_not_a_valid_number")), std::string{"tx"}, std::string{tx->Value()}));
						const auto &ty_v = Converter::to_float32((ty != nullptr ? ty->Value() : "0"), String::format(fmt::format("{}", Language::get("popcap.animation.from_flash.matrix_is_not_a_valid_number")), std::string{"ty"}, std::string{ty->Value()}));
						const auto &skew_x = std::atan2(b_v, a_v);
						const auto &skew_y = std::atan2(c_v, d_v);
						auto dx = 57.2957795131;
						auto quarter_pi = 0.785398163;
						auto sx = 0.0f;
						auto sy = 0.0f;
						if (std::abs(skew_x) < quarter_pi || std::abs(skew_x) > quarter_pi * 3)
						{
							sx = static_cast<float>(a_v / std::cos(skew_x));
						}
						else
						{
							sx = static_cast<float>(b_v / std::sin(skew_x));
						}
						if (std::abs(skew_y) < quarter_pi || std::abs(skew_y) > quarter_pi * 3)
						{
							sy = static_cast<float>(d_v / std::cos(skew_x));
						}
						else
						{
							sy = static_cast<float>(c_v / std::sin(skew_x));
						}
						auto kx = static_cast<float>(dx * skew_x);
						auto ky = static_cast<float>(dx * skew_y);
						if (initial_transform.x != tx_v)
						{
							initial_transform.x = tx_v;
							this_transform.x = tx_v;
						}
						if (initial_transform.y != ty_v)
						{
							initial_transform.y = ty_v;
							this_transform.y = ty_v;
						}
						if (initial_transform.kx != kx)
						{
							initial_transform.kx = kx;
							this_transform.kx = kx;
						}
						if (initial_transform.ky != ky)
						{
							initial_transform.ky = ky;
							this_transform.ky = ky;
						}
						if (initial_transform.sx != sx)
						{
							initial_transform.sx = sx;
							this_transform.sx = sx;
						}
						if (initial_transform.sy != sy)
						{
							initial_transform.sy = sy;
							this_transform.sy = sy;
						}
						if (frame_switch)
						{
							this_transform.f = 0;
							frame_switch = false;
						}
					}
					auto color = DOMSymbolInstance->FirstChildElement("color");
					if (color != nullptr)
					{
						auto Color = color->FirstChildElement("Color");
						assert_conditional(Color != nullptr, fmt::format("{}", Language::get("popcap.reanim.from_flash.invalid_color")), "process");
						auto alpha = Converter::to_float32(Color->FindAttribute("alphaMultiplier")->Value(), String::format(fmt::format("{}", Language::get("popcap.reanim.matrix_is_not_a_valid_number")), std::string{"alphaMultiplier"}, std::string{Color->FindAttribute("alphaMultiplier")->Value()}));
						if (initial_transform.a != alpha)
						{
							initial_transform.a = alpha;
							this_transform.a = alpha;
						}
					}
					track.transforms.emplace_back(this_transform);
				}
				tracks.emplace_back(track);
			}
			auto frameRate = document.FirstChildElement("DOMDocument")->FindAttribute("frameRate");
			reanim.fps = Converter::to_float32(frameRate != nullptr ? frameRate->Value() : "12", String::format(fmt::format("{}", Language::get("popcap.animation.from_flash.frame_rate_is_not_a_integer")), std::string{frameRate->Value()}));
			return;
		}

		inline static auto process_fs(
			std::string_view source,
			std::string_view destination) -> void
		{
			auto from_flash = FromFlash{};
			auto reanim = ReanimInfo{};
			from_flash.process(source, reanim);
			FileSystem::write_json(destination, reanim);
			return;
		}
	};
}