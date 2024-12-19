#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include <tuple>
#include <map>
#include <queue>
#include <set>
#include <unordered_map>
#include <cstdint>
#include <limits>

using namespace std;

class Solution {
private:
    ifstream fin;
    vector<string> grid;
    const vector<pair<int, int>> directions{{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    pair<int, int> start;
    pair<int, int> end;
    map<tuple<int, int, int>, int> dist;
    map<tuple<int, int, int>, vector<tuple<int, int, int>>> predecessors;

    void parseGrid() {
        string line;
        int row = 0;

        while (getline(fin, line)) {
            grid.push_back(line);
            for (int col = 0; col < static_cast<int>(line.size()); ++col) {
                if (line[col] == 'S') start = {row, col};
                if (line[col] == 'E') end = {row, col};
            }
            ++row;
        }
    }

    uint64_t dijkstra() {
        using State = tuple<int, int, int, int>; // distance, row, col, direction
        priority_queue<State, vector<State>, greater<>> pq;

        pq.push({0, start.first, start.second, 0});
        dist[{start.first, start.second, 0}] = 0;

        while (!pq.empty()) {
            auto [d, r, c, dir] = pq.top();
            pq.pop();

            if (grid[r][c] == 'E') return d;

            for (int i = 0; i < 4; ++i) {
                int nr = r + directions[i].first;
                int nc = c + directions[i].second;
                int nd = d + 1 + (i != dir ? 1000 : 0);

                if (nr < 0 || nc < 0 || nr >= static_cast<int>(grid.size()) || nc >= static_cast<int>(grid[0].size()) || grid[nr][nc] == '#')
                    continue;

                auto current = make_tuple(nr, nc, i);
                if (dist.find(current) == dist.end() || nd < dist[current]) {
                    dist[current] = nd;
                    pq.push({nd, nr, nc, i});
                    predecessors[current] = {{r, c, dir}};
                } else if (nd == dist[current]) {
                    predecessors[current].push_back({r, c, dir});
                }
            }
        }

        return numeric_limits<uint64_t>::max();
    }

    uint64_t backtrack() {
        set<pair<int, int>> uniqueTiles;
        queue<tuple<int, int, int>> q;

        for (int i = 0; i < 4; ++i) q.push({end.first, end.second, i});

        while (!q.empty()) {
            auto [r, c, dir] = q.front();
            q.pop();

            uniqueTiles.emplace(r, c);

            for (const auto& pred : predecessors[{r, c, dir}]) {
                q.push(pred);
            }
        }

        return uniqueTiles.size();
    }

public:
    explicit Solution(const string& fileName) {
        fin.open(fileName);
        if (!fin) throw runtime_error("Failed to open file: " + fileName);
        parseGrid();
    }

    uint64_t part1() {
        return dijkstra();
    }

    uint64_t part2() {
        dijkstra();
        return backtrack();
    }
};

int main() {
    try {
        Solution aoc("input.txt");
        //Solution test1("test1.txt");
        //Solution test2("test2.txt");

        //cout << "Part 1 test: " << test1.part1() << endl;
        cout << "Part 1: " << aoc.part1() << endl;
        cout << "--------------------------" << endl;
        //cout << "Part 2 test: " << test2.part2() << endl;
        cout << "Part 2: " << aoc.part2() << endl;

    } catch (const exception& ex) {
        cerr << "Error: " << ex.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
