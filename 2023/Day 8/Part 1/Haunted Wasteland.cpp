#include <iostream>
#include <map>
#include <string>
#include <vector>

void readInput();

std::string instructions;
std::map<std::string, std::pair<std::string, std::string>> M;
std::vector<std::string> vs;

int main(int argc, char* argv[]) {
    readInput();

    std::string s = "AAA";
    if (argc > 1) {
        int const idx = std::stoi(argv[1]);
        s = (idx < vs.size()) ? vs[idx] : "AAA";
    }

    int i = 0, ans = 0;
    while (!s.empty() && s.back() != 'Z' && i < instructions.size()) {
        if (instructions[i] == 'L') {
            s = M[s].first;
        } else { // Assume 'R' when not 'L'
            s = M[s].second;
        }

        ans++;
        i = (i + 1) % instructions.size();
    }

    std::cout << ans << std::endl;
}

void readInput() {
    std::getline(std::cin, instructions);

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.size() < 15) {
            // Handle the case where the line is too short
            std::cerr << "Error: Line is too short." << std::endl;
            // Add error handling logic here
        } else {
            std::string org   = line.substr(0, 3);
            std::string left  = line.substr(7, 3);
            std::string right = line.substr(12, 3);
            M[org] = std::make_pair(left, right);
            if (org.back() == 'A') {
                vs.push_back(org);
            }
        }
    }
}
