#pragma once
#ifndef AOCLI_HH
#define AOCLI_HH

#include <string>
#include <ctime>
#include <filesystem>
#include <curl/curl.h>
#include <fstream>
#include <gumbo.h>
#include <sstream>
#include <regex>
#include <iostream>

namespace fs = std::filesystem;

namespace term {
    const std::string reset = "\033[0m";
    const std::string bold = "\033[1m";
    const std::string dim = "\033[2m";
    const std::string italic = "\033[3m";
    const std::string underline = "\033[4m";
    const std::string yellow = "\033[33m";
    const std::string cyan = "\033[36m";
    const std::string white = "\033[37m";
    const std::string red = "\033[31m";    // Add this
    const std::string green = "\033[32m";  // Add this
}

struct Config {
        fs::path cache_dir;
        fs::path cookie_file;
        fs::path cookie_timestamp_file;
        fs::path inputs_dir;
        fs::path problems_dir;
        fs::path answers_dir;
};

Config initialize_config();
size_t writeCallback(void *contents, size_t size, size_t nmemb, std::string *userp);
std::string get_cookie(const Config& config);
std::string get_cached_input(const Config& config, int year, int day);
void cache_input(const Config& config, int year, int day, const std::string& input);
void update_cookie(const Config& config);
bool is_cookie_valid(const Config& config);
std::string viewProblem(int year, int day, const std::string& cookie);
std::string get_cached_problem(const Config& config, int year, int day);
void cache_problem(const Config& config, int year, int day, const std::string& problem);
void getCurrentYearAndDay(int &year, int &day);
bool isProblemAvailable(int year, int day);
std::string fetchAdventOfCodeInput(int year, int day, const std::string &cookie);
std::string formatText(const std::string& text, size_t width = 80);
std::string extractText(GumboNode* node);
enum class SubmitResult {
    CORRECT,
    INCORRECT,
    TOO_HIGH,
    TOO_LOW,
    RATE_LIMITED,
    ERROR
};

struct SubmitResponse {
    SubmitResult result;
    std::string message;
};
SubmitResponse submitAnswer(int year, int day, int part, const std::string& answer, const std::string& cookie);


#endif // AOCLI_HH
