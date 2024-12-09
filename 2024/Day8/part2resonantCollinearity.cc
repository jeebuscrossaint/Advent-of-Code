#include <fstream>
#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>

int main() {
  std::unordered_map<char, std::vector<std::vector<int>>> antenna_pos_by_freq;
  std::set<char> keys;
  int p1 = 0;
  std::ifstream in("input.txt");
  std::string line;
  std::vector<std::string> arr;
  int row = 0;
  int xdim = 0, ydim = 0;
  int p2 = 0;
  while (getline(in, line)) {
    for (int col = 0; col < line.size(); col++) {
      if (line[col] != '.') {
        if (antenna_pos_by_freq.find(line[col]) == antenna_pos_by_freq.end())
          antenna_pos_by_freq[line[col]] = {};
        antenna_pos_by_freq[line[col]].push_back({row, col});
        keys.insert(line[col]);
      }
    }
    arr.push_back(line);
    ydim = line.size();
    xdim++;
    row++;
  }
  xdim -= 1;
  ydim -= 1;

  for (char key : keys) {
    for (int i = 0; i < antenna_pos_by_freq[key].size(); i++) {
      std::vector<int> pos1 = antenna_pos_by_freq[key][i];
      for (int j = i; j < antenna_pos_by_freq[key].size(); j++) {
        if (i == j)
          continue;
        std::vector<int> pos2 = antenna_pos_by_freq[key][j];
        if (pos2[0] < pos1[0]) {
          std::vector<int> temp = pos2;
          pos2 = pos1;
          pos1 = temp;
        }
        int xdiff = pos2[0] - pos1[0], ydiff = pos2[1] - pos1[1];
        std::vector<int> temp1 = pos1, temp2 = pos2;
        while (0 <= temp1[0] && temp1[0] <= xdim && 0 <= temp1[1] &&
               temp1[1] <= ydim) {
          std::cout << pos1[0] << ',' << pos1[1] << '\n';
          arr[temp1[0]][temp1[1]] = '#';
          temp1[0] -= xdiff;
          temp1[1] -= ydiff;
        }
        while (0 <= temp2[0] && temp2[0] <= xdim && 0 <= temp2[1] &&
               temp2[1] <= ydim) {
          arr[temp2[0]][temp2[1]] = '#';
          temp2[0] += xdiff;
          temp2[1] += ydiff;
        }
      }
    }
  }
  for (std::string row : arr) {
    std::cout << row << '\n';
    for (char ch : row)
      if (ch == '#')
        p2++;
  }
  std::cout << "p2: " << p2 << '\n';
}
