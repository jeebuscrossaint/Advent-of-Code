#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <sstream>

class CardHand
{
private:
    std::string hand;
    long long bidValue;

public:
    CardHand(std::string hand, long long bidValue) : hand{hand}, bidValue{bidValue} {}

    std::string getHand() const { return hand; }
    long long getBidValue() const { return bidValue; }
};

enum StrengthType
{
    HighCard,
    OnePair,
    TwoPair,
    ThreeOfAKind,
    FullHouse,
    FourOfAKind,
    FiveOfAKind
};

bool isFind(const std::vector<int> &vec, int target)
{
    return std::find(vec.begin(), vec.end(), target) != vec.end();
}

namespace Part1
{
    std::map<char, int> convertToRelativeStrength{
        {'2', 1}, {'3', 2}, {'4', 3}, {'5', 4}, {'6', 5}, {'7', 6}, {'8', 7}, {'9', 8},
        {'T', 9}, {'J', 10}, {'Q', 11}, {'K', 12}, {'A', 13}};

    StrengthType getStrengthType(const std::string &hand)
    {
        std::vector<int> occurrence;
        std::map<char, int> countOccurrence;

        for (auto c : hand)
            ++countOccurrence[c];

        for (auto p : countOccurrence)
            occurrence.push_back(p.second);

        if (isFind(occurrence, 5))
            return FiveOfAKind;
        else if (isFind(occurrence, 4))
            return FourOfAKind;
        else if (isFind(occurrence, 3) && isFind(occurrence, 2))
            return FullHouse;
        else if (isFind(occurrence, 3))
            return ThreeOfAKind;
        else if (std::count(occurrence.begin(), occurrence.end(), 2) == 2)
            return TwoPair;
        else if (isFind(occurrence, 2))
            return OnePair;
        else
            return HighCard;
    }

    bool compare(const CardHand &c1, const CardHand &c2)
    {
        std::string c1Hand = c1.getHand();
        std::string c2Hand = c2.getHand();

        if (getStrengthType(c1Hand) != getStrengthType(c2Hand))
            return getStrengthType(c1Hand) < getStrengthType(c2Hand);
        else
        {
            for (int i = 0; i < 5; ++i)
            {
                if (c1Hand[i] != c2Hand[i])
                    return convertToRelativeStrength[c1Hand[i]] < convertToRelativeStrength[c2Hand[i]];
            }
            return false;
        }
    }
}

namespace Part2
{
    std::string allCardFace = "23456789TQKA";
    std::map<char, int> convertToRelativeStrength{
        {'J', 1}, {'2', 2}, {'3', 3}, {'4', 4}, {'5', 5}, {'6', 6}, {'7', 7}, {'8', 8},
        {'9', 9}, {'T', 10}, {'Q', 11}, {'K', 12}, {'A', 13}};

    StrengthType getStrengthType(const std::string &hand)
    {
        StrengthType maxHandStrength = HighCard;

        for (auto face : allCardFace)
        {
            std::string possibleHand = hand;
            std::replace(possibleHand.begin(), possibleHand.end(), 'J', face);

            StrengthType handStrength;
            std::vector<int> occurrence;
            std::map<char, int> countOccurrence;

            for (auto c : possibleHand)
                ++countOccurrence[c];

            for (auto p : countOccurrence)
                occurrence.push_back(p.second);

            if (isFind(occurrence, 5))
                handStrength = FiveOfAKind;
            else if (isFind(occurrence, 4))
                handStrength = FourOfAKind;
            else if (isFind(occurrence, 3) && isFind(occurrence, 2))
                handStrength = FullHouse;
            else if (isFind(occurrence, 3))
                handStrength = ThreeOfAKind;
            else if (std::count(occurrence.begin(), occurrence.end(), 2) == 2)
                handStrength = TwoPair;
            else if (isFind(occurrence, 2))
                handStrength = OnePair;
            else
                handStrength = HighCard;

            maxHandStrength = std::max(maxHandStrength, handStrength);
        }

        return maxHandStrength;
    }

    bool compare(const CardHand &c1, const CardHand &c2)
    {
        std::string c1Hand = c1.getHand();
        std::string c2Hand = c2.getHand();

        if (getStrengthType(c1Hand) != getStrengthType(c2Hand))
            return getStrengthType(c1Hand) < getStrengthType(c2Hand);
        else
        {
            for (int i = 0; i < 5; ++i)
            {
                if (c1Hand[i] != c2Hand[i])
                    return convertToRelativeStrength[c1Hand[i]] < convertToRelativeStrength[c2Hand[i]];
            }
            return false;
        }
    }
}

std::vector<std::string> processInput()
{
    std::string s;
    std::vector<std::string> input;

    while (std::getline(std::cin, s))
        input.push_back(s);

    return input;
}

long long part2(std::vector<std::string> &input)
{
    std::vector<CardHand> cardsHand;

    for (const std::string &cardHand : input)
    {
        std::stringstream ss(cardHand);
        std::string hand;
        ss >> hand;

        std::string bidStr;

        if (!(ss >> bidStr))
        {
            continue; // Skip this entry and move to the next one
        }

        long long bid = std::stoll(bidStr);
        cardsHand.push_back({hand, bid});
    }

    std::sort(cardsHand.begin(), cardsHand.end(), Part2::compare);

    long long totalWinnings = 0;

    for (int i = 0; i < cardsHand.size(); ++i)
        totalWinnings += (i + 1) * cardsHand[i].getBidValue();

    return totalWinnings;
}

int main()
{
    std::freopen("input.txt", "r", stdin);
    std::freopen("output.txt", "w", stdout);

    std::vector<std::string> input = processInput();

    std::cout << "The solution for part 2 is: " << part2(input) << '\n';

    return 0;
}
