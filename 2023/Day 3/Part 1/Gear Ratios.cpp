#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
#include <algorithm>
#include <utility>
#include <set>
#include <map>

using namespace std;

vector<string> processInput()
{
    ifstream inputFile("input.txt");
    vector<string> input;
    string s;
    while (inputFile >> s)
    {
        input.push_back(s);
    }
    inputFile.close();
    return input;
}

bool is_digit(char c)
{
    return (c >= '0' && c <= '9');
}

bool is_symbol(char c)
{
    return (c != '.' && !is_digit(c));
}

void gear_of_number(set<pair<long long, long long>>& gears, long long i, long long j, vector<string> input)
{
    for(long long a = i - 1; a <= i + 1; ++a)
    {
        for(long long b = j - 1; b <= j + 1; ++b)
        {
            if(input[a][b] == '*')
            {
                gears.insert({a, b});
            }
        }
    }
}

long long part1(vector<string> input)
{
    long long row_size = input[0].size();
    long long sum = 0;

    input.insert(input.begin(), std::string(row_size, '.'));
    input.push_back(std::string(row_size, '.'));

    for(long long i = 0; i < input.size(); ++i)
    {
        input[i].insert(input[i].begin(), '.');
    }

    for(long long i = 0; i < input.size(); ++i)
    {
        input[i].push_back('.');
    }

    for(long long i = 1; i < input.size() - 1; ++i)
    {
        std::string number;
        bool is_part_number = false;

        for(long long j = 1; j < input[i].size() - 1; ++j)
        {
            int character = input[i][j];

            bool top_left = is_symbol(input[i - 1][j - 1]);
            bool top_center = is_symbol(input[i - 1][j]);
            bool top_right = is_symbol(input[i - 1][j + 1]);
            bool middle_left = is_symbol(input[i][j - 1]);
            bool middle_right = is_symbol(input[i][j + 1]);
            bool bottom_left = is_symbol(input[i + 1][j - 1]);
            bool bottom_center = is_symbol(input[i + 1][j]);
            bool bottom_right = is_symbol(input[i + 1][j + 1]);

            if(is_digit(character))
            {
                number += character;

                if(top_left || top_center || top_right || middle_left || middle_right || bottom_left || bottom_center || bottom_right)
                    is_part_number = true;
            }
            if(!is_digit(character) || j == row_size )
            {

                if(is_part_number && !number.empty() )
                {
                    sum += std::stol(number);
                }
                number.clear();
                is_part_number = false;

            }
        }
    }
    return sum;
}

long long part2(std::vector<std::string> input)
{
    long long row_size = input[0].size();
    long long sum = 0;

    input.insert(input.begin(), std::string(row_size, '.'));
    input.push_back(std::string(row_size, '.'));

    for(long long i = 0; i < input.size(); ++i)
    {
        input[i].insert(input[i].begin(), '.');
    }

    for(long long i = 0; i < input.size(); ++i)
    {
        input[i].push_back('.');
    }
    std::set<std::pair<long long, long long>> gears;

    std::map<std::pair<long long, long long>, std::vector<long long>> gear_to_number;
    for(long long i = 1; i < input.size() - 1; ++i)
    {
        std::string number;

        for(long long j = 1; j < input[i].size() - 1; ++j)
        {
            int character = input[i][j];
            if(is_digit(character))
            {
                number += character;
                gear_of_number(gears, i, j, input);
            }

            if(!is_digit(character) || j == row_size )
            {
                //std::cout << number << '\n';
                if (!number.empty() && !gears.empty())
                {
                    for(auto p : gears)
                    {
                        gear_to_number[p].push_back(std::stoi(number));
                    }
                }
                gears.clear();
                number.clear();
            }
        }
    }

    for(auto p : gear_to_number)
    {
        if(!p.second.empty())
        {
            if(p.second.size() >= 2)
            {
                long long mutiple = 1;

                for(auto i : p.second)
                {
                    mutiple *= i;
                }
                sum += mutiple;
            }
        }
    }
    return sum;
}

int main()
{
    //std::freopen("day3inp.txt", "r", stdin);
    //std::freopen("day3out.txt", "w", stdout);

    std::vector<std::string> input = processInput();

    std::cout << ""<< part1(input) <<'\n';
    //std::cout << "The solution for part 2 is: " << part2(input) <<'\n';
}