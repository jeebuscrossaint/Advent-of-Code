#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>

using uint = unsigned int;
using uu = std::pair<uint, uint>;

namespace
{
    struct Range
    {
        uint dst, src, len;
        friend std::ostream& operator<<(std::ostream& in, Range const& r);
    };

    std::ostream& operator<<(std::ostream& in, Range const& r)
    {
        in << r.dst << ' ' << r.src << ' ' << r.len;
        return in;
    }

    struct Map
    {
        std::vector<Range> ranges;

        uint apply(uint n)
        {
            for (auto const& r : ranges)
                if (r.src <= n and n <= r.src + r.len - 1)
                    return n - r.src + r.dst;
            // Any source number that isn't mapped
            // corresponds to the same destination number.
            return n;
        }
    };
} // anonymous namespace

Map s2s, s2f, f2w, w2l, l2t, t2h, h2l;
std::vector<uint> seeds;

void read_input();

int main()
{
    read_input();

    uu ans{UINT_MAX, UINT_MAX};

    for (auto seed : seeds)
    {
        ans.first = std::min(ans.first,
            h2l.apply(t2h.apply(l2t.apply(w2l.apply(f2w.apply(s2f.apply(s2s.apply(seed)))))))
        );
    }
    std::cout << ans.first << '\n';

    assert(seeds.size() % 2 == 0);
    for (uint i = 0; i < seeds.size(); i += 2)
    {
        for (uint seed = seeds[i]; seed <= seeds[i] + seeds[i + 1] - 1; seed++)
        {
            ans.second = std::min(ans.second,
                h2l.apply(t2h.apply(l2t.apply(w2l.apply(f2w.apply(s2f.apply(s2s.apply(seed)))))))
            );
        }
    }
    std::cout << ans.second << '\n';
}


void read_input()
{
    std::ifstream file("input.txt");
    if (!file)
    {
        std::cerr << "Failed to open input.txt\n";
        return;
    }

    std::string line;

    std::getline(file, line);
    std::stringstream ss(line);
    std::string str;
    ss >> str;
    assert(str == "seeds:");
    uint seed;
    while (!ss.eof())
    {
        ss >> seed;
        seeds.push_back(seed);
    }

    std::getline(file, line);
    assert(line.empty());

    auto read_map = [&line, &ss, &file](std::string_view map_name, Map& m)
    {
        std::getline(file, line);
        std::ostringstream oss;
        oss << map_name << " map:";
        assert(line == oss.str());
        for (;;)
        {
            std::getline(file, line);
            if (line.empty())
                break;
            ss = std::stringstream(line);
            Range r;
            ss >> r.dst >> r.src >> r.len;
            m.ranges.push_back(r);
        }
    };

    read_map("seed-to-soil", s2s);
    read_map("soil-to-fertilizer", s2f);
    read_map("fertilizer-to-water", f2w);
    read_map("water-to-light", w2l);
    read_map("light-to-temperature", l2t);
    read_map("temperature-to-humidity", t2h);
}
