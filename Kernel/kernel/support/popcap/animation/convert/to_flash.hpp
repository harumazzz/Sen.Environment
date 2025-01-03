#pragma once

#include "kernel/definition/utility.hpp"
#include "kernel/support/popcap/animation/convert/definition.hpp"
#include "kernel/support/popcap/animation/convert/common.hpp"

namespace Sen::Kernel::Support::PopCap::Animation::Convert
{
	template <auto split_label>
	struct ToFlash : Common
	{
	protected:
		inline static auto exchange_image_document(
			AnimationImage const &image,
			std::string const &image_name,
			XMLDocument &value) -> void
		{
			auto image_transform_matrix = Transform{};
			k_version < 2 ? exchange_tranform_from_rotate_to_standard(image.transform, image_transform_matrix) : exchange_tranform_by_copy(image.transform, image_transform_matrix);
			auto dom_symbol_item = value.NewElement("DOMSymbolItem");
			dom_symbol_item->SetAttribute("xmlns:xsi", k_xmlns_xsi_attribute.data());
			dom_symbol_item->SetAttribute("xmlns", k_xmlns_attribute.data());
			dom_symbol_item->SetAttribute("name", fmt::format("image/{}", image_name).data());
			dom_symbol_item->SetAttribute("symbolType", k_symbol_type.data());
			auto dom_timeline = value.NewElement("DOMTimeline");
			dom_timeline->SetAttribute("name", image_name.data());
			auto dom_bitmap_instance = value.NewElement("DOMBitmapInstance");
			dom_bitmap_instance->SetAttribute("libraryItemName", fmt::format("media/{}", image.path).data());
			auto transform_matrix = value.NewElement("Matrix");
			transform_matrix->SetAttribute("a", to_fixed<6>(image_transform_matrix[0] * k_media_scale_ratio).data());
			transform_matrix->SetAttribute("b", to_fixed<6>(image_transform_matrix[1]).data());
			transform_matrix->SetAttribute("c", to_fixed<6>(image_transform_matrix[2]).data());
			transform_matrix->SetAttribute("d", to_fixed<6>(image_transform_matrix[3] * k_media_scale_ratio).data());
			transform_matrix->SetAttribute("tx", to_fixed<6>(image_transform_matrix[4]).data());
			transform_matrix->SetAttribute("ty", to_fixed<6>(image_transform_matrix[5]).data());
			auto matrix = value.NewElement("matrix");
			matrix->InsertEndChild(transform_matrix);
			dom_bitmap_instance->InsertEndChild(matrix);
			auto dom_frame = value.NewElement("DOMFrame");
			dom_frame->SetAttribute("index", 0);
			auto elements = value.NewElement("elements");
			elements->InsertEndChild(dom_bitmap_instance);
			dom_frame->InsertEndChild(elements);
			auto frames = value.NewElement("frames");
			frames->InsertEndChild(dom_frame);
			auto dom_layer = value.NewElement("DOMLayer");
			dom_layer->InsertEndChild(frames);
			auto layers = value.NewElement("layers");
			layers->InsertEndChild(dom_layer);
			dom_timeline->InsertEndChild(layers);
			auto timeline = value.NewElement("timeline");
			timeline->InsertEndChild(dom_timeline);
			dom_symbol_item->InsertEndChild(timeline);
			value.InsertEndChild(dom_symbol_item);
			return;
		}

		template <auto sprite_type>
		inline static auto exchange_sprite_document(
			AnimationNameList const &animation_name_list,
			std::string const &name,
			FrameNodeStructure &frame_node_structure,
			XMLDocument &value) -> void
		{
			static_assert(sizeof(sprite_type) == sizeof(SpriteType));
			auto dom_symbol_item = value.NewElement("DOMSymbolItem");
			dom_symbol_item->SetAttribute("xmlns:xsi", k_xmlns_xsi_attribute.data());
			dom_symbol_item->SetAttribute("xmlns", k_xmlns_attribute.data());
			if constexpr (sprite_type == SpriteType::sprite)
			{
				dom_symbol_item->SetAttribute("name", fmt::format("sprite/{}", name).data());
			}
			else if constexpr (sprite_type == SpriteType::label)
			{
				dom_symbol_item->SetAttribute("name", fmt::format("label/{}", name).data());
			}
			else if constexpr (sprite_type == SpriteType::main_sprite)
			{
				dom_symbol_item->SetAttribute("name", fmt::format("{}", name).data());
			}
			else
			{
				static_assert("invalid_sprite_type");
			}
			dom_symbol_item->SetAttribute("symbolType", k_symbol_type.data());
			auto dom_timeline = value.NewElement("DOMTimeline");
			dom_timeline->SetAttribute("name", name.data());
			auto layers = value.NewElement("layers");
			auto dom_layer_index = k_begin_index_int + 1;
			for (auto &[layer_index, frame_node_list] : frame_node_structure)
			{
				auto dom_layer = value.NewElement("DOMLayer");
				auto frames = value.NewElement("frames");
				for (auto &frame_node : frame_node_list)
				{
					auto dom_frame = value.NewElement("DOMFrame");
					dom_frame->SetAttribute("index", std::to_string(frame_node.index).data());
					dom_frame->SetAttribute("duration", std::to_string(frame_node.duration).data());
					auto elements = value.NewElement("elements");
					if (frame_node.resource != k_native_index)
					{
						auto dom_symbol_instance = value.NewElement("DOMSymbolInstance");
						if (frame_node.sprite)
						{
							if (static_cast<size_t>(frame_node.resource) >= animation_name_list.sprite.size())
							{
								continue; // skip vaild
							}
							dom_symbol_instance->SetAttribute("libraryItemName", fmt::format("sprite/{}", animation_name_list.sprite[frame_node.resource]).data());
							dom_symbol_instance->SetAttribute("firstFrame", std::to_string(frame_node.first_frame).data());
						}
						else
						{
							if (static_cast<size_t>(frame_node.resource) >= animation_name_list.image.size())
							{
								continue; // skip vaild
							}
							dom_symbol_instance->SetAttribute("libraryItemName", fmt::format("image/{}", animation_name_list.image[frame_node.resource]).data());
						}
						dom_symbol_instance->SetAttribute("symbolType", k_symbol_type.data());
						dom_symbol_instance->SetAttribute("loop", "loop");
						auto transform_matrix = value.NewElement("Matrix");
						transform_matrix->SetAttribute("a", to_fixed<6>(frame_node.transform[0]).data());
						transform_matrix->SetAttribute("b", to_fixed<6>(frame_node.transform[1]).data());
						transform_matrix->SetAttribute("c", to_fixed<6>(frame_node.transform[2]).data());
						transform_matrix->SetAttribute("d", to_fixed<6>(frame_node.transform[3]).data());
						transform_matrix->SetAttribute("tx", to_fixed<6>(frame_node.transform[4]).data());
						transform_matrix->SetAttribute("ty", to_fixed<6>(frame_node.transform[5]).data());
						auto frame_color = value.NewElement("Color");
						frame_color->SetAttribute("redMultiplier", to_fixed<6>(frame_node.color[0]).data());
						frame_color->SetAttribute("greenMultiplier", to_fixed<6>(frame_node.color[1]).data());
						frame_color->SetAttribute("blueMultiplier", to_fixed<6>(frame_node.color[2]).data());
						frame_color->SetAttribute("alphaMultiplier", to_fixed<6>(frame_node.color[3]).data());
						auto matrix = value.NewElement("matrix");
						matrix->InsertEndChild(transform_matrix);
						auto color = value.NewElement("color");
						color->InsertEndChild(frame_color);
						dom_symbol_instance->InsertEndChild(matrix);
						dom_symbol_instance->InsertEndChild(color);
						elements->InsertEndChild(dom_symbol_instance);
					}
					dom_frame->InsertEndChild(elements);
					frames->InsertEndChild(dom_frame);
				}
				dom_layer->SetAttribute("name", std::to_string(dom_layer_index).data());
				dom_layer->InsertEndChild(frames);
				layers->InsertFirstChild(dom_layer);
				++dom_layer_index;
			}
			dom_timeline->InsertEndChild(layers);
			auto timeline = value.NewElement("timeline");
			timeline->InsertEndChild(dom_timeline);
			dom_symbol_item->InsertEndChild(timeline);
			value.InsertEndChild(dom_symbol_item);
			frame_node_structure.clear();
			return;
		}

		inline static auto check_and_rewrite_vaild_label(
			std::string &label) -> void
		{
			if (label.size() > k_max_label_name_size)
			{
				label = label.substr(0, k_max_label_name_size);
			}
			exchange_sprite_name_invalid<true>(label);
			return;
		}

		template <auto get_label>
		inline static auto exchange_frame_node(
			SexyAnimation const &definition,
			AnimationSprite const &sprite,
			PackageLibrary &package_library) -> void
		{
			static_assert(get_label == true || get_label == false, "get_label must be true or false");
			auto model_structure = std::map<int, Model>{};
			auto label_name = std::string{"animation"};
			auto label_duplicate_system_stored = std::map<std::string, size_t>{};
			auto &frame_node_structure = package_library.frame_node;
			for (auto frame_index : Range<int>(sprite.frame.size()))
			{
				auto &frame = sprite.frame[frame_index];
				if constexpr (get_label)
				{
					if (!frame.label.empty() && frame.label != label_name)
					{
						label_name = frame.label;
						check_and_rewrite_vaild_label(label_name); // only trash label will be fix.
						if (label_duplicate_system_stored.contains(label_name))
						{
							++label_duplicate_system_stored[label_name];
							label_name = fmt::format("{}_{}", label_name, label_duplicate_system_stored[label_name]);
						}
						else
						{
							label_duplicate_system_stored[label_name] = k_begin_index + 1_size;
						}
						package_library.label[label_name].start = frame_index;
					}
					++package_library.label[label_name].duration;
				}
				for (auto &remove_index : frame.remove)
				{
					model_structure[remove_index].state = State::state_false;
				}
				for (auto &append : frame.append)
				{
					model_structure[append.index] = Model{
						.state = State::state_null,
						.resource = append.resource,
						.sprite = append.sprite,
						.transform = k_initial_transform,
						.color = k_initial_color,
						.frame_start = frame_index,
						.frame_duration = frame_index};
					if (frame_index > k_begin_index)
					{
						frame_node_structure[append.index].emplace_back(
							FrameNode{
								.index = static_cast<int>(k_begin_index),
								.duration = frame_index});
					}
				}
				for (auto &change : frame.change)
				{
					auto &layer = model_structure[change.index];
					layer.state = State::state_true;
					exchange_transform_from_variant_to_standard(change.transform, layer.transform);
					if (!(change.color.at(0) == 0.0 && change.color.at(1) == 0.0 && change.color.at(2) == 0.0 && change.color.at(3) == 0.0))
					{
						layer.color = change.color;
					}
				}
				auto model_keys = Map::keys<int, Model>(model_structure);
				for (auto &layer_index : model_keys)
				{
					if constexpr (get_label)
					{
						auto &layer_index_list = package_library.label[label_name].layer_index_list;
						layer_index_list.emplace_back(layer_index);
					}
					auto &model_layer = model_structure[layer_index];
					auto &frame_node_list = frame_node_structure[layer_index];

					if (model_layer.state != State::state_null)
					{
						if (frame_node_list.size() > 0_size)
						{
							frame_node_list.back().duration = model_layer.frame_duration;
						}
					}
					if (model_layer.state == State::state_true)
					{
						auto frame_node = FrameNode{
							.index = frame_index,
							.duration = static_cast<int>(k_none_size),
							.resource = model_layer.resource,
							.sprite = model_layer.sprite,
							.transform = model_layer.transform,
							.color = model_layer.color};
						if (frame_node.sprite)
						{
							auto frame_size = definition.sprite[model_layer.resource].frame.size();
							frame_node.first_frame = (frame_index - model_layer.frame_start) % (frame_size == k_none_size ? 1_size : frame_size);
						}
						frame_node_list.emplace_back(frame_node);
						model_layer.state = State::state_null;
						model_layer.frame_duration = static_cast<int>(k_none_size);
					}
					if (model_layer.state == State::state_false)
					{
						model_structure.erase(layer_index);
					}
					++model_layer.frame_duration;
				}
			}
			for (auto &[layer_index, model_layer] : model_structure)
			{
				auto &frame_node_list = frame_node_structure[layer_index];
				frame_node_list.back().duration = model_layer.frame_duration;
			}
			return;
			/*
			if constexpr (get_label)
			{
				auto last_frame = k_begin_index_int;
				for (auto &[layer_index, model_layer] : model_structure)
				{
					auto &frame_node_list = frame_node_structure[layer_index];
					frame_node_list.back().duration = model_layer.frame_duration;
					if (auto current_frame_duration = static_cast<int>(frame_node_list.back().index + frame_node_list.back().duration); last_frame < current_frame_duration) {
						last_frame = current_frame_duration;
					}
				}
				return last_frame;
			}
			else
			{
				for (auto &[layer_index, model_layer] : model_structure)
				{
					auto &frame_node_list = frame_node_structure[layer_index];
					frame_node_list.back().duration = model_layer.frame_duration;
				}
				return 0;
			}
			*/
		}

		inline static auto exchange_label(
			FrameNodeStructure const &frame_node_structure,
			tsl::ordered_map<std::string, LabelInfo> const &label_info_structure,
			tsl::ordered_map<std::string, FrameNodeStructure> &label_structure) -> void
		{
			for (auto &[label_name, label_info] : label_info_structure)
			{
				auto &layer_index_list = label_info.layer_index_list;
				auto start_index = label_info.start;
				auto end_index = start_index + label_info.duration;
				auto label_layer_index = k_begin_index;
				auto last_frame_in_label = 0;
				for (auto &[layer_index, frame_node_list] : frame_node_structure)
				{
					if (std::find(layer_index_list.begin(), layer_index_list.end(), layer_index) == layer_index_list.end())
					{
						continue;
					}
					for (auto &frame_node : frame_node_list)
					{
						auto frame_index = frame_node.index;
						auto duration = frame_node.duration;
						if (frame_index >= end_index)
						{
							break;
						}
						if (frame_index + duration <= start_index)
						{
							continue;
						}
						auto new_frame_node = frame_node;
						if (frame_index + duration > end_index)
						{
							if (frame_index < end_index)
							{
								if (frame_index < start_index)
								{
									new_frame_node.index = start_index;
									new_frame_node.duration -= duration + start_index - end_index;
								}
								else
								{
									new_frame_node.duration -= duration + frame_index - end_index;
								}
							}
							new_frame_node.index -= start_index;
							label_structure[label_name][label_layer_index].emplace_back(new_frame_node);
							if (new_frame_node.index + new_frame_node.duration > last_frame_in_label)
							{
								last_frame_in_label = new_frame_node.index + new_frame_node.duration;
							}
						}
						else
						{
							if (frame_index < start_index)
							{
								new_frame_node.index = start_index;
								new_frame_node.duration -= start_index - frame_index;
							}
							new_frame_node.index -= start_index;
							label_structure[label_name][label_layer_index].emplace_back(new_frame_node);
							if (new_frame_node.index + new_frame_node.duration > last_frame_in_label)
							{
								last_frame_in_label = new_frame_node.index + new_frame_node.duration;
							}
						}
					}
					++label_layer_index;
				}
				if (static_cast<int>(last_frame_in_label) < label_info.duration)
				{
					auto frame_node = FrameNode{
						.index = static_cast<int>(k_begin_index),
						.duration = label_info.duration};
					label_structure[label_name][label_layer_index].emplace_back(frame_node);
				}
			}
			return;
		}

		inline static auto exchange_dom_document(
			SexyAnimation const &definition,
			AnimationNameList const &animation_name_list,
			FlashPackage &flash_package) -> void
		{
			auto &value = flash_package.document;
			auto dom_document = value.NewElement("DOMDocument");
			dom_document->SetAttribute("xmlns:xsi", k_xmlns_xsi_attribute.data());
			dom_document->SetAttribute("xmlns", k_xmlns_attribute.data());
			dom_document->SetAttribute("backgroundColor", "#999999");
			dom_document->SetAttribute("frameRate", std::to_string(definition.frame_rate).data());
			dom_document->SetAttribute("width", std::to_string(definition.size.width).data());
			dom_document->SetAttribute("height", std::to_string(definition.size.height).data());
			dom_document->SetAttribute("xflVersion", k_xfl_version.data());
			auto folders = value.NewElement("folders");
			if constexpr (split_label)
			{
				for (auto &folder_name : std::array<std::string_view, 4>{"media"_sv, "image"_sv, "sprite"_sv, "label"_sv})
				{
					auto dom_folder_item = value.NewElement("DOMFolderItem");
					dom_folder_item->SetAttribute("name", folder_name.data());
					dom_folder_item->SetAttribute("isExpanded", "true");
					folders->InsertEndChild(dom_folder_item);
				}
			}
			else
			{
				for (auto &folder_name : std::array<std::string_view, 3>{"media"_sv, "image"_sv, "sprite"_sv})
				{
					auto dom_folder_item = value.NewElement("DOMFolderItem");
					dom_folder_item->SetAttribute("name", folder_name.data());
					dom_folder_item->SetAttribute("isExpanded", "true");
					folders->InsertEndChild(dom_folder_item);
				}
			}
			dom_document->InsertEndChild(folders);
			auto media = value.NewElement("media");
			auto symbols = value.NewElement("symbols");
			for (auto image_index : Range(animation_name_list.image.size()))
			{
				auto &image = definition.image[image_index];
				auto dom_bitmap_item = value.NewElement("DOMBitmapItem");
				dom_bitmap_item->SetAttribute("name", fmt::format("media/{}", animation_name_list.image[image_index]).data());
				dom_bitmap_item->SetAttribute("href", fmt::format("media/{}.png", animation_name_list.image[image_index]).data());
				media->InsertEndChild(dom_bitmap_item);
				auto include = value.NewElement("Include");
				include->SetAttribute("href", fmt::format("image/{}.xml", animation_name_list.image[image_index]).data());
				symbols->InsertEndChild(include);
			}
			dom_document->InsertEndChild(media);
			for (auto &sprite_name : animation_name_list.sprite)
			{
				auto include = value.NewElement("Include");
				include->SetAttribute("href", fmt::format("sprite/{}.xml", sprite_name).data());
				symbols->InsertEndChild(include);
			}
			auto label_layer = value.NewElement("DOMLayer");
			label_layer->SetAttribute("name", "label");
			auto label_frames = value.NewElement("frames");
			if (flash_package.library.label.size() != k_none_size)
			{
				for (auto &[label_name, label_info] : flash_package.library.label)
				{
					auto include = value.NewElement("Include");
					include->SetAttribute("href", fmt::format("label/{}.xml", label_name).data());
					symbols->InsertEndChild(include);
					auto dom_frame = value.NewElement("DOMFrame");
					dom_frame->SetAttribute("index", std::to_string(label_info.start).data());
					dom_frame->SetAttribute("duration", std::to_string(label_info.duration).data());
					dom_frame->SetAttribute("name", label_name.data());
					dom_frame->SetAttribute("labelType", "name");
					auto elements = value.NewElement("elements");
					dom_frame->InsertEndChild(elements);
					label_frames->InsertEndChild(dom_frame);
				}
			}
			else
			{
				auto dom_frame = value.NewElement("DOMFrame");
				dom_frame->SetAttribute("index", std::to_string(k_begin_index).data());
				dom_frame->SetAttribute("duration", std::to_string(definition.main_sprite.frame.size()).data());
				auto elements = value.NewElement("elements");
				dom_frame->InsertEndChild(elements);
				label_frames->InsertEndChild(dom_frame);
			}
			label_layer->InsertEndChild(label_frames);
			auto action_layer = value.NewElement("DOMLayer");
			action_layer->SetAttribute("name", "action");
			auto action_frames = value.NewElement("frames");
			auto prev_end_index = static_cast<int>(k_begin_index);
			for (auto frame_index : Range<int>(definition.main_sprite.frame.size()))
			{
				auto &frame = definition.main_sprite.frame[frame_index];
				auto action_command_list = List<std::string>{};
				if (frame.stop || !frame.command.empty())
				{
					for (auto &command : frame.command)
					{
						action_command_list.emplace_back(fmt::format("fscommand(\"{}\", \"{}\");", command.command, command.argument));
					}
					if (frame.stop)
					{
						action_command_list.emplace_back("stop();");
					}
				}
				if (!frame.stop && static_cast<size_t>(frame_index) == definition.main_sprite.frame.size() - 1_size)
				{
					action_command_list.emplace_back("stop();");
				}
				if (!action_command_list.empty())
				{
					if (frame_index - prev_end_index > static_cast<int>(k_begin_index))
					{
						auto dom_frame = value.NewElement("DOMFrame");
						dom_frame->SetAttribute("index", std::to_string(prev_end_index).data());
						dom_frame->SetAttribute("duration", std::to_string(frame_index - prev_end_index).data());
						auto elements = value.NewElement("elements");
						dom_frame->InsertEndChild(elements);
						action_frames->InsertEndChild(dom_frame);
					}
					auto dom_frame = value.NewElement("DOMFrame");
					dom_frame->SetAttribute("index", std::to_string(frame_index).data());
					auto action_script = value.NewElement("Actionscript");
					auto script = value.NewElement("script");
					auto cdata_string = value.NewText(String::join(action_command_list, "\n"_sv).data());
					cdata_string->SetCData(true);
					script->InsertEndChild(cdata_string);
					auto elements = value.NewElement("elements");
					action_script->InsertEndChild(script);
					dom_frame->InsertEndChild(action_script);
					dom_frame->InsertEndChild(elements);
					action_frames->InsertEndChild(dom_frame);
					prev_end_index = frame_index + 1;
				}
			}
			action_layer->InsertEndChild(action_frames);
			auto instance_layer = value.NewElement("DOMLayer");
			instance_layer->SetAttribute("name", "instance");
			auto instance_frames = value.NewElement("frames");
			if constexpr (split_label)
			{
				for (auto &[label_name, label_info] : flash_package.library.label)
				{
					auto dom_frame = value.NewElement("DOMFrame");
					dom_frame->SetAttribute("index", std::to_string(label_info.start).data());
					dom_frame->SetAttribute("duration", std::to_string(label_info.duration).data());
					auto elements = value.NewElement("elements");
					auto dom_symbol_instance = value.NewElement("DOMSymbolInstance");
					dom_symbol_instance->SetAttribute("libraryItemName", fmt::format("label/{}", label_name).data());
					dom_symbol_instance->SetAttribute("symbolType", "graphic");
					dom_symbol_instance->SetAttribute("loop", "loop");
					elements->InsertEndChild(dom_symbol_instance);
					dom_frame->InsertEndChild(elements);
					instance_frames->InsertEndChild(dom_frame);
				}
			}
			else
			{
				auto include = value.NewElement("Include");
				include->SetAttribute("href", "main_sprite.xml");
				symbols->InsertEndChild(include);
				auto dom_frame = value.NewElement("DOMFrame");
				dom_frame->SetAttribute("index", std::to_string(k_begin_index).data());
				dom_frame->SetAttribute("duration", std::to_string(definition.main_sprite.frame.size()).data());
				auto elements = value.NewElement("elements");
				auto dom_symbol_instance = value.NewElement("DOMSymbolInstance");
				dom_symbol_instance->SetAttribute("libraryItemName", "main_sprite");
				dom_symbol_instance->SetAttribute("symbolType", "graphic");
				dom_symbol_instance->SetAttribute("loop", "loop");
				elements->InsertEndChild(dom_symbol_instance);
				dom_frame->InsertEndChild(elements);
				instance_frames->InsertEndChild(dom_frame);
			}
			dom_document->InsertEndChild(symbols);
			instance_layer->InsertEndChild(instance_frames);
			auto timelines = value.NewElement("timelines");
			auto dom_timeline = value.NewElement("DOMTimeline");
			dom_timeline->SetAttribute("name", "animation");
			auto layers = value.NewElement("layers");
			layers->InsertEndChild(label_layer);
			layers->InsertEndChild(action_layer);
			layers->InsertEndChild(instance_layer);
			dom_timeline->InsertEndChild(layers);
			timelines->InsertEndChild(dom_timeline);
			dom_document->InsertEndChild(timelines);
			value.InsertEndChild(dom_document);
			return;
		}

		inline static auto save_flash_package(
			FlashPackage &flash_package,
			std::string_view destination) -> void
		{
			write_xml(fmt::format("{}/DOMDocument.xml", destination), flash_package.document);
			for (auto &[image_name, image_document] : flash_package.library.image)
			{
				write_xml(fmt::format("{}/library/image/{}.xml", destination, image_name), image_document);
			}
			for (auto &[sprite_name, sprite_document] : flash_package.library.sprite)
			{
				write_xml(fmt::format("{}/library/sprite/{}.xml", destination, sprite_name), sprite_document);
			}
			if constexpr (split_label)
			{
				for (auto &[label_name, label_info] : flash_package.library.label)
				{
					write_xml(fmt::format("{}/library/label/{}.xml", destination, label_name), flash_package.library.label_document.at(label_name));
				}
			}
			else
			{
				write_xml(fmt::format("{}/library/main_sprite.xml", destination), flash_package.library.main_sprite);
			}
			return;
		}

		inline static auto save_xfl_content_file(
			std::string_view destination) -> void
		{
			write_text(fmt::format("{}/main.xfl", destination), k_xfl_content.data());
			return;
		}

		inline static auto save_media_directory(
			std::string_view destination) -> void
		{
			FileSystem::create_directory(fmt::format("{}/library/media", destination));
			return;
		}

		inline static auto exchange_flash(
			SexyAnimation const &definition,
			ExtraInfo &extra,
			AnimationNameList &animation_name_list,
			PackageLibrary &package_library) -> void
		{
			exchange_simple_extra(definition, extra);
			exchange_default_extra(extra);
			auto image_duplicate_stored = std::map<std::string, List<string>>{};
			for (auto &image : definition.image)
			{
				auto image_name = image.path;
				auto image_is_changed = false;
				if (std::find(image_name.begin(), image_name.end(), '/') != image_name.end())
				{
					auto string_list = String{image_name}.split("/"_sv);
					image_name = string_list.back();
					image_is_changed = true;
				}
				if (std::find(animation_name_list.image.begin(), animation_name_list.image.end(), image_name) != animation_name_list.image.end())
				{
					auto new_image_name = fmt::format("{}_{}", image_name, image_duplicate_stored[image_name].size() + 2_size);
					image_duplicate_stored[image_name].emplace_back(image_name);
					image_name = new_image_name;
					image_is_changed = true;
				}
				trim(image_name);
				animation_name_list.image.emplace_back(image_name);
				for (auto image_index : Range(definition.image.size()))
				{
					extra.image[image_name] = ImageInfo{
						.path = image_is_changed ? image.path : "",
						.id = image.id,
						.dimension = ImageDimension{
							.width = static_cast<int>(image.dimension.width),
							.height = static_cast<int>(image.dimension.height)}};
				}
			}
			for (auto image_index : Range(definition.image.size()))
			{
				auto &image = definition.image[image_index];
				auto &image_name = animation_name_list.image[image_index];
				exchange_image_document(image, image_name, package_library.image[image_name]);
			}
			for (auto &sprite : definition.sprite)
			{
				auto sprite_name = sprite.name;
				if (exchange_sprite_name_invalid<false>(sprite_name))
				{
					if (std::find(extra.sprite[sprite.name].begin(), extra.sprite[sprite.name].end(), sprite_name) == extra.sprite[sprite.name].end())
					{
						extra.sprite[sprite.name].emplace_back(sprite_name);
					}
				}
				if (std::find(animation_name_list.sprite.begin(), animation_name_list.sprite.end(), sprite_name) != animation_name_list.sprite.end())
				{
					auto new_sprite_name = fmt::format("{}_{}", sprite_name, extra.sprite[sprite.name].size() + 1_size);
					extra.sprite[sprite.name].emplace_back(new_sprite_name);
					animation_name_list.sprite.emplace_back(new_sprite_name);
				}
				else
				{
					animation_name_list.sprite.emplace_back(sprite_name);
				}
			}
			for (auto sprite_index : Range(definition.sprite.size()))
			{
				auto &sprite = definition.sprite[sprite_index];
				auto &sprite_name = animation_name_list.sprite[sprite_index];
				exchange_frame_node<false>(definition, sprite, package_library);
				exchange_sprite_document<SpriteType::sprite>(animation_name_list, sprite_name, package_library.frame_node, package_library.sprite[sprite_name]);
			}
			exchange_frame_node<true>(definition, definition.main_sprite, package_library);
			if constexpr (split_label)
			{
				auto label_frame_node = tsl::ordered_map<std::string, FrameNodeStructure>{};
				exchange_label(package_library.frame_node, package_library.label, label_frame_node);
				for (auto &element : label_frame_node)
				{
					auto &label_name = element.first;
					exchange_sprite_document<SpriteType::label>(animation_name_list, label_name, label_frame_node[label_name], package_library.label_document[label_name]);
				}
			}
			else
			{
				/*
				if (auto main_sprite_last_frame = static_cast<int>(definition.main_sprite.frame.size()); main_sprite_last_frame > last_frame)
				{
					auto frame_node = FrameNode{.index = static_cast<int>(k_begin_index),
												.duration = static_cast<int>(main_sprite_last_frame)};
					package_library.frame_node[package_library.frame_node.size()].emplace_back(frame_node);
				}
				*/
				exchange_sprite_document<SpriteType::main_sprite>(animation_name_list, "main_sprite", package_library.frame_node, package_library.main_sprite);
			}
			return;
		}

	public:
		inline static auto process_whole(
			SexyAnimation const &definition,
			ExtraInfo &extra,
			std::string_view destination) -> void
		{
			static_assert(split_label == true || split_label == false, "split_label must be true or false");
			auto flash_package = FlashPackage{};
			auto animation_name_list = AnimationNameList{};
			exchange_flash(definition, extra, animation_name_list, flash_package.library);
			exchange_dom_document(definition, animation_name_list, flash_package);
			save_flash_package(flash_package, destination);
			save_xfl_content_file(destination);
			save_media_directory(destination);
			return;
		}

		inline static auto process_fs(
			std::string_view source,
			std::string_view destination,
			int resolution) -> void
		{
			static_assert(split_label == true || split_label == false, "split_label must be true or false");
			auto definition = *FileSystem::read_json(source);
			auto extra = ExtraInfo{.resolution = resolution};
			process_whole(definition, extra, destination);
			write_json(fmt::format("{}/data.json", destination), extra);
			return;
		}
	};

	using ConvertToFlashWithMainSprite = ToFlash<false>;

	using ConvertToFlashWithLabel = ToFlash<true>;
}