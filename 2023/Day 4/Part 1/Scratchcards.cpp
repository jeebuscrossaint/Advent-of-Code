#include <algorithm>
#include <numeric>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

vector<int> extract_numbers(const string& s) {
    vector<int> numbers;
    size_t start = 0;
    size_t end = s.find(' ');
    for (int i = 0; i < s.size(); i += 3) {
        if (s[i] == ' ') {  
            numbers.push_back(stoi(s.substr(i+1, i+2)));
        } else {
            numbers.push_back(stoi(s.substr(i, i+2)));
        }
    }
    return numbers;
}

int main(int argc, char * argv[]) {
  string input = "input.txt";
  if (argc > 1) {
    input = argv[1];
  }

  string line;
  fstream file(input);

  size_t total_points = 0;
   while(getline(file, line)) {
    size_t start = 0;
    size_t end = line.find(':');
    const int card_id = stoi(line.substr(5, end - 5));
    start = end + 2;
    end = line.find('|', start);
    const auto winning_numbers = extract_numbers(line.substr(start, end - start));
    const auto card_numbers = extract_numbers(line.substr(end + 2, line.size() - end));
    int count = 0;
    for (const auto& card_number : card_numbers) {
        if (std::find(std::begin(winning_numbers), std::end(winning_numbers), card_number) !=  std::end(winning_numbers)) {
            count++;
        }
    }
    if (count > 0) {
        total_points += pow(2, count - 1);
    }
   }
  cout << total_points << '\n';
   return 0;
}