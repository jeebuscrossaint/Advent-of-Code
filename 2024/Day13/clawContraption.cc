#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <iostream>

using IntRow = std::vector<long>;
using IntRows = std::vector<IntRow>;

IntRows readFile(const std::string& filename)
{
    std::ifstream infile(filename);
    IntRows csvRows;

    if (!infile.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return csvRows;
    }

    std::string line;
    while (std::getline(infile, line)) {
        auto colonPos = line.find(':');
        if (colonPos == std::string::npos) {
            continue;
        }

        std::string lineSub = line.substr(colonPos + 1);
        std::istringstream ss(lineSub);
        IntRow row;

        char discard;
        long value1 = 0, value2 = 0;
        ss >> discard >> discard >> value1 >> discard >> discard >> discard >> value2;

        row.push_back(value1);
        row.push_back(value2);
        csvRows.push_back(std::move(row));
    }

    return csvRows;
}

bool isWithinTolerance(double num1, double num2, double tolerance)
{
    return std::abs(num1 - num2) <= tolerance;
}

long solveEquations(const IntRows& equations, long prizeOffset)
{
    long tokenSum = 0;

    for (size_t i = 0; i < equations.size() / 3; ++i) {
        const long a = equations[i * 3][0];
        const long b = equations[i * 3 + 1][0];
        const long c = equations[i * 3][1];
        const long d = equations[i * 3 + 1][1];

        const double determinant = a * d - b * c;
        if (determinant == 0) {
            continue;
        }

        const double scale = 1.0 / determinant;
        const double aInv = scale * d;
        const double bInv = scale * (-b);
        const double cInv = scale * (-c);
        const double dInv = scale * a;

        const long prizeX = equations[i * 3 + 2][0] + prizeOffset;
        const long prizeY = equations[i * 3 + 2][1] + prizeOffset;

        const double aPress = aInv * prizeX + bInv * prizeY;
        const double bPress = cInv * prizeX + dInv * prizeY;

        const double aPressRounded = std::round(aPress);
        const double bPressRounded = std::round(bPress);

        if (isWithinTolerance(aPress, aPressRounded, 1e-3) && 
            isWithinTolerance(bPress, bPressRounded, 1e-3)) {
            tokenSum += static_cast<long>(std::lround(aPress)) * 3 + static_cast<long>(std::lround(bPress));
        }
    }

    return tokenSum;
}

int main()
{
    const std::string filename = "input.txt";
    const IntRows values = readFile(filename);

    if (values.empty()) {
        return 1;
    }

    const long tokenSumPart1 = solveEquations(values, 0);
    const long tokenSumPart2 = solveEquations(values, 10'000'000'000'000L);

    std::cout << tokenSumPart1 << '\n';
    std::cout << tokenSumPart2 << '\n';

    return 0;
}
