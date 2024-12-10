#include "aocli.hh"
#include <string>
#include <iostream>
#include <filesystem>

// callback function for curl to write response data

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
}

AoCCLI::AoCCLI() {
        // get current date
        time_t now = time(0);
        tm* ltm = localtime(&now);
        current_year = 1900 + ltm->tm_year;
        current_day = ltm->tm_mday;

        // set default cache directory
        cache_directory = "./cache";

        // initialize curl
        curl_global_init(CURL_GLOBAL_DEFAULT);
}

void AoCCLI::getInput(int day, int year) {
        // Use current day/year if not specified
        if (day == 0) day = current_day;
        if (year == 0) year = current_year;

        // validate day and year
        if (day < 1 || day > 25) {
                throw AoCException("Invalid day: " + std::to_string(day));
        }
        if (year < 2015 || year > current_year) {
                throw AoCException("Invalid year: " + std::to_string(year));
        }

        // check if input is already cahced
        std::string cache_filename = cache_directory + "/" + std::to_string(year) + "_" + std::to_string(day) + "_input.txt";

        if (isCached(cache_filename)) {
                std::ifstream cache_file(cache_filename);
                std::string input((std::istreambuf_iterator<char>(cache_file)),
                        std::istreambuf_iterator<char>());
                std::cout << input;
                return;
        }

        // if not cached, fetch from AoC website
        if (session_token.empty()) {
                throw AoCException("Session token not set. Use 'set-session' command to set the session token.");
        }

        std::string url = BASE_URL + "/" + std::to_string(year) +
                             "/day/" + std::to_string(day) + "/input";

           CURL* curl = curl_easy_init();
           std::string response;

           if (curl) {
               struct curl_slist* headers = NULL;
               std::string cookie = "session=" + session_token;
               headers = curl_slist_append(headers, cookie.c_str());

               curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
               curl_easy_setopt(curl, CURLOPT_COOKIE, cookie.c_str());
               curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
               curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

               CURLcode res = curl_easy_perform(curl);

               if (res != CURLE_OK) {
                   throw AoCException("Failed to fetch input: " +
                                    std::string(curl_easy_strerror(res)));
               }

               curl_easy_cleanup(curl);
           }

           // Save to cache
           saveToCache(response, cache_filename);

           // Output the input
           std::cout << response;
}

void AoCCLI::saveToCache(const std::string& content, const std::string& filename) {
    // Create cache directory if it doesn't exist
    std::filesystem::create_directories(cache_directory);

    std::ofstream cache_file(filename);
    if (!cache_file) {
        throw AoCException("Failed to create cache file: " + filename);
    }
    cache_file << content;
}

bool AoCCLI::isCached(const std::string& filename) {
    return std::filesystem::exists(filename);
}

void AoCCLI::setSession(const std::string& token) {
    session_token = token;
}
