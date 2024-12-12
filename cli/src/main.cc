#include <iostream>
#include <string>
#include <string_view>
#include "aocli.hh"

void help() {
    std::cout << "Usage: aocli <command> [options] [arguments]\n\n";

    std::cout << "Commands:\n";
    std::cout << "  fetch         Fetch puzzle input\n";
    std::cout << "                aocli fetch [day] [year]\n";
    std::cout << "                aocli fetch -f [day] [year]      (force refresh)\n\n";

    std::cout << "  view          View puzzle description\n";
    std::cout << "                aocli view [day] [year]\n";
    std::cout << "                aocli view -f [day] [year]       (force refresh)\n\n";

    std::cout << "  submit        Submit puzzle answer\n";
    std::cout << "                aocli submit <part> <answer> [day] [year]\n";
    std::cout << "                part: 1 or 2\n";
    std::cout << "                answer: your solution\n\n";

    std::cout << "  update-cookie Update session cookie\n";
    std::cout << "                aocli update-cookie\n\n";

    std::cout << "  cookie-status Check cookie validity\n";
    std::cout << "                aocli cookie-status\n\n";

    std::cout << "Options:\n";
    std::cout << "  -f, --refresh Force refresh cached content\n\n";

    std::cout << "Arguments:\n";
    std::cout << "  day           Puzzle day (1-25)\n";
    std::cout << "  year          Puzzle year (2015-present)\n";
    std::cout << "                If not provided, defaults to current day/year\n";
    std::cout << "                during December, or day 1 otherwise\n\n";

    std::cout << "Examples:\n";
    std::cout << "  aocli fetch                    Fetch today's input\n";
    std::cout << "  aocli fetch 1 2023             Fetch day 1, 2023 input\n";
    std::cout << "  aocli view -f 5 2022           View day 5, 2022 puzzle (force refresh)\n";
    std::cout << "  aocli submit 1 \"123\" 3 2023    Submit 123 as part 1 answer for day 3, 2023\n";
    std::cout << "  aocli update-cookie            Update session cookie\n";
}

int main(int argc, char **argv) {
    if (argc < 2) {
        help();
        return 1;
    }

    Config config = initialize_config();
    std::string cookie = get_cookie(config);
    std::string command = argv[1];

    // Handle force refresh flag
    bool forceRefresh = false;
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-f" || arg == "--refresh") {
            forceRefresh = true;
            // Shift remaining args left
            for (int j = i; j < argc - 1; j++) {
                argv[j] = argv[j + 1];
            }
            argc--;
            i--;
        }
    }

    try {
        // Special handling for submit command
        if (command == "submit") {
            if (argc < 4) {
                std::cerr << "Usage: aoc submit <part> <answer> [day] [year]" << std::endl;
                return 1;
            }

            int part = std::stoi(argv[2]);
            std::string answer = argv[3];

            // Get default year and day
            int day = 0, year = 0;
            getCurrentYearAndDay(year, day);

            // Override with provided day/year if any
            if (argc > 4) day = std::stoi(argv[4]);
            if (argc > 5) year = std::stoi(argv[5]);

            if (part != 1 && part != 2) {
                throw std::runtime_error("Part must be 1 or 2");
            }

            if (!isProblemAvailable(year, day)) {
                throw std::runtime_error("Problem not available yet (Year: " +
                    std::to_string(year) + ", Day: " +
                    std::to_string(day) + ")");
            }

            std::cout << term::bold << "Submitting answer for Year " << year
                     << " Day " << day << " Part " << part << "..."
                     << term::reset << std::endl;

            SubmitResponse response = submitAnswer(year, day, part, answer, cookie);

            std::cout << std::string(80, '=') << "\n";
            switch (response.result) {
                case SubmitResult::CORRECT:
                    std::cout << term::bold << term::green
                             << "✓ Correct Answer!" << term::reset << std::endl;
                    break;
                case SubmitResult::TOO_HIGH:
                    std::cout << term::bold << term::red
                             << "⚠ Too High!" << term::reset << std::endl;
                    break;
                case SubmitResult::TOO_LOW:
                    std::cout << term::bold << term::red
                             << "⚠ Too Low!" << term::reset << std::endl;
                    break;
                case SubmitResult::RATE_LIMITED:
                    std::cout << term::bold << term::yellow
                             << "⚠ Rate Limited!" << term::reset << std::endl;
                    break;
                case SubmitResult::INCORRECT:
                    std::cout << term::bold << term::red
                             << "✗ Incorrect Answer!" << term::reset << std::endl;
                    break;
                case SubmitResult::ERROR:
                    std::cout << term::bold << term::red
                             << "⚠ Error!" << term::reset << std::endl;
                    break;
            }
            std::cout << std::string(80, '-') << "\n";
            std::cout << formatText(response.message) << std::endl;
            std::cout << std::string(80, '=') << "\n";
            return 0;
        }

        // Handle other commands
        int day = 0, year = 0;
        getCurrentYearAndDay(year, day);

        // Override defaults if arguments provided
        if (argc > 2) day = std::stoi(argv[2]);
        if (argc > 3) year = std::stoi(argv[3]);

        // Validate the date for commands that need it
        if (command == "fetch" || command == "view") {
            if (!isProblemAvailable(year, day)) {
                throw std::runtime_error("Problem not available yet (Year: " +
                    std::to_string(year) + ", Day: " +
                    std::to_string(day) + ")");
            }
        }

        if (command == "fetch") {
            std::string input = get_cached_input(config, year, day);
            if (forceRefresh || input.empty()) {
                input = fetchAdventOfCodeInput(year, day, cookie);
                cache_input(config, year, day, input);
            }
            std::cout << input;
        }
        else if (command == "view") {
            std::string problem;
            if (!forceRefresh) {
                problem = get_cached_problem(config, year, day);
            }

            // If we have a cached version but it doesn't contain Part 2, try refreshing
            if (!problem.empty() && problem.find("--- Part Two ---") == std::string::npos) {
                std::string fresh_problem = viewProblem(year, day, cookie);
                if (fresh_problem.find("--- Part Two ---") != std::string::npos) {
                    problem = fresh_problem;
                    cache_problem(config, year, day, problem);
                }
            }

            // If no cached version exists or force refresh, fetch it
            if (problem.empty()) {
                problem = viewProblem(year, day, cookie);
                cache_problem(config, year, day, problem);
            }

            // Format the header
            std::cout << term::bold << term::yellow;
            std::cout << std::string(80, '=') << "\n";
            std::cout << "--- Day " << day << ": ";

            // Find and display the title
            size_t titleStart = problem.find(": ") + 2;
            size_t titleEnd = problem.find("\n", titleStart);
            if (titleStart != std::string::npos && titleEnd != std::string::npos) {
                std::cout << problem.substr(titleStart, titleEnd - titleStart);
            }
            std::cout << " ---\n";
            std::cout << std::string(80, '=') << term::reset << "\n\n";

            // Split and format parts
            size_t part2Start = problem.find("--- Part Two ---");
            if (part2Start != std::string::npos) {
                // Print Part 1
                std::cout << term::bold << term::cyan << "Part One:" << term::reset << "\n";
                std::cout << std::string(40, '-') << "\n";
                std::string part1 = problem.substr(0, part2Start);
                std::cout << formatText(part1) << "\n";

                // Print Part 2
                std::cout << term::bold << term::cyan << "Part Two:" << term::reset << "\n";
                std::cout << std::string(40, '-') << "\n";
                std::string part2 = problem.substr(part2Start + 14);
                std::cout << formatText(part2) << "\n";
            } else {
                // Only Part 1 available
                std::cout << term::bold << term::cyan << "Part One:" << term::reset << "\n";
                std::cout << std::string(40, '-') << "\n";
                std::cout << formatText(problem) << "\n";
            }
        }
        else if (command == "update-cookie") {
            update_cookie(config);
            std::cout << "Cookie updated successfully." << std::endl;
        }
        else if (command == "cookie-status") {
            if (is_cookie_valid(config)) {
                std::cout << "Cookie is valid." << std::endl;
            } else {
                std::cout << "Cookie is invalid or expired." << std::endl;
            }
        }
        else if (command == "version" || "-v") {
                std::cout << "aocli v1.0 using libcurl and gumbo-parser" << std::endl;
        }
        else {
            std::cerr << "Unknown command: " << command << std::endl;
            help();
            return 1;
        }
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
