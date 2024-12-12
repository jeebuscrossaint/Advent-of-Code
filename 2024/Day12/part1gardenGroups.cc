#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <queue>

using namespace std;

// Direction arrays for traversing adjacent cells
const int dx[] = {-1, 1, 0, 0};  // up, down, left, right
const int dy[] = {0, 0, -1, 1};

struct Region {
    char type;
    int area;
    int perimeter;
};

// Check if a position is valid in the grid
bool isValid(int x, int y, int rows, int cols) {
    return x >= 0 && x < rows && y >= 0 && y < cols;
}

// Flood fill to find a region and calculate its properties
Region findRegion(vector<vector<char>>& grid, vector<vector<bool>>& visited,
                 int startX, int startY) {
    Region region = {grid[startX][startY], 0, 0};
    queue<pair<int, int>> q;
    q.push({startX, startY});
    visited[startX][startY] = true;

    while (!q.empty()) {
        int x = q.front().first;
        int y = q.front().second;
        q.pop();
        region.area++;

        // Check all four sides for perimeter and adjacent plots
        for (int i = 0; i < 4; i++) {
            int newX = x + dx[i];
            int newY = y + dy[i];

            if (!isValid(newX, newY, grid.size(), grid[0].size()) ||
                grid[newX][newY] != region.type) {
                region.perimeter++;
            } else if (!visited[newX][newY]) {
                visited[newX][newY] = true;
                q.push({newX, newY});
            }
        }
    }
    return region;
}

int calculateTotalPrice(vector<vector<char>>& grid) {
    int rows = grid.size();
    int cols = grid[0].size();
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    int totalPrice = 0;

    // Find all regions
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (!visited[i][j]) {
                Region region = findRegion(grid, visited, i, j);
                int price = region.area * region.perimeter;
                totalPrice += price;
            }
        }
    }
    return totalPrice;
}

int main() {
    ifstream file("input.txt");
    vector<vector<char>> grid;
    string line;

    // Read input
    while (getline(file, line)) {
        vector<char> row;
        for (char c : line) {
            row.push_back(c);
        }
        grid.push_back(row);
    }

    int result = calculateTotalPrice(grid);
    cout << "Total price: " << result << endl;

    return 0;
}
