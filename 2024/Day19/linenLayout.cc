#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>

bool canCreateDesign(const std::string& design, const std::vector<std::string>& patterns) {
    // Base case: if design is empty, we've successfully used all parts
    if (design.empty()) return true;

    // Try each pattern at the start of the design
    for (const std::string& pattern : patterns) {
        // Check if the current pattern could be the start of the design
        if (design.length() >= pattern.length() &&
            design.substr(0, pattern.length()) == pattern) {
            // Recursively check if we can create the rest of the design
            if (canCreateDesign(design.substr(pattern.length()), patterns)) {
                return true;
            }
        }
    }

    return false;
}

long long countDesignWays(const std::string& design, const std::vector<std::string>& patterns,
                         std::unordered_map<std::string, long long>& memo) {
    // Check if we've already computed this design
    if (memo.count(design) > 0) {
        return memo[design];
    }

    // Base case: if design is empty, we found one valid combination
    if (design.empty()) return 1;

    // Count all possible ways
    long long total_ways = 0;

    // Try each pattern at the start of the design
    for (const std::string& pattern : patterns) {
        if (design.length() >= pattern.length() &&
            design.substr(0, pattern.length()) == pattern) {
            total_ways += countDesignWays(design.substr(pattern.length()),
                                        patterns, memo);
        }
    }

    // Store result in memo before returning
    memo[design] = total_ways;
    return total_ways;
}

int main() {
    std::ifstream file("input.txt");
    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    // First, read the patterns line
    std::string patterns_line;
    std::getline(file, patterns_line);

    // Vector to store the individual patterns
    std::vector<std::string> patterns;

    // Parse the patterns (split by comma and trim whitespace)
    std::stringstream ss(patterns_line);
    std::string pattern;
    while (std::getline(ss, pattern, ',')) {
        // Remove whitespace
        pattern.erase(remove_if(pattern.begin(), pattern.end(), isspace), pattern.end());
        patterns.push_back(pattern);
    }

    // Skip the empty line
    std::getline(file, pattern);

    // Now we'll read the designs to check
    std::vector<std::string> designs;
    while (std::getline(file, pattern)) {
        designs.push_back(pattern);
    }

    int possible_designs = 0;
        for (const std::string& design : designs) {
            if (canCreateDesign(design, patterns)) {
                possible_designs++;
            }
        }

        std::cout << "Possible designs: " << possible_designs << std::endl;

        std::unordered_map<std::string, long long> memo;

           long long total_combinations = 0;
           for (const std::string& design : designs) {
               total_combinations += countDesignWays(design, patterns, memo);
        }

        std::cout << "Total number of ways: " << total_combinations << std::endl;

        return 0;
}
