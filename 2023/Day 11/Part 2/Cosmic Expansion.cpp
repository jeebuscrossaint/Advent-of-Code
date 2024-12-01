#include <bits/stdc++.h>
#include <fstream>

#define pp pair<int, int>
#define mp(x, y) make_pair(x, y)
#define ll long long
using namespace std;

int main() {
    ifstream infline("input.txt");
    ofstream out("output.txt");

    vector<vector<char>> temp;
    vector<vector<char>> grid;
    vector<int> prerow, precol;

    string line;
    while (getline(infline, line)) {
        vector<char> row;
        bool isemptyrow = true;
        for (char c : line) {
            row.push_back(c);
            if (c == '#') isemptyrow = false;
        }

        temp.push_back(row);
        if (isemptyrow) {
            row.assign(line.size(), '!');
            temp.push_back(row);
        }
    }

    int m = temp[0].size();
    int n = temp.size();

    grid.resize(n);
    prerow.resize(n, 0);
    precol.resize(m, 0);

    for (int i = 0; i < m; i++) {
        bool isemptycol = true;
        for (int j = 0; j < n; j++) {
            if (temp[j][i] == '#') isemptycol = false;
            grid[j].push_back(temp[j][i]);
        }

        if (isemptycol) {
            for (auto &row : grid) {
                row.push_back('!');
            }
        }
    }

    for (int i = 1; i < n; i++) {
        bool expandedrow = all_of(grid[i].begin(), grid[i].end(), [](char c) { return c == '!'; });
        prerow[i] = expandedrow ? prerow[i - 1] + 1 : prerow[i - 1];
    }

    for (int j = 1; j < m; j++) {
        bool expandedcol = all_of(grid.begin(), grid.end(), [j](const vector<char> &row) { return row[j] == '!'; });
        precol[j] = expandedcol ? precol[j - 1] + 1 : precol[j - 1];
    }

    for (const auto &row : grid) {
        copy(row.begin(), row.end(), ostream_iterator<char>(out));
        out << endl;
    }
    out << endl;

    copy(prerow.begin(), prerow.end(), ostream_iterator<int>(out, " "));
    out << endl;
    copy(precol.begin(), precol.end(), ostream_iterator<int>(out, " "));

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

            ll rowwrap = abs(prerow[x2] - prerow[x1]);
            ll colwrap = abs(precol[y2] - precol[y1]);

            ans += abs(x2 - x1) + abs(y2 - y1) + rowwrap * 999998 + colwrap * 999998;
        }
    }

    cout << ans << endl;
}
