#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include <sstream>
#include <queue>

class Solution {
private:
    std::fstream fin;
    std::vector<std::pair<int, int>> data;

    void parse() {
        std::string line;
        while (std::getline(fin, line)) {
            std::istringstream iss(line);
            int x, y;
            char delimiter;
            if (iss >> x >> delimiter >> y) {
                data.emplace_back(x, y);
            }
        }
    }

    int bfs(int sx, int sy, int ex, int ey, int t) const { // Marked as const
        const std::pair<int, int> start = {sx, sy};
        const std::pair<int, int> end = {ex, ey};

        const std::vector<std::pair<int, int>> directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
        std::vector<std::vector<int>> grid(ex + 1, std::vector<int>(ey + 1, 0));
        std::vector<std::vector<int>> visited(ex + 1, std::vector<int>(ey + 1, 0));
        std::vector<std::vector<int>> dist(ex + 1, std::vector<int>(ey + 1, 0));

        for (int i = 0; i <= t; ++i) {
            grid[data[i].first][data[i].second] = 1;
        }

        std::queue<std::pair<std::pair<int, int>, int>> q;
        q.push({start, 0});
        visited[start.first][start.second] = 1;

        while (!q.empty()) {
            const auto [curr, d] = q.front();
            q.pop();

            if (curr == end) {
                return d;
            }

            for (const auto& dir : directions) {
                int x = curr.first + dir.first;
                int y = curr.second + dir.second;

                if (x < 0 || x > ex || y < 0 || y > ey || visited[x][y] || grid[x][y]) {
                    continue;
                }

                visited[x][y] = 1;
                dist[x][y] = d + 1;
                q.push({{x, y}, d + 1});
            }
        }

        return visited[end.first][end.second] ? dist[end.first][end.second] : -1;
    }

public:
    explicit Solution(const std::string& fileName) {
        fin.open(fileName, std::ios::in);
        if (!fin.is_open()) {
            throw std::runtime_error("Could not open file: " + fileName);
        }
        parse();
        fin.close();
    }

    int part1(int sx, int sy, int ex, int ey, int t) const {
        return bfs(sx, sy, ex, ey, t - 1);
    }

    std::string part2(int sx, int sy, int ex, int ey, int t) const {
        for (int i = t; i < static_cast<int>(data.size()); ++i) {
            int d = bfs(sx, sy, ex, ey, i);
            if (d == -1) {
                return std::to_string(data[i].first) + "," + std::to_string(data[i].second);
            }
        }
        return "No solution";
    }
};

int main() {
    try {
        Solution aoc("input.txt");

        std::cout << "Part 1: " << aoc.part1(0, 0, 70, 70, 1024) << '\n';
        std::cout << "Part 2: " << aoc.part2(0, 0, 70, 70, 1024) << '\n';
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }

    return 0;
}
