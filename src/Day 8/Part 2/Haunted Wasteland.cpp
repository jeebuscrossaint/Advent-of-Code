#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <numeric>
#include <cassert>

std::map<std::string, std::pair<std::string, std::string>> m;
std::string dirs;

auto find_cycle_len(std::string curr, bool part2)
{
    auto steps = 0LL;

    while (true)
    {
        for (auto d : dirs)
        {
            steps++;
            curr = (d == 'L') ? m[curr].first : m[curr].second;
            if (curr == "ZZZ" || (part2 && curr.back() == 'Z'))
                return steps;
        }
    }
}

auto solve(const std::string& filename)
{
    std::ifstream file(filename);
    std::string line;

    std::getline(file, dirs);
    std::getline(file, line);

    while (std::getline(file, line))
        m[line.substr(0, 3)] = {line.substr(7, 3), line.substr(12, 3)};

    auto retval = std::make_pair(find_cycle_len("AAA", false), 1LL);

    for (auto& i : m)
    {
        if (i.first.back() == 'A')
        {
            retval.second = std::lcm(retval.second, find_cycle_len(i.first, true)); // C++17
        }
    }

    return retval;
}

int main()
{
    auto answer = solve("input.txt");
    std::cout << " " << answer.first << std::endl;
    std::cout << " " << answer.second << std::endl;
    return 0;
}
