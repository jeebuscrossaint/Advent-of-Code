#include "aocli.hh"
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <sys/stat.h>
#include <chrono>
#include <ctime>
#include <algorithm>

namespace fs = std::filesystem;

Config initialize_config() {
        Config config;
        // setup a cache directory structure
        config.cache_dir = fs::path(std::getenv("HOME")) / ".cache" / "aocli";
        config.cookie_file = config.cache_dir / "cookie";
        config.cookie_timestamp_file = config.cache_dir / "cookie_timestamp";
        config.inputs_dir = config.cache_dir / "inputs";
        config.problems_dir = config.cache_dir / "problems";
        config.answers_dir = config.cache_dir / "answers";

        // create tehse directorys if they dont exist
        //fs::create_directories(config.cache_dir);
        fs::create_directories(config.inputs_dir);
        fs::create_directories(config.problems_dir);
        fs::create_directories(config.answers_dir);

        return config;
}

bool is_cookie_valid(const Config& config) {
        if (!fs::exists(config.cookie_file) || !fs::exists(config.cookie_timestamp_file)) {
                return false;
        }

        std::ifstream timestamp_file(config.cookie_timestamp_file);
        std::time_t stored_time;
        timestamp_file >> stored_time;

        std::time_t current_time = std::time(nullptr);
        constexpr std::time_t thirty_days = 30 * 24 * 60 * 60; // 30 days in seconds lol

        return (current_time - stored_time) < thirty_days;
}

void store_cookie(const Config& config, const std::string& cookie) {
    // Store cookie without newline
    std::ofstream cookie_file(config.cookie_file, std::ios::binary);
    cookie_file.write(cookie.c_str(), cookie.length());
    cookie_file.close();

    // Store timestamp without newline
    std::ofstream timestamp_file(config.cookie_timestamp_file, std::ios::binary);
    std::string timestamp = std::to_string(std::time(nullptr));
    timestamp_file.write(timestamp.c_str(), timestamp.length());
    timestamp_file.close();

    chmod(config.cookie_file.c_str(), S_IRUSR | S_IWUSR);
    chmod(config.cookie_timestamp_file.c_str(), S_IRUSR | S_IWUSR);
}

std::string get_cookie(const Config& config) {
    if (!is_cookie_valid(config)) {
        std::cout << "Cookie not found or expired. Please enter your Advent of Code session cookie: ";
        std::string cookie;
        std::getline(std::cin, cookie);

        // Trim whitespace and newlines
        cookie.erase(std::remove_if(cookie.begin(), cookie.end(),
            [](unsigned char c) { return std::isspace(c); }), cookie.end());

        store_cookie(config, cookie);
        return cookie;
    }

    std::ifstream cookie_file(config.cookie_file, std::ios::binary);
    std::string cookie;
    std::getline(cookie_file, cookie);
    // Trim any potential whitespace
    cookie.erase(std::remove_if(cookie.begin(), cookie.end(),
        [](unsigned char c) { return std::isspace(c); }), cookie.end());
    return cookie;
}

// force a cookie update when needed
void update_cookie(const Config& config) {
    std::cout << "Please enter your new Advent of Code session cookie: ";
    std::string cookie;
    std::getline(std::cin, cookie);

    store_cookie(config, cookie);
}
