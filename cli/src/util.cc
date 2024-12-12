#include <ctime>
#include "aocli.hh"

void getCurrentYearAndDay(int &year, int &day) {
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);

    year = now->tm_year + 1900;

    // If it's December, use current day (1-25)
    if ((now->tm_mon + 1) == 12 && now->tm_mday <= 25) {
        day = now->tm_mday;
    } else {
        // If not December or after day 25, default to day 1
        day = 1;
    }
}

bool isProblemAvailable(int year, int day) {
    // Basic validation
    if (day < 1 || day > 25) return false;
    if (year < 2015) return false;

    // Get current time
    std::time_t now = std::time(nullptr);
    std::tm* tm_now = std::localtime(&now);
    int current_year = tm_now->tm_year + 1900;

    // Past years are always available
    if (year < current_year) return true;

    // Future years are never available
    if (year > current_year) return false;

    // For current year, check if puzzle has been released
    if ((tm_now->tm_mon + 1) == 12 && tm_now->tm_mday >= day) {
        return true;
    }

    return false;
}
