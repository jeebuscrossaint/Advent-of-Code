#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <sstream>
#include <queue>
#include <algorithm>

bool isValidOrder(const std::vector<int>& pages, const std::map<int, std::set<int>>& rules) {
	// each page check if its required predecessors come after it
	for (size_t i = 0; i < pages.size(); i++) {
		// if the page has rules
		if (rules.count(pages[i])) {
			// get set of pages that must come after this page
			const auto& mustComeAfter = rules.at(pages[i]);

			// check all subsequent pages
			for (size_t j = 0; j < i; j++) {
				// if we find a page that should come after the current page
				if (mustComeAfter.count(pages[j])) {
					return false;
				}
			}
		}
	}

	return true;
}

// parse a line containing numbers separated by commas (sounds like csv)
std::vector<int> parseLine(const std::string& line) {
	std::vector<int> numbers;
	std::stringstream ss(line);
	std::string number;

	while (getline(ss, number, ',')) {
		numbers.push_back(std::stoi(number));
	}

	return numbers;
}

long part1(const std::string& filename) {
	std::ifstream file(filename);
	std::string line;
	std::map<int, std::set<int>> rules;
	std::vector<std::vector<int>> updates;
	bool parsingRules = true;

	// parse input
	while (getline(file, line)) {
		if (line.empty()) {
			parsingRules = false;
			continue;
		}

		if (parsingRules) {
			// parse rule like "47|53"
			size_t pos = line.find('|');
			int before = std::stoi(line.substr(0, pos));
			int after = std::stoi(line.substr(pos + 1));
			rules[before].insert(after);
		} else {
			// parse update lines like "75,47,61,53,29"
			updates.push_back(parseLine(line));
		}
	}

	long sum = 0;
	// process each update
	for (const auto& update : updates) {
		if (isValidOrder(update, rules)) {
			// if valid add middle number to sum
			sum += update[update.size() / 2];
		}
	}

	std::cout << "part 1 sol: " << sum << std::endl;
	file.close();
	return sum;
}

std::vector<int> topologicalSort(const std::set<int>& nodes, const std::map<int, std::set<int>>& rules) {
	std::map<int, int> inDegree;
	std::map<int, std::set<int>> graph;

	// initialize in degree for all nodes
	for (int node: nodes) {
		inDegree[node] = 0;
		graph[node] = std::set<int>();
	}

	// build graph and calculate in degree
	for (int node : nodes) {
		if (rules.count(node)) {
			for (int after : rules.at(node)) {
				if (nodes.count(after)) { // only consider edges between nodes in this update
					graph[node].insert(after);
					inDegree[after]++;
				}
			}
		}
	}

	// use priority queue to always pick smallest available node
	std::priority_queue<int, std::vector<int>, std::greater<int>> queue;
	// add all nodes with 0 in degree
	for (const auto& pair : inDegree) {
		if (pair.second == 0) {
			queue.push(pair.first);
		}
	}

	std::vector<int> result;
	while (!queue.empty()) {
		int current = queue.top();
		queue.pop();
		result.push_back(current);

		// reduce in degree for all neighbors
		for (int neighbor : graph[current]) {
			inDegree[neighbor]--;
			if (inDegree[neighbor] == 0) {
				queue.push(neighbor);
			}
		}
	}

	return result;
}

long part2(const std::string& filename) {
	std::ifstream file(filename);
	std::string line;
	std::map<int, std::set<int>> rules;
	std::vector<std::vector<int>> updates;
	bool parsingRules = true;

	// parse input
	while (getline(file, line)) {
		if (line.empty()) {
			parsingRules = false;
			continue;
		}

		if (parsingRules) {
			size_t pos = line.find('|');
			int before = std::stoi(line.substr(0, pos));
			int after = std::stoi(line.substr(pos + 1));
			rules[before].insert(after);
		} else {
			updates.push_back(parseLine(line));
		}
	}

	long sum = 0;
	// process each update
	for (const auto& update : updates) {
		if (!isValidOrder(update, rules)) {
			// convert update to set for topological sort
			std::set<int> updateSet(update.begin(), update.end());

			// get correctly ordered version
			std::vector<int> orderedUpdate = topologicalSort(updateSet, rules);

			// add middle number to sum
			sum += orderedUpdate[orderedUpdate.size() / 2];
		}
	}

	std::cout << "part 2 sol: " << sum << std::endl;
	file.close();
	return sum;
}

int main() {
	part1("input.txt");
	part2("input.txt");
	return 0;
}
