#include <bits/stdc++.h>
#include <fstream>

#define pp pair<int, int>
#define mp(x, y) make_pair(x, y)
#define ll long long
using namespace std;

int main() {
    vector<vector<char>> temp;
    vector<vector<char>> grid;

    ifstream infline("input.txt");

    string line;
    while (getline(infline, line)) {
        vector<char> row;
        bool isemptyrow = true;
        for (char c : line) {
            row.push_back(c);
            if (c == '#') isemptyrow = false;
        }

        temp.push_back(row);
        if (isemptyrow) temp.push_back(row);
    }

    int m = temp[0].size();
    int n = temp.size();

    grid.resize(n);

    for (int i = 0; i < m; i++) {
        bool isemptycol = true;
        for (int j = 0; j < n; j++) {
            if (temp[j][i] == '#') isemptycol = false;
            grid[j].push_back(temp[j][i]);
        }

        if (isemptycol) {
            for (auto &row : grid) {
                row.push_back('.');
            }
        }
    }

    vector<pp> galaxies;
    n = grid.size();
    m = grid[0].size();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (grid[i][j] == '#') {
                galaxies.push_back(mp(i, j));
            }
        }
    }

    ll ans = 0;
    for (int i = 0; i < galaxies.size(); i++) {
        for (int j = i + 1; j < galaxies.size(); j++) {
            int x1 = galaxies[i].first;
            int y1 = galaxies[i].second;
            int x2 = galaxies[j].first;
            int y2 = galaxies[j].second;

            ans += abs(x1 - x2) + abs(y1 - y2);
        }
    }

    cout << ans << endl;
}
