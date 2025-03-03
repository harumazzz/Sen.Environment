// Copyright (c) 2024 Pyarelal Knowles, MIT License

#pragma once

#include <filesystem>
#include <stdexcept>
#include <string>

namespace decodeless {

namespace fs = std::filesystem;

class mapping_error : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
    void print() const { fprintf(stderr, "Error: %s\n", what()); }
};

class mapped_file_error : public fs::filesystem_error {
public:
    using fs::filesystem_error::filesystem_error;
};

} // namespace decodeless
