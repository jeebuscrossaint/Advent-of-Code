#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

int main() {
    std::ifstream file("input.txt");
    if (!file) {
        std::cerr << "Unable to open file input.txt";
        return 1;
    }

    int totalSum = 0;
    std::string line;
    while (std::getline(file, line)) {
        line.erase(std::remove_if(line.begin(), line.end(), [](char c) { return !std::isdigit(c); }), line.end());
        int num;
        if (line.length() == 1) {
            num = std::stoi(line + line);
        } else {
            num = std::stoi(std::string() + line.front() + line.back());
        }
        totalSum += num;
    }

    file.close();

    std::cout << "Total sum: " << totalSum << std::endl;

    return 0;
}