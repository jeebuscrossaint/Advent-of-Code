#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <cmath>

void calculateRoots(int64_t t, int64_t d, double& r1, double& r2) {
    double bDiv2a = t / 2.0;
    double addSub = sqrt(t * t - 4.0 * d) / 2.0;
    r1 = bDiv2a - addSub;
    r2 = bDiv2a + addSub;
}

int64_t concatNum(int64_t base, int64_t add) {
    int push = 10;
    while (push < add) {
        push *= 10;
    }
    return base * push + add;
}

void readNInts(std::ifstream& list, std::vector<int>* values, int64_t& concat, int n) {
    int num;
    for (int i = 0; i < n; i++) {
        list >> num;
        values->push_back(num);
        concat = concatNum(concat, num);
    }
}

int main(int argc, char** argv) {
    if (argc != 2)
        throw std::invalid_argument("Missing input file argument.");

    std::ifstream input;
    input.open(argv[1]);

    if (!input.is_open()) {
        std::cout << "Failed to open file." << std::endl;
        return -1;
    }

    std::vector<int>* times = new std::vector<int>();
    int64_t timeConcat = 0;
    std::vector<int>* distances = new std::vector<int>();
    int64_t distConcat = 0;
    std::string _;

    input >> _;
    readNInts(input, times, timeConcat, 4);

    input >> _;
    readNInts(input, distances, distConcat, 4);

    /*
    * The distance the boat will travel is determined by the expression xt-x^2,
    * where x is the time spent holding down the button and t is the final time.
    * The number of ways to win is the difference of the floor of the roots
    * of xt - x^2 - d, where d is the previous winning distance.
    */

    double r1, r2;
    int product = 1;
    for (int i = 0; i < 4; i++) {
        calculateRoots(times->at(i), distances->at(i), r1, r2);
        std::cout << "Roots: " << r1 << ", " << r2 << std::endl;
        std::cout << "Number of ways to win: " << std::floor(r2) - std::floor(r1) << std::endl;
        product *= std::floor(r2) - std::floor(r1);
    }

    std::cout << "Part 1: The product of all possible ways to win is " << product << "." << std::endl;

    calculateRoots(timeConcat, distConcat, r1, r2);

    std::cout << "Part 2: You can win in " << static_cast<int>(std::floor(r2) - std::floor(r1)) << " different ways." << std::endl;

    input.close();
    return 0;
}
