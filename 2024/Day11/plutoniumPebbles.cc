#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

// Helper function to count digits in a number
int countDigits(long long n) {
    if (n == 0) return 1;
    int count = 0;
    while (n > 0) {
        count++;
        n /= 10;
    }
    return count;
}

// Helper function to split a number into two parts

std::pair<long long, long long> splitNumber(long long n) {
    int digits = countDigits(n);
    int leftDigits = digits / 2;
    long long divisor = 1;
    for (int i = 0; i < leftDigits; i++) {
        divisor *= 10;
    }
    return {n / divisor, n % divisor};
}

// Function to process blink using a map
std::unordered_map<long long, long long> processBlink(const std::unordered_map<long long, long long>& stones) {
    std::unordered_map<long long, long long> newStones;

    for (const auto& [number, count] : stones) {
        if (number == 0) {
            // Rule 1: 0 becomes 1
            newStones[1] += count;
        }
        else if (countDigits(number) % 2 == 0) {
            // Rule 2: Split even-digit numbers
            auto [left, right] = splitNumber(number);
            newStones[left] += count;
            newStones[right] += count;
        }
        else {
            // Rule 3: Multiply by 2024
            newStones[number * 2024] += count;
        }
    }

    return newStones;
}

// Function to read input from a file into an unordered_map

std::unordered_map<long long, long long> readInput(const std::string& filename) {
    std::unordered_map<long long, long long> stones;
    std::ifstream file(filename);
    long long number;

    while (file >> number) {
        stones[number]++; // Count of each number
    }

    return stones;
}

// 25 blinks for part 1

long long part1(const std::string& filename) {
    auto stones = readInput(filename);

    for (int i = 0; i < 25; i++) {
        stones = processBlink(stones);
    }

    // Sum up all counts
    long long total = 0;
    for (const auto& [number, count] : stones) {
        total += count;
    }
    return total;
}

// 75 blinks for part 2

long long part2(const std::string& filename) {
    auto stones = readInput(filename);

    for (int i = 0; i < 75; i++) {
        stones = processBlink(stones);
    }

    // Sum up all counts
    long long total = 0;
    for (const auto& [number, count] : stones) {
        total += count;
    }
    return total;
}

int main() {
        std::cout << "Part 1: " << part1("input.txt") << std::endl;
        std::cout << "Part 2: " << part2("input.txt") << std::endl;
        return 0;
}
