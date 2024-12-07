#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>

// split string by delimiter
std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(s);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// helper function to concatenate two numbers
long long concatenate(long long a, long long b) {
    long long b_digits = 0;
    long long temp = b;
    while (temp > 0) {
        temp /= 10;
        b_digits++;
    }
    return a * pow(10, b_digits) + b;
}

// recursion to evaluate all possible operator placements
bool evaluate(const std::vector<int>& numbers, int index, long long current_value, long long target) {
    if (index == numbers.size()) {
        return current_value == target;
    }

    // try addition
    if (evaluate(numbers, index + 1, current_value + numbers[index], target)) {
        return true;
    }

    // try multiplication
    if (evaluate(numbers, index + 1, current_value * numbers[index], target)) {
        return true;
    }

    // try concatenation
    if (evaluate(numbers, index + 1, concatenate(current_value, numbers[index]), target)) {
        return true;
    }

    return false;
}

// evaluatepart2 with additional bitwise OR operation
bool evaluatepart2(const std::vector<int>& numbers, int index, long long current_value, long long target) {
    if (index == numbers.size()) {
        if (current_value == target) {
            std::cout << " valid combo found " << current_value << "\n";
            return true;
        }
        return false;
    }

    // try addition
    if (evaluatepart2(numbers, index + 1, current_value + numbers[index], target)) {
        return true;
    }

    // try multiplication
    if (evaluatepart2(numbers, index + 1, current_value * numbers[index], target)) {
        return true;
    }

    // try concatenation
    if (evaluatepart2(numbers, index + 1, concatenate(current_value, numbers[index]), target)) {
        return true;
    }

    // try bitwise OR
    if (evaluatepart2(numbers, index + 1, current_value | numbers[index], target)) {
        return true;
    }

    return false;
}

long long part1(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    long long total_calibration = 0;

    while (std::getline(file, line)) {
        auto parts = split(line, ':');
        long long target = std::stoll(parts[0]);
        auto number_strings = split(parts[1], ' ');

        // convert strings to numbers
        std::vector<int> numbers;
        for (const auto& number_string : number_strings) {
            if (!number_string.empty()) {
                numbers.push_back(std::stoi(number_string));
            }
        }

        // check if the target value can be reached
        if (evaluate(numbers, 1, numbers[0], target)) {
            total_calibration += target;
        }
    }

    return total_calibration;
}

long long part2(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    long long total_calibration = 0;

    while (std::getline(file, line)) {
        auto parts = split(line, ':');
        long long target = std::stoll(parts[0]);
        auto number_strings = split(parts[1], ' ');

        // convert strings to numbers
        std::vector<int> numbers;
        for (const auto& number_string : number_strings) {
            if (!number_string.empty()) {
                numbers.push_back(std::stoi(number_string));
            }
        }

        // check if target value can be reached
        if (evaluatepart2(numbers, 1, numbers[0], target)) {
            total_calibration += target;
        }
    }

    return total_calibration;
}

int main() {
    auto part1sol = part1("input.txt");
    auto part2sol = part2("input.txt");
    std::cout << "Part 1 solution: " << part1sol << std::endl;
    std::cout << "Part 2 solution: " << part2sol << std::endl;
    return 0;
}
