#pragma once
#ifndef AOCCLI_HH
#define AOCCLI_HH

#include <iostream>
#include <curl/curl.h>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <ctime>
#include <sstream>

class AoCCLI {
        private:
        // Configuration
        const std::string BASE_URL = "https://adventofcode.com";
        std::string session_token;
        int current_year;
        int current_day;

        // Cache management
        std::string cache_directory;

        // Command handlers
        std::map<std::string, std::function<void(const std::vector<std::string>&)>> commands;


        public:
        // Constructor
        AoCCLI();

        // Core functioanlity
        void viewProblem(int day = 0, int year = 0);
        void getInput(int day = 0, int year = 0);
        void submitAnswer(const std::string& answer, int part, int day = 0, int year = 0);

        // additional functionality
        void setSession(const std:string& token);
        void setYear(int year);
        void setDay(int day);

        // Cache management
        void clearCache();
        void setCacheDirectory(const std::string& dir);

        // statistics and tracking
        void showStats();
        void showProgress();

        // testing
        void runtests(int day, int year);
        void benchmark(int day, int year);

        // helper functions
        std::string fetchFromUrl(const std::string & url);
        void saveToCache(const std::string& filename);
        bool isCached(const std::string& filename);

        // cli interface
        void run();
        void processCommand(const std::string& command);
        void help();

        class AoCException : public std::exception {
                private:
                std::string message;
                public:
                AoCException(const std::string& msg) : message(msg) {}
                const char* what() const noexcept override {
                        return message.c_str();
                }
        };
};

#endif // AOCCLI_HH
