#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <cmath>

// helper struct
struct Point {
	int x, y;

	// custom hash function
	struct Hash {
		size_t operator()(const Point& p) const {
			return std::hash<int>()(p.x) ^ std::hash<int>()(p.y);
		}
	};

	// custom equality function
	bool operator==(const Point& other) const {
		return x == other.x && y == other.y;
	}
};

// devious time with a class
class AntennaSignalAnalyzer {
private:
	std::vector<std::string> grid;
	std::unordered_map<char, std::vector<Point>> antennas;

	// check if a point is within the grid bounds
	bool isInBounds(const Point& p) const {
		return p.x >= 0 && p.x < grid[0].size() && p.y >= 0 && p.y < grid.size();
	}

	// calculate manhattan distance between two points
	int manhattanDistance(const Point& a, const Point& b) const {
		return std::abs(a.x - b.x) + std::abs(a.y - b.y);
	}

	// find antinodes for a specific antenna frequency
	std::unordered_set<Point, Point::Hash> findAntinodes(char freq) const {
		std::unordered_set<Point, Point::Hash> antinodes;
		const auto& points = antennas.at(freq);

		for (size_t i = 0; i < points.size(); ++i) {
			for (size_t j = i + 1; j < points.size(); ++j) {
				const auto& a = points[i];
				const auto& b = points[j];
				int dist = manhattanDistance(a, b);

				// calculate potential antinode points
				Point antinode1 {
					a.x + (b.x - a.x) * 2,
					a.y + (b.y - a.y) * 2
				};

				Point antinode2 {
					b.x + (a.x - b.x) * 2,
					b.y + (a.y - b.y) * 2
				};

				// add antinodes if they are within grid bounds
				if (isInBounds(antinode1)) antinodes.insert(antinode1);
				if (isInBounds(antinode2)) antinodes.insert(antinode2);
			}
		}

		return antinodes;
	}

public:
	// load a grid fro ma fille
	void loadGrid(const std::string& filename) {
		std::ifstream file(filename);
		std::string line;
		grid.clear();
		antennas.clear();

		while (std::getline(file, line)) {
			grid.push_back(line);
		}
	}

	// populate antenna positions
	void findAntennas() {
		for (int y = 0; y < grid.size(); ++y) {
			for (int x = 0; x < grid[y].size(); ++x) {
				char ch = grid[y][x];
				if (ch != '.' && ch != ' ') {
					antennas[ch].push_back({x, y});
				}
			}
		}
	}

	// calculate total unique antinodes
	long long calculateAntinodes() {
		std::unordered_set<Point, Point::Hash> uniqueAntinodes;

		for (const auto& [freq, _] : antennas) {
			auto freqAntinodes = findAntinodes(freq);
			uniqueAntinodes.insert(freqAntinodes.begin(), freqAntinodes.end());
		}

		return uniqueAntinodes.size();
	}
};


long long part1(const std::string& filename) {
	AntennaSignalAnalyzer analyzer;
	analyzer.loadGrid(filename);
	analyzer.findAntennas();
	return analyzer.calculateAntinodes();
}

int main() {
	std::cout << "part 1: " << part1("input.txt") << std::endl;
}
