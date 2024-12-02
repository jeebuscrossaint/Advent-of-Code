#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

// Define isSafeReport first
bool isSafeReport(const std::vector<int>& levels) {
    bool increasing = true, decreasing = true;

    for (size_t i = 1; i < levels.size(); ++i) {
        int diff = levels[i] - levels[i-1];

        if (std::abs(diff) > 3) {
            return false;
        }

        if (diff > 0) {
            decreasing = false;
        } else if (diff < 0) {
            increasing = false;
        } else {
            increasing = false;
            decreasing = false;
        }

        if (!increasing && !decreasing) {
            return false;
        }
    }

    return true;
}

bool isSafeReportWithDampener(const std::vector<int>& levels) {
    // First, check if the original report is safe
    if (isSafeReport(levels)) {
        return true;
    }

    // Try removing each level and check if the result becomes safe
    for (size_t remove = 0; remove < levels.size(); ++remove) {
        std::vector<int> modifiedLevels;

        // Create a modified list without the current level
        for (size_t i = 0; i < levels.size(); ++i) {
            if (i != remove) {
                modifiedLevels.push_back(levels[i]);
            }
        }

        // Check if the modified list is safe
        if (isSafeReport(modifiedLevels)) {
            return true;
        }
    }

    return false;
}

int countSafeReports(const std::string& filename) {
    std::ifstream inputFile(filename);
    if (!inputFile) {
        std::cerr << "Unable to open file" << std::endl;
        return -1;
    }

    int safeReportCount = 0;
    std::string line;

    while (std::getline(inputFile, line)) {
        std::vector<int> levels;
        std::istringstream iss(line);
        int level;

        while (iss >> level) {
            levels.push_back(level);
        }

        // Use the new function with Problem Dampener
        if (isSafeReportWithDampener(levels)) {
            safeReportCount++;
        }
    }

    return safeReportCount;
}

int main() {
    std::string inputFile = "input.txt";
    int result = countSafeReports(inputFile);

    if (result != -1) {
        std::cout << "Number of safe reports with Problem Dampener: " << result << std::endl;
    }

    return 0;
}
