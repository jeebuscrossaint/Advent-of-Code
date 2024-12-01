#include <bits/stdc++.h>
using namespace std;

int main()
{
    ifstream file("input.txt");
    vector<int> coord2;
    unordered_map<string, string> num_map = {{"1","one"}, {"2","two"}, {"3","three"}, {"4","four"}, {"5","five"}, {"6","six"}, {"7","seven"}, {"8","eight"}, {"9","nine"}};

    for (string line; file >> line;)
    {
        unordered_map<size_t, pair<string, string>> finds;
        for (const auto& num: num_map)
        {
            size_t found = line.find(num.second);
            while (found != string::npos)
            {
                finds[found] = num;
                found = line.find(num.second, found+1);
            }
        }
        vector<int> find_idxs;
        transform(finds.begin(), finds.end(), back_inserter(find_idxs), [](const auto& pair) { return pair.first; });
        sort(find_idxs.begin(), find_idxs.end());
        if (!find_idxs.empty())
        {
            line.replace(find_idxs.front(), 1, finds[find_idxs.front()].first);
            line.replace(find_idxs.back(), 1, finds[find_idxs.back()].first);
        }

        line.erase(remove_if(line.begin(), line.end(), ::isalpha), line.end());
        string n = {line.front(), line.back()};
        coord2.push_back(stoi(n));
    }
    auto res2 = accumulate(coord2.begin(), coord2.end(), 0);
    cout << res2 << endl;
    return(0);
}