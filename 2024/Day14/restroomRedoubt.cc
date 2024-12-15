#include <array>
#include <climits>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdint>
#include <cassert>

using namespace std;

constexpr int N = 103;
constexpr int P = 101;

struct Robot {
    int x, y, dx, dy;
};

float calculate_variance(const vector<Robot>& robots, bool use_x) {
    float mean = 0;
    for (const auto& robot : robots) {
        mean += use_x ? robot.x : robot.y;
    }
    mean /= robots.size();

    float variance = 0;
    for (const auto& robot : robots) {
        float diff = (use_x ? robot.x : robot.y) - mean;
        variance += diff * diff;
    }

    return variance;
}

int main(int argc, char** argv) {
    // Ensure a single argument is provided (the input file path)
    assert(argc == 2);

    ifstream file(argv[1]);
    if (!file) {
        cerr << "Error: Could not open file " << argv[1] << endl;
        return 1;
    }

    vector<Robot> robots;
    string line;
    int t = 100;
    int quadrants[4] = {};

    // Parse input and calculate quadrants for part 1
    while (getline(file, line)) {
        int data[4];
        char* ptr = &line[2];
        for (int i = 0; i < 4; ++i) {
            data[i] = strtol(ptr, &ptr, 10);
            ptr += (i == 1) ? 3 : 1;
        }

        robots.push_back({data[0], data[1], data[2], data[3]});

        int x = (data[0] + t * data[2]) % P;
        int y = (data[1] + t * data[3]) % N;
        if (x < 0) x += P;
        if (y < 0) y += N;

        quadrants[(x < P / 2) * 2 + (y < N / 2)] += (x != P / 2 && y != N / 2);
    }

    int64_t output = 1;
    for (int count : quadrants) {
        output *= count;
    }

    cout << "part1: " << output << endl;

    // Part 2: Detect times X and Y where variance is minimized
    int X = 0, Y = 0;
    float min_variance_x = INFINITY, min_variance_y = INFINITY;

    for (int i = 0; i < N; ++i) {
        float variance_x = calculate_variance(robots, true);
        float variance_y = calculate_variance(robots, false);

        if (i < P && variance_x < min_variance_x) {
            min_variance_x = variance_x;
            X = i;
        }

        if (variance_y < min_variance_y) {
            min_variance_y = variance_y;
            Y = i;
        }

        // Update robot positions for the next iteration
        for (auto& robot : robots) {
            robot.x = (robot.x + robot.dx) % P;
            robot.y = (robot.y + robot.dy) % N;

            if (robot.x < 0) robot.x += P;
            if (robot.y < 0) robot.y += N;
        }
    }

    // Solve for a and b such that P * a - N * b = Y - X
    int a = 0, b = 0;
    while (a * P % (N - P) != abs(Y - X) % (N - P) || a * P < (Y - X)) {
        ++a;
    }
    b = (a * P - (Y - X)) / (N - P);
    a += b;

    cout << "part2: " << X + P * a << endl;

    return 0;
}
