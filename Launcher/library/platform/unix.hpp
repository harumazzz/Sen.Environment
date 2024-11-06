#pragma once

#ifndef _WIN32
namespace Sen::Launcher {
	auto get_executable_path(
	) -> std::string {
		char path[PATH_MAX];
		auto count = readlink("/proc/self/exe", path, sizeof(path));
		if (count == -1) {
			throw std::runtime_error{ "Error retrieving executable path" };
			return "";
		}
		path[count] = '\0';
		auto execPath = std::string(path);
		auto pos = execPath.find_last_of("/\\");
		if (pos != std::string::npos) {
			execPath = execPath.substr(0, pos);
		}
		return execPath;
	}
}
#endif