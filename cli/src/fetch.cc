#include <iostream>
#include <string>
#include <ctime>
#include <curl/curl.h>
#include "aocli.hh"

// callback function for libcurl to write the response into a string
size_t writeCallback(void *contents, size_t size, size_t nmemb, std::string *userp) {
        size_t totalSize = size * nmemb;
        userp->append((char *)contents, totalSize);
        return totalSize;
}

// function to fetch advent of code input
bool isValidPuzzleDate(int year, int day) {
    // Basic validation: day must be 1-25, year must be 2015 or later
    if (day < 1 || day > 25) return false;
    if (year < 2015) return false;
    return true;
}

std::string fetchAdventOfCodeInput(int year, int day, const std::string &cookie) {
    if (year == 0 || day == 0) {
        getCurrentYearAndDay(year, day);
    }

    if (!isValidPuzzleDate(year, day)) {
        throw std::runtime_error("Invalid puzzle date:\n"
                               "- Year must be 2015 or later\n"
                               "- Day must be between 1-25\n"
                               "Current request: Year " + std::to_string(year) +
                               ", Day " + std::to_string(day));
    }

    // prepare the url for fetching input
    std::string url = "https://adventofcode.com/" + std::to_string(year) + "/day/" + std::to_string(day) + "/input";
    std::string response;

    // initialize curl
    CURL *curl = curl_easy_init();
    if (!curl) {
        throw std::runtime_error("Failed to initialize curl");
    }

    try {
        // setup the request
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // set the cookie directly using CURLOPT_COOKIE
        std::string cookieStr = "session=" + cookie;
        curl_easy_setopt(curl, CURLOPT_COOKIE, cookieStr.c_str());

        // perform the request
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            throw std::runtime_error("Failed to fetch input: " + std::string(curl_easy_strerror(res)));
        }

        curl_easy_cleanup(curl);
        return response;
    }
    catch (...) {
        curl_easy_cleanup(curl);
        throw;
    }
}
