#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include <utility>
#include <cstdint>

using namespace std;

class Solution {
private:
    fstream fin;
    vector<string> grid;
    map<pair<int, int>, bool> visited;

    void parse() {
        string line;
        while (getline(fin, line)) {
            grid.push_back(line);
        }
    }

    void dfs(int r, int c, map<pair<int, int>, int>& area, int a, char ch) {
        if (r < 0 || r >= static_cast<int>(grid.size()) || c < 0 || c >= static_cast<int>(grid[r].size())) return;
        if (visited[{r, c}] || grid[r][c] != ch) return;

        visited[{r, c}] = true;
        area[{r, c}] = a;

        dfs(r + 1, c, area, a, ch);
        dfs(r - 1, c, area, a, ch);
        dfs(r, c + 1, area, a, ch);
        dfs(r, c - 1, area, a, ch);
    }

    int countCorners(int r, int c, const map<pair<int, int>, int>& area) const {
        int sol = 0;
        int curr = area.at({r, c});

        auto getArea = [&](int x, int y) -> int {
            return area.contains({x, y}) ? area.at({x, y}) : -1;
        };

        int top = getArea(r - 1, c);
        int bottom = getArea(r + 1, c);
        int left = getArea(r, c - 1);
        int right = getArea(r, c + 1);

        int top_left = getArea(r - 1, c - 1);
        int top_right = getArea(r - 1, c + 1);
        int bottom_left = getArea(r + 1, c - 1);
        int bottom_right = getArea(r + 1, c + 1);

        if (curr != top && curr != left) ++sol;
        if (curr != top && curr != right) ++sol;
        if (curr != bottom && curr != left) ++sol;
        if (curr != bottom && curr != right) ++sol;

        if (curr == right && curr == bottom && curr != bottom_right) ++sol;
        if (curr == left && curr == bottom && curr != bottom_left) ++sol;
        if (curr == right && curr == top && curr != top_right) ++sol;
        if (curr == left && curr == top && curr != top_left) ++sol;

        return sol;
    }

public:
    explicit Solution(const string& fileName) {
        fin.open(fileName, ios::in);
        if (!fin.is_open()) {
            cerr << "Error: Could not open file " << fileName << endl;
            exit(EXIT_FAILURE);
        }
        parse();
        fin.close();
    }

    uint64_t part1() {
        uint64_t sol = 0;
        map<pair<int, int>, int> area;
        map<int, pair<int, int>> sizes;

        int a = 1;
        for (int r = 0; r < static_cast<int>(grid.size()); ++r) {
            for (int c = 0; c < static_cast<int>(grid[r].size()); ++c) {
                if (!visited[{r, c}]) {
                    dfs(r, c, area, a++, grid[r][c]);
                }
            }
        }

        for (int r = 0; r < static_cast<int>(grid.size()); ++r) {
            for (int c = 0; c < static_cast<int>(grid[r].size()); ++c) {
                sizes[area[{r, c}]].first++;

                for (int j = 0; j < 4; ++j) {
                    int nr = r + "0121"[j] - '1';
                    int nc = c + "1210"[j] - '1';

                    if (nr < 0 || nr >= static_cast<int>(grid.size()) || nc < 0 || nc >= static_cast<int>(grid[r].size())) {
                        sizes[area[{r, c}]].second++;
                    } else if (area[{nr, nc}] != area[{r, c}]) {
                        sizes[area[{r, c}]].second++;
                    }
                }
            }
        }

        for (const auto& [k, v] : sizes) {
            sol += static_cast<uint64_t>(v.first) * v.second;
        }

        return sol;
    }

    uint64_t part2() {
        visited.clear();

        uint64_t sol = 0;
        map<pair<int, int>, int> area;
        map<int, pair<int, int>> sizes;

        int a = 1;
        for (int r = 0; r < static_cast<int>(grid.size()); ++r) {
            for (int c = 0; c < static_cast<int>(grid[r].size()); ++c) {
                if (!visited[{r, c}]) {
                    dfs(r, c, area, a++, grid[r][c]);
                }
            }
        }

        for (int r = 0; r < static_cast<int>(grid.size()); ++r) {
            for (int c = 0; c < static_cast<int>(grid[r].size()); ++c) {
                sizes[area[{r, c}]].first++;
                sizes[area[{r, c}]].second += countCorners(r, c, area);
            }
        }

        for (const auto& [k, v] : sizes) {
            sol += static_cast<uint64_t>(v.first) * v.second;
        }

        return sol;
    }
};

int main() {
    Solution aoc("input.txt");
    cout << "Part 1: " << aoc.part1() << endl;
    cout << "--------------------------" << endl;
    cout << "Part 2: " << aoc.part2() << endl;

    return 0;
}
