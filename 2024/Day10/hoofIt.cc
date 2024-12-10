#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <set>

struct Position {
        int row, col, height;
        Position(int r, int c, int h) : row(r), col(c), height(h) {}
};

// Directions: up, right, down, left
const int dr[] = {-1, 0, 1, 0};
const int dc[] = {0, 1, 0, -1};

long long countReachableNines(const std::vector<std::string>& grid, int startRow, int startCol) {
        int rows = grid.size();
        int cols = grid[0].size();
        std::set<std::pair<int, int>> reachableNines;

        std::queue<Position> q;
        std::set<std::pair<int, int>> visited;

        q.push(Position(startRow, startCol, 0));
        visited.insert({startRow, startCol});

        while (!q.empty()) {
                Position current = q.front();
                q.pop();

                // If we reached a 9 add it to our set of reachable nines
                if (grid[current.row][current.col] == '9') {
                        reachableNines.insert({current.row, current.col});
                        continue;
                }

                // Check all 4 directions
                for (int i = 0; i < 4; i++) {
                        int newRow = current.row + dr[i];
                        int newCol = current.col + dc[i];

                        if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols) {
                                int nextHeight = grid[newRow][newCol] - '0';
                                if (nextHeight == current.height + 1 && visited.find({newRow, newCol}) == visited.end()) {
                                        q.push(Position(newRow, newCol, nextHeight));
                                        visited.insert({newRow, newCol});
                                }
                        }
                }
        }

        return reachableNines.size();
}

long long part1(const std::string& filename) {
        std::ifstream file(filename);
        std::string line;
        std::vector<std::string> grid;

        while (std::getline(file, line)) {
                grid.push_back(line);
        }

        long long totalScore = 0;

        // find all trailheads (positions with height 0)
        for (int i = 0; i < grid.size(); i++) {
                for (int j = 0; j < grid[i].size(); j++) {
                        if (grid[i][j] == '0') {
                                totalScore += countReachableNines(grid, i, j);
                        }
                }
        }

        file.close();
        return totalScore;
}

long long countDistinctPaths(const std::vector<std::string>& grid, int startRow, int startCol,
                           std::vector<std::vector<long long>>& memo,
                           std::vector<std::vector<bool>>& visited) {
    int rows = grid.size();
    int cols = grid[0].size();

    // If we reached a 9, we found a valid path
    if (grid[startRow][startCol] == '9') {
        return 1;
    }

    // If we've already calculated this position, return memoized result
    if (memo[startRow][startCol] != -1) {
        return memo[startRow][startCol];
    }

    long long paths = 0;
    int currentHeight = grid[startRow][startCol] - '0';

    // Mark current position as visited
    visited[startRow][startCol] = true;

    // Check all four directions
    for (int i = 0; i < 4; i++) {
        int newRow = startRow + dr[i];
        int newCol = startCol + dc[i];

        if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols) {
            int nextHeight = grid[newRow][newCol] - '0';
            if (nextHeight == currentHeight + 1 && !visited[newRow][newCol]) {
                paths += countDistinctPaths(grid, newRow, newCol, memo, visited);
            }
        }
    }

    // Unmark current position before returning
    visited[startRow][startCol] = false;

    // Memoize and return result
    memo[startRow][startCol] = paths;
    return paths;
}

long long part2(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    std::vector<std::string> grid;

    while (std::getline(file, line)) {
        grid.push_back(line);
    }

    int rows = grid.size();
    int cols = grid[0].size();
    long long totalRating = 0;

    // For each trailhead (height 0)
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j] == '0') {
                // Initialize memoization and visited arrays for each trailhead
                std::vector<std::vector<long long>> memo(rows, std::vector<long long>(cols, -1));
                std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
                totalRating += countDistinctPaths(grid, i, j, memo, visited);
            }
        }
    }

    file.close();
    return totalRating;
}

int main() {
        std::cout << "Part 1: " << part1("input.txt") << std::endl;
        std::cout << "Part 2: " << part2("input.txt") << std::endl;
}
