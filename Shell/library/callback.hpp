#pragma once

#include "library/macro.hpp"
#include "library/color.hpp"
#include "bridge/executor.hpp"
#include "library/version.hpp"
#include "library/string.hpp"
#include "library/dialog.hpp"

namespace Sen::Shell {

	using String = String;

	using StringList = StringList;

	using Color = Color;
	
	inline auto input(

	) -> std::string
	{
		auto str = std::string{};
		#if WINDOWS
			auto state_b = BOOL{};
			auto handle = GetStdHandle(STD_INPUT_HANDLE);
			auto handle_mode = DWORD{};
			if (GetConsoleMode(handle, &handle_mode)) {
				auto text_16 = std::array<char16_t, 0x1000>{};
				auto length = DWORD{};
				state_b = ReadConsoleW(handle, text_16.data(), static_cast<DWORD>(text_16.size()), &length, nullptr);
				auto text_8 = u16_to_u8(std::u16string_view{text_16.data(), length - 2});
				str = std::move(reinterpret_cast<std::string &>(text_8));
			}
			else {
				std::getline(std::cin, str);
			}
		#else 
			std::getline(std::cin, str);
		#endif
		return str;
	}

	inline auto print_color(
		std::string_view title,
		Color const& color
	) -> void
	{
		switch (color) {
			case Color::RED:
				std::cout << "\033[31m" << title << "\033[0m" << '\n' << std::flush;
				break;
			case Color::GREEN:
				std::cout << "\033[32m" << title << "\033[0m" << '\n' << std::flush;
				break;
			case Color::CYAN: 
				std::cout << "\033[36m" << title << "\033[0m" << '\n' << std::flush;
				break;
			case Color::YELLOW: 
				std::cout << "\033[33m" << title << "\033[0m" << '\n' << std::flush;
				break;
			case Color::DEFAULT:
				std::cout << title << '\n' << std::flush;
				break;
			default: throw std::runtime_error("Unknown color");
		}
	}

	#if WINDOWS
	inline auto print_utf16(
		HANDLE & handle,
		std::string_view message
	) -> void
	{
		auto text = std::string{message.data(), message.size()} + '\n';
		auto text_16 = u8_to_u16(std::u8string_view{reinterpret_cast<const char8_t*>(text.data()), text.size()});
		auto state_b = WriteConsoleW(handle, text_16.data(), static_cast<DWORD>(text_16.size()), nullptr, nullptr);
		return;
	}
	#endif

	inline auto print(
		std::string_view title,
		std::string_view message = std::string{""},
		const Color &color = Color::DEFAULT
	) -> void
	{
		#if WINDOWS
			auto state_b = BOOL{};
			auto handle = GetStdHandle(STD_OUTPUT_HANDLE);
			auto handle_mode = DWORD{};
			SetConsoleTextAttribute(handle, static_cast<WORD>(color));
			auto state = GetConsoleMode(handle, &handle_mode);
			if (state) {
				print_utf16(handle, title);
			} 
			else {
				std::cout << title << '\n' << std::flush;
			}
		#else
			print_color(title, color);
		#endif
		#if WINDOWS
			SetConsoleTextAttribute(handle, static_cast<WORD>(Color::DEFAULT));
		#endif
			if (message != "") {
				# if WINDOWS
					if (state) {
						print_utf16(handle, message);
					}
					else {
						std::cout << message << '\n' << std::flush;
					}
				#else
					std::cout << message << '\n' << std::flush;
				#endif
			}
		return;
	}

	inline auto convert_color(
		std::string_view that
	) -> Color
	{
		switch (hash_string(that.data())) {
			case hash_string("red"):
				return Color::RED;
			case hash_string("green"):
				return Color::GREEN;
			case hash_string("cyan"):
				return Color::CYAN;
			case hash_string("yellow"):
				return Color::YELLOW;
			default:
				return Color::DEFAULT;
		}
	}
		

	inline auto to_vector(
		StringList* & that
	) -> std::vector<std::string>
	{
		auto destination = std::vector<std::string>{};
		destination.reserve(that->size);
		for (auto i = std::size_t{0}; i < that->size; ++i)
		{
			destination.emplace_back(std::string{ reinterpret_cast<char const*>(that->value[i].value), that->value[i].size });
		}
		return destination;
	}

	inline auto display_argument (
		std::vector<std::string> & result
	) -> void
	{
		assert_conditional(result.size() >= 2, "argument must be greater than 2");
		switch (result.size()) {
			case 2: {
				print(result[1]);
				break;
			}
			case 3: {
				print(result[1], result[2]);
				break;
			}
			case 4: {
				print(result[1], result[2], convert_color(result[3]));
				break;
			}
		}
		return;
	}

	inline auto input_argument(
		std::unique_ptr<uint8_t[]> & memory,
		String* destination
	) -> void
	{
		auto result = input();
		if (result.size() > 0) {
			memory.reset(new uint8_t[result.size() + 1]);
			std::memcpy(memory.get(), result.data(), result.size());
			destination->size = result.size();
			destination->value = memory.release();
		}
		else {
			memory.reset(new uint8_t[1]);
			memory.get()[0] = '\0';
			destination->size = 0;
			destination->value = memory.release();
		}
	}

	inline auto is_gui(
		std::unique_ptr<uint8_t[]> & memory,
		String* destination
	) -> void
	{
		memory.reset(new uint8_t[2]);
		memory.get()[0] = '0';
    	memory.get()[1] = '\0';
		destination->value = memory.release();
		destination->size = 1;
		return;
	}

	inline auto wait (

	) -> void
	{
		#if WINDOWS
			auto state_b = BOOL{};
			auto handle = GetStdHandle(STD_OUTPUT_HANDLE);
			auto handle_mode = DWORD{};
			SetConsoleTextAttribute(handle, static_cast<WORD>(Color::CYAN));
			auto state = GetConsoleMode(handle, &handle_mode);
			if (state) {
				auto text = std::string{"● "};
				auto text_16 = u8_to_u16(std::u8string_view{reinterpret_cast<const char8_t*>(text.data()), text.size()});
				state_b = WriteConsoleW(handle, text_16.data(), static_cast<DWORD>(text_16.size()), nullptr, nullptr);
			} 
			else {
				std::cout << "● " << std::flush;
			}
		#else
				std::cout << "\033[36m● \033[0m";
		#endif
	}

	inline auto clear(

	) -> void
	{
		#if WINDOWS
			auto hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, static_cast<WORD>(Color::DEFAULT));
		#endif
		return;
	}

	inline auto current_version(
		String* destination,
		std::unique_ptr<uint8_t[]> & memory
	) -> void
	{
		auto version = std::to_string(Shell::version);
		memory.reset(new uint8_t[version.size()]);
		std::memcpy(memory.get(), version.data(), version.size());
		destination->size = version.size();
		destination->value = memory.release();
	}

	inline auto pick_file (
		String* destination,
		std::unique_ptr<uint8_t[]> & memory
	) -> void
	{
		#if WINDOWS
			auto raw_selection = pick_path(false, false);
			if (!raw_selection.empty()) {
				memory.reset(new uint8_t[static_cast<std::size_t>(raw_selection[0].size() + 1)]);
				std::memcpy(memory.get(), raw_selection[0].data(), raw_selection[0].size());
				destination->size = raw_selection[0].size();
				destination->value = memory.release();
			}
		#elif LINUX || MACINTOSH
			auto raw_selection = tinyfd_openFileDialog("", nullptr, 0, nullptr, nullptr, false);
			if (raw_selection != nullptr) {
				memory.reset(new uint8_t[static_cast<std::size_t>(std::strlen(raw_selection) + 1)]);
				std::memcpy(memory.get(), raw_selection, strlen(raw_selection));
				destination->size = std::strlen(raw_selection);
				destination->value = memory.release();
			}
		#else
			throw std::runtime_error{ "invalid method" };
		#endif
	}

	inline auto pick_directory (
		String* destination,
		std::unique_ptr<uint8_t[]> & memory
	) -> void
	{
		#if WINDOWS
		auto raw_selection = pick_path(true, false);
		if (!raw_selection.empty()) {
			memory.reset(new uint8_t[static_cast<std::size_t>(raw_selection[0].size() + 1)]);
			std::memcpy(memory.get(), raw_selection[0].data(), raw_selection[0].size());
			destination->size = raw_selection[0].size();
			destination->value = memory.release();
		}
		#elif LINUX || MACINTOSH
		auto raw_selection = tinyfd_selectFolderDialog("", nullptr);
		if (raw_selection != nullptr) {
			memory.reset(new uint8_t[static_cast<std::size_t>(strlen(raw_selection) + 1)]);
			std::memcpy(memory.get(), raw_selection, strlen(raw_selection));
			destination->size = std::strlen(raw_selection);
			destination->value = memory.release();
		}
		#endif
	}

	inline auto callback(
		StringList* list,
		String* destination
	) -> int
	{
		auto result = to_vector(list);
		assert_conditional(result.size() >= 1, "argument must be greater than 1");
		auto memory = std::unique_ptr<uint8_t[]>(nullptr);
		switch (hash_string(result[0].data())) {
			case hash_string("display"):
				display_argument(result);
				break;
			case hash_string("input"):
				input_argument(memory, destination);
				break;
			case hash_string("is_gui"):
				is_gui(memory, destination);
				break;
			case hash_string("wait"):
				wait();
				break;
			case hash_string("clear"):
				clear();
				break;
			case hash_string("version"):
				current_version(destination, memory);
				break;
			case hash_string("pick_file"):
				pick_file(destination, memory);
				break;
			case hash_string("pick_directory"):
				pick_directory(destination, memory);
				break;
		};
		return 0;
	}

}