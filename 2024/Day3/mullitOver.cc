#include <iostream>
#include <fstream>
#include <regex>
#include <string>

long part1(const std::string filename) {
	std::ifstream file(filename);
	std::string line;

	std::regex pattern(R"(mul\((\d+),(\d+)\))");
	std::smatch matches;

	long long sum = 0;

	while (std::getline(file, line)) {
		std::string::const_iterator searchStart(line.cbegin());
		while (std::regex_search(searchStart, line.cend(), matches, pattern)) {
			long long operand1 = std::stoi(matches[1]);
			long long operand2 = std::stoi(matches[2]);
			long long product = operand1 * operand2;
			//std::cout << operand1 << "*" << operand2 << "=" << product << std::endl; debug
			sum += product;
			//line = matches.suffix().str(); old
			searchStart = matches.suffix().first;
		}
	}

	std::cout << "sum of mul ops " << sum << std::endl;
	file.close();
	return 0;
}

long part2(const std::string filename) {
    std::ifstream file(filename);
    std::string line;

    std::regex mul_pattern(R"(mul\((\d+),(\d+)\))");
    std::regex do_pattern(R"(do\(\))");
    std::regex dont_pattern(R"(don't\(\))");
    std::smatch matches;

    long long sum = 0;
    bool mul_enabled = true; // mul enabled by default

    while (std::getline(file, line)) {
        size_t pos = 0;
        while (pos < line.length()) {
            // Find the next occurrence of any pattern
            std::string remaining = line.substr(pos);
            std::string::const_iterator search_start(remaining.cbegin());

            // Check for control instructions first
            if (std::regex_search(search_start, remaining.cend(), matches, do_pattern)) {
                if (matches.position(0) == 0) {
                    mul_enabled = true;
                    pos += matches.length(0);
                    continue;
                }
            }

            if (std::regex_search(search_start, remaining.cend(), matches, dont_pattern)) {
                if (matches.position(0) == 0) {
                    mul_enabled = false;
                    pos += matches.length(0);
                    continue;
                }
            }

            // Check for multiplication
            if (std::regex_search(search_start, remaining.cend(), matches, mul_pattern)) {
                if (matches.position(0) == 0) {
                    if (mul_enabled) {
                        long long operand1 = std::stoll(matches[1]);
                        long long operand2 = std::stoll(matches[2]);
                        long long product = operand1 * operand2;
                        sum += product;
                    }
                    pos += matches.length(0);
                    continue;
                }
            }

            // Move forward if no pattern matches at current position
            pos++;
        }
    }

    std::cout << "Sum of enabled mul ops: " << sum << std::endl;
    file.close();
    return sum;
}


int main() {
	part1("input.txt");
	part2("input.txt");
	return 0;
}
