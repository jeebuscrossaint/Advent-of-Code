#include <algorithm>
#include <numeric>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

vector<int> extract_numbers(const string& s) {
  //cout << s << '\n';
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


struct Card {
  int id;
  // Number of elements is small enough to make vectors more efficient than unordered_sets
  vector<int> winning_numbers;
  vector<int> numbers_on_card;
  size_t copies;
};

int main(int argc, char * argv[]) {
  string input = "input.txt";
  if (argc > 1) {
    input = argv[1];
  }

  string line;
  fstream file(input);

  size_t total_points = 0;
  vector<Card> cards;
  while(getline(file, line)) {
    Card c;
    size_t start = 0;
    size_t end = line.find(':');
    c.id = stoi(line.substr(5, end - 5));
    start = end + 2;
    end = line.find('|', start);
    
    c.winning_numbers = extract_numbers(line.substr(start, end - start));
    c.numbers_on_card = extract_numbers(line.substr(end + 2, line.size() - end));
    c.copies = 1;
    cards.push_back(c);
  }

  for (const auto& card : cards) {
    int count = 0;
    for (const auto& number_on_card : card.numbers_on_card) {
      if (find(begin(card.winning_numbers), end(card.winning_numbers), number_on_card) !=  end(card.winning_numbers)) {
        count++;
      }
    }
    for (int i = card.id + 1 - 1 /*zero index*/; i < min(cards.size() , size_t(card.id + count)); i++) {
      cards[i].copies += card.copies;
    }
  }
  cout << accumulate(begin(cards), end(cards), 0, [](size_t total, const Card& card) {return total + card.copies;}) << '\n';
  return 0;
}