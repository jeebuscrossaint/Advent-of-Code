#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

bool isSafeReport(const std::vector<int>& levels) {
    // Check if all increasing or all decreasing
    bool increasing = true, decreasing = true;

    for (size_t i = 1; i < levels.size(); ++i) {
        int diff = levels[i] - levels[i-1];

        // Check if adjacent levels differ by at most 3
        if (std::abs(diff) > 3) {
            return false;
        }

        // Update increasing/decreasing flags
        if (diff > 0) {
            decreasing = false;
        } else if (diff < 0) {
            increasing = false;
        } else {
            // If difference is zero, it breaks both increasing and decreasing
            increasing = false;
            decreasing = false;
        }

        // If neither increasing nor decreasing, report is unsafe
        if (!increasing && !decreasing) {
            return false;
        }
    }

    return true;
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

        // Parse levels from the line
        while (iss >> level) {
            levels.push_back(level);
        }

        // Check if report is safe
        if (isSafeReport(levels)) {
            safeReportCount++;
        }
    }

    return safeReportCount;
}

int main() {
    std::string inputFile = "input.txt";
    int result = countSafeReports(inputFile);

    if (result != -1) {
        std::cout << "Number of safe reports: " << result << std::endl;
    }

    return 0;
}
