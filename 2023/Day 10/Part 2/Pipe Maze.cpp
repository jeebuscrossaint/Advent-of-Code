#include <bits/stdc++.h>
using namespace std;

vector<string> readLines(string fileName = "input.txt") {
    vector<string> result;
    string line;
    ifstream file(fileName);
    if (file.is_open()) {
        while (getline(file, line)) {
            result.push_back(line);
        }
        file.close();
    }
    return result;
}

vector<string> splitString(string s, char delimiter = ' ') {
    vector<string> result;
    string current = "";
    for (char ch : s) {
        if (ch == delimiter) {
            if (!current.empty())
                result.push_back(current);
            current = "";
        } else {
            current += ch;
        }
    }
    if (!current.empty()) {
        result.push_back(current);
    }
    return result;
}

string trimString(string s, char character = ' ') {
    int lo = -1, hi = -1;
    for (int i = 0; i < (int)s.size(); i++) {
        if (s[i] != character) {
            lo = i;
            break;
        }
    }
    for (int i = (int)s.size() - 1; i >= 0; i--) {
        if (s[i] != character) {
            hi = i;
            break;
        }
    }
    if (lo != -1 and hi != -1) {
        string result = "";
        for (int i = lo; i <= hi; i++)
            result += s[i];
        return result;
    }
    return "";
}

int64_t silver(vector<string> lines) {
    int64_t answer = 0;
    vector<string> a = lines;
    const int n = (int)a.size(), m = (int)a[0].size();

    pair<int, int> source = {-1, -1};

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (a[i][j] == 'S') {
                source = {i, j};
                break;
            }
        }
        if (source != make_pair(-1, -1))
            break;
    }

    auto isInRange = [&](int x, int y) -> bool {
        return 0 <= x and x < n and 0 <= y and y < m;
    };

    auto possibleMoves = [&](int x, int y) -> vector<pair<int, int>> {
        char c = a[x][y];
        if (c == 'S') {
            return {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
        } else if (c == 'F') {
            return {{0, 1}, {1, 0}};
        } else if (c == 'L') {
            return {{0, 1}, {-1, 0}};
        } else if (c == 'J') {
            return {{0, -1}, {-1, 0}};
        } else if (c == '7') {
            return {{0, -1}, {1, 0}};
        } else if (c == '-') {
            return {{0, -1}, {0, 1}};
        } else if (c == '|') {
            return {{1, 0}, {-1, 0}};
        }
        return {};
    };

    auto isPossibleMove = [&](int x, int y, int dx, int dy) -> bool {
        if (!isInRange(x + dx, y + dy))
            return false;
        char c = a[x + dx][y + dy];
        string allowedChars = "FLJ7-|";
        bool validChar = false;
        for (char ch : allowedChars) {
            if (ch == c)
                validChar = true;
        }
        if (validChar) {
            for (auto &[rdx, rdy] : possibleMoves(x + dx, y + dy)) {
                if (dx == -rdx and dy == -rdy)
                    return true;
            }
        }
        return false;
    };

    auto [sx, sy] = source;
    vector<vector<int64_t>> distance(n, vector<int64_t>(m, -1));
    queue<pair<int, int>> q;
    distance[sx][sy] = 0;
    q.emplace(sx, sy);
    while (!q.empty()) {
        auto [ux, uy] = q.front();
        answer = max(answer, distance[ux][uy]);
        q.pop();
        for (auto &[dx, dy] : possibleMoves(ux, uy)) {
            int vx = ux + dx, vy = uy + dy;
            if (isPossibleMove(ux, uy, dx, dy) and distance[vx][vy] == -1) {
                distance[vx][vy] = distance[ux][uy] + 1;
                q.emplace(vx, vy);
            }
        }
    }

    return answer;
}

int64_t gold(vector<string> lines) {
    int64_t answer = 0;
    vector<string> a(2 * lines.size(), string(2 * lines[0].size(), '.'));
    for (int i = 0; i < (int)lines.size(); i++) {
        for (int j = 0; j < (int)lines[0].size(); j++) {
            a[2 * i][2 * j] = lines[i][j];
        }
    }

    const int n = (int)a.size(), m = (int)a[0].size();

    auto isInRange = [&](int x, int y) -> bool {
        return 0 <= x and x < n and 0 <= y and y < m;
    };

    auto possibleMoves = [&](int x, int y) -> vector<pair<int, int>> {
        char c = a[x][y];
        if (c == 'S') {
            return {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
        } else if (c == 'F') {
            return {{0, 1}, {1, 0}};
        } else if (c == 'L') {
            return {{0, 1}, {-1, 0}};
        } else if (c == 'J') {
            return {{0, -1}, {-1, 0}};
        } else if (c == '7') {
            return {{0, -1}, {1, 0}};
        } else if (c == '-') {
            return {{0, -1}, {0, 1}};
        } else if (c == '|') {
            return {{1, 0}, {-1, 0}};
        }
        return {};
    };

    auto isPossibleMove = [&](int x, int y, int dx, int dy) -> bool {
        if (!isInRange(x + dx, y + dy))
            return false;
        char c = a[x + dx][y + dy];
        string allowedChars = "SFLJ7-|";
        bool validChar = false;
        for (char ch : allowedChars) {
            if (ch == c)
                validChar = true;
        }
        if (validChar) {
            for (auto &[rdx, rdy] : possibleMoves(x + dx, y + dy)) {
                if (dx == -rdx and dy == -rdy)
                    return true;
            }
        }
        return false;
    };

    for (int i = 0; i < n; i++) {
        for (int j = 1; j < m; j += 2) {
            if (isPossibleMove(i, j, 0, -1) and isPossibleMove(i, j, 0, 1))
                a[i][j] = '-';

            if (isPossibleMove(i, j, -1, 0) and isPossibleMove(i, j, 1, 0))
                a[i][j] = '|';
        }
    }
    for (int j = 0; j < m; j++) {
        for (int i = 1; i < n; i += 2) {
            if (isPossibleMove(i, j, 0, -1) and isPossibleMove(i, j, 0, 1))
                a[i][j] = '-';

            if (isPossibleMove(i, j, -1, 0) and isPossibleMove(i, j, 1, 0))
                a[i][j] = '|';
        }
    }

    pair<int, int> source = {-1, -1};

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (a[i][j] == 'S') {
                source = {i, j};
                break;
            }
        }
        if (source != make_pair(-1, -1))
            break;
    }

    auto [sx, sy] = source;
    vector<vector<int64_t>> distance(n, vector<int64_t>(m, -1));
    queue<pair<int, int>> q;
    distance[sx][sy] = 0;
    q.emplace(sx, sy);
    while (!q.empty()) {
        auto [ux, uy] = q.front();
        q.pop();
        for (auto &[dx, dy] : possibleMoves(ux, uy)) {
            int vx = ux + dx, vy = uy + dy;
            if (isPossibleMove(ux, uy, dx, dy) and distance[vx][vy] == -1) {
                distance[vx][vy] = distance[ux][uy] + 1;
                q.emplace(vx, vy);
            }
        }
    }

    function<void(int, int)> dfs = [&](int x, int y) -> void {
        distance[x][y] = 0;
        for (auto &[dx, dy] : possibleMoves(sx, sy)) {
            int vx = x + dx, vy = y + dy;
            if (isInRange(vx, vy) and distance[vx][vy] == -1) {
                dfs(vx, vy);
            }
        }
    };

    for (int i = 0; i < n; i++) {
        int x = i, y = 0;
        if (distance[x][y] == -1)
            dfs(x, y);
        y = m - 1;
        if (distance[x][y] == -1)
            dfs(x, y);
    }

    for (int j = 0; j < m; j++) {
        int x = 0, y = j;
        if (distance[x][y] == -1)
            dfs(x, y);
        x = n - 1;
        if (distance[x][y] == -1)
            dfs(x, y);
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            answer += (distance[i][j] == -1);
        }
    }

    int result = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            vector<pair<int, int>> nextPositions = {
                {i, j}, {i + 1, j + 1}, {i + 1, j}, {i, j + 1}};
            int count = 0;
            for (auto &[x, y] : nextPositions) {
                if (isInRange(x, y)) {
                    count += (distance[x][y] == -1);
                }
            }
            if (count == 4) {
                result++;
                for (auto &[x, y] : nextPositions) {
                    distance[x][y] = -2;
                }
            }
        }
    }
    answer = result;

    return answer;
}

int main() {
    vector<string> lines = readLines();
    cout << silver(lines) << '\n';
    cout << gold(lines) << '\n';
    return 0;
}
