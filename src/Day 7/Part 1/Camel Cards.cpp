#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

vector<string> lines;
vector<pair<string, int>> hands;
map<char, int> value = {{'A', 14}, {'K', 13}, {'Q', 12}, {'J', 11}, {'T', 10},
                        {'9', 9},  {'8', 8},  {'7', 7},  {'6', 6},  {'5', 5},
                        {'4', 4},  {'3', 3},  {'2', 2}};

void parseLines(const vector<string>& input) {
    for (const auto& currLine : input) {
        size_t spacePos = currLine.find(" ");
        string cards = currLine.substr(0, spacePos);
        int bid = stoi(currLine.substr(spacePos + 1));
        hands.emplace_back(cards, bid);
    }
}

pair<int, int> repeatedChars(const string& input) {
    map<char, int> charCount;
    for (char c : input) {
        charCount[c]++;
    }

    int maxCount = 0;
    int secondMaxCount = 0;
    for (const auto& entry : charCount) {
        if (entry.second > maxCount) {
            secondMaxCount = maxCount;
            maxCount = entry.second;
        } else if (entry.second > secondMaxCount && entry.second <= maxCount) {
            secondMaxCount = entry.second;
        }
    }

    return {maxCount, secondMaxCount};
}

bool compareHands(const pair<string, int>& a, const pair<string, int>& b) {
    auto aChars = repeatedChars(a.first);
    auto bChars = repeatedChars(b.first);

    if (aChars.first != bChars.first) {
        return aChars.first < bChars.first;
    }

    if (aChars.first == bChars.first && aChars.first == 3) {
        if (aChars.second != bChars.second) {
            return aChars.second < bChars.second;
        }
    }

    if (aChars.first == bChars.first && aChars.first == 2) {
        if (aChars.second != bChars.second) {
            return aChars.second < bChars.second;
        }
    }

    for (size_t i = 0; i < a.first.size(); i++) {
        if (value.at(a.first[i]) != value.at(b.first[i])) {
            return value.at(a.first[i]) < value.at(b.first[i]);
        }
    }

    return false;
}

int main() {
    int sum = 0;
    string currLine;
    ifstream myfile("input.txt");

    if (myfile.is_open()) {
        while (getline(myfile, currLine)) {
            lines.emplace_back(currLine);
        }
        myfile.close();
    }

    parseLines(lines);
    sort(hands.begin(), hands.end(), compareHands);

    for (size_t i = 0; i < hands.size(); i++) {
        sum += hands[i].second * (i + 1);
    }

    cout << sum << endl;
    return 0;
}
