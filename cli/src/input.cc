#include "aocli.hh"
#include <iostream>
#include <iterator>
#include <string>
#include <fstream>

std::string get_cached_input(const Config& config, int year, int day) {
        fs::path input_file = config.inputs_dir / (std::to_string(year) + "_" + std::to_string(day) + "txt");
        if (fs::exists(input_file)) {
                std::ifstream file(input_file);
                std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

                return content;
        }
        return "";
}

void cache_input(const Config& config, int year, int day, const std::string& input) {
        fs::path input_file = config.inputs_dir / (std::to_string(year) + "_" + std::to_string(day) + ".text");
        std::ofstream file(input_file);
        file << input;
}
