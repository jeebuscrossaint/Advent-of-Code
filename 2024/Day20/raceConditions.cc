#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cmath>

std::vector<std::string> parseFile(const std::string& fileName) {
    std::ifstream file(fileName);
    std::vector<std::string> grid;
    std::string line;
    
    while (std::getline(file, line)) {
        grid.push_back(line);
    }
    
    return grid;
}

std::map<std::pair<int, int>, int> getDistances(const std::vector<std::string>& grid, const std::pair<int, int>& start, const std::pair<int, int>& end) {
    std::map<std::pair<int, int>, int> distances = {{start, 0}};
    std::pair<int, int> prev = {-1, -1}, current = start;
    
    std::vector<std::pair<int, int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    
    while (current != end) {
        for (const auto& dir : directions) {
            std::pair<int, int> next = {current.first + dir.first, current.second + dir.second};
            
            if (prev != next && grid[next.first][next.second] != '#') {
                distances[next] = distances[current] + 1;
                prev = current;
                current = next;
            }
        }
    }
    
    return distances;
}

int countValidShortcuts(const std::vector<std::string>& grid, int maxBypass) {
    int rows = grid.size();
    int columns = grid[0].size();
    std::map<char, std::pair<int, int>> lastSeenNodes;
    
    for (int row = 0; row < rows; ++row) {
        for (int column = 0; column < columns; ++column) {
            lastSeenNodes[grid[row][column]] = {row, column};
        }
    }

    std::pair<int, int> start = lastSeenNodes['S'];
    std::pair<int, int> end = lastSeenNodes['E'];
    
    auto distancesFromStart = getDistances(grid, start, end);
    auto distancesFromEnd = getDistances(grid, end, start);
    
    int shortcuts = 0;
    int threshold = 100;
    int maxLength = distancesFromEnd[start];
    
    for (const auto& [startCheat, startDist] : distancesFromStart) {
        for (const auto& [endCheat, endDist] : distancesFromEnd) {
            int manhattan = std::abs(startCheat.first - endCheat.first) + std::abs(startCheat.second - endCheat.second);
            int bypassedLength = startDist + endDist + manhattan;
            
            if (manhattan >= 2 && manhattan <= maxBypass && maxLength - bypassedLength >= threshold) {
                ++shortcuts;
            }
        }
    }
    
    return shortcuts;
}

int main() {
    auto grid = parseFile("input.txt");
    
    std::cout << "Part 1: " << countValidShortcuts(grid, 2) << std::endl;
    std::cout << "Part 2: " << countValidShortcuts(grid, 20) << std::endl;
    
    return 0;
}
