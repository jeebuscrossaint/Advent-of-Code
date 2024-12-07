#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <string>

using namespace std;

// Directions are represented as (dy, dx)
const vector<pair<int, int>> DIRECTIONS = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}}; // Up, Right, Down, Left

// Function to simulate the guard's path and return whether the guard gets stuck
bool isLoopPossible(vector<string>& map, int obstacle_y, int obstacle_x) {
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

    // Add the obstacle to the map
    char original = map[obstacle_y][obstacle_x];
    map[obstacle_y][obstacle_x] = '#';

    // Track unique states to detect loops
    set<pair<pair<int, int>, int>> visitedStates;
    int steps = 0;

    while (true) {
        // Check for a loop
        pair<pair<int, int>, int> currentState = {{guard_y, guard_x}, direction};
        if (visitedStates.count(currentState)) {
            // Restore the map
            map[obstacle_y][obstacle_x] = original;
            return true;
        }
        visitedStates.insert(currentState);

        // Calculate the next position based on the current direction
        int new_y = guard_y + DIRECTIONS[direction].first;
        int new_x = guard_x + DIRECTIONS[direction].second;

        // Check if the guard is out of bounds
        if (new_y < 0 || new_y >= rows || new_x < 0 || new_x >= cols) {
            // Restore the map
            map[obstacle_y][obstacle_x] = original;
            return false;
        }

        // If the next position is an obstacle, turn right (clockwise)
        if (map[new_y][new_x] == '#') {
            direction = (direction + 1) % 4;
        } else {
            // Otherwise, move to the new position
            guard_y = new_y;
            guard_x = new_x;
        }

        // Prevent infinite loop in case of edge cases
        if (++steps > rows * cols * 100) {
            // Restore the map
            map[obstacle_y][obstacle_x] = original;
            return false;
        }
    }
}

int part2(const string& filename) {
    // Read the map from the input file
    ifstream input(filename);
    vector<string> map;
    string line;

    while (getline(input, line)) {
        map.push_back(line);
    }

    int rows = map.size();
    int cols = map[0].size();

    int loopPositions = 0;

    // Try placing an obstacle at every position
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            // Skip the starting position and existing obstacles
            if ((y == map.size() / 2 && x == map[0].size() / 2) || map[y][x] == '#') continue;

            // Create a copy of the map to modify
            vector<string> mapCopy = map;

            // Check if this position causes a loop
            if (isLoopPossible(mapCopy, y, x)) {
                loopPositions++;
            }
        }
    }

    return loopPositions;
}

int main() {
    // Call part2 with the input file "input.txt"
    int result = part2("input.txt");
    cout << "There are " << result << " positions that would cause the guard to get stuck in a loop." << endl;
    return 0;
}
