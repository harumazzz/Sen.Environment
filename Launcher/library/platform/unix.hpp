#pragma once

#ifndef _WIN32
namespace Sen::Launcher {
	auto current_home(
	) -> std::string {
		auto path = std::array<char, PATH_MAX>{};
		auto count = readlink("/proc/self/exe", path.data(), path.size());
		if (count == -1) {
			throw std::runtime_error{ "Error retrieving executable path" };
		}
		path[count] = '\0';
		auto execPath = std::string{path.data(), path.size()};
		auto pos = execPath.find_last_of("/\\");
		if (pos != std::string::npos) {
			execPath = execPath.substr(0, pos);
		}
		return execPath;
	}
}
#endif