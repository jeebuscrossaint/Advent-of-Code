#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <string>

using namespace std;

// Directions are represented as (dy, dx)
const vector<pair<int, int>> DIRECTIONS = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}}; // Up, Right, Down, Left

// Function to simulate the guard's path and return the number of distinct positions visited
int part1(const string& filename) {
    // Read the map from the input file
    ifstream input(filename);
    vector<string> map;
    string line;

    while (getline(input, line)) {
        map.push_back(line);
    }

    int rows = map.size();
    int cols = map[0].size();

    // Find the initial position of the guard and set the direction
    int guard_x = -1, guard_y = -1, direction = -1;

    // Search for the initial position and direction
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            if (map[y][x] == '^') {
                guard_x = x;
                guard_y = y;
                direction = 0; // Up
                break;
            } else if (map[y][x] == '>') {
                guard_x = x;
                guard_y = y;
                direction = 1; // Right
                break;
            } else if (map[y][x] == 'v') {
                guard_x = x;
                guard_y = y;
                direction = 2; // Down
                break;
            } else if (map[y][x] == '<') {
                guard_x = x;
                guard_y = y;
                direction = 3; // Left
                break;
            }
        }
        if (guard_x != -1) break;
    }

    set<pair<int, int>> visitedPositions; // Set to store the distinct visited positions
    visitedPositions.insert({guard_y, guard_x});

    while (true) {
        // Calculate the next position based on the current direction
        int new_y = guard_y + DIRECTIONS[direction].first;
        int new_x = guard_x + DIRECTIONS[direction].second;

        // Check if the guard is out of bounds
        if (new_y < 0 || new_y >= rows || new_x < 0 || new_x >= cols) {
            break; // Guard has left the area
        }

        // If the next position is an obstacle, turn right (clockwise)
        if (map[new_y][new_x] == '#') {
            direction = (direction + 1) % 4;
        } else {
            // Otherwise, move to the new position
            guard_y = new_y;
            guard_x = new_x;
            visitedPositions.insert({guard_y, guard_x});
        }
    }

    return visitedPositions.size(); // Return the number of distinct positions visited
}

int main() {
    // Call part1 with the input file "input.txt"
    int result = part1("input.txt");
    cout << "The guard visited " << result << " distinct positions before leaving the map." << endl;
    return 0;
}
