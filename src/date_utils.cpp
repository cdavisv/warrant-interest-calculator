#include "date_utils.h"
#include <chrono>

namespace DateUtils {

bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int daysInMonth(int month, int year) {
    static const int days[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month < 1 || month > 12) return 0;
    if (month == 2 && isLeapYear(year)) return 29;
    return days[month];
}

bool isValidDate(const Date& d) {
    if (d.year < 1900 || d.year > 2100) return false;
    if (d.month < 1 || d.month > 12) return false;
    if (d.day < 1 || d.day > daysInMonth(d.month, d.year)) return false;
    return true;
}

std::optional<Date> parseDate(const std::wstring& dateStr) {
    // Expected format: MM-DD-YYYY (exactly 10 characters)
    if (dateStr.length() != 10) return std::nullopt;
    if (dateStr[2] != L'-' || dateStr[5] != L'-') return std::nullopt;

    // Verify all other characters are digits
    for (int i = 0; i < 10; ++i) {
        if (i == 2 || i == 5) continue;
        if (dateStr[i] < L'0' || dateStr[i] > L'9') return std::nullopt;
    }

    try {
        int month = std::stoi(dateStr.substr(0, 2));
        int day = std::stoi(dateStr.substr(3, 2));
        int year = std::stoi(dateStr.substr(6, 4));

        Date d{year, month, day};
        if (!isValidDate(d)) return std::nullopt;
        return d;
    } catch (...) {
        return std::nullopt;
    }
}

long long daysBetween(const Date& start, const Date& end) {
    using namespace std::chrono;
    auto startDays = sys_days{
        year{start.year} / month{static_cast<unsigned>(start.month)} /
        day{static_cast<unsigned>(start.day)}};
    auto endDays = sys_days{
        year{end.year} / month{static_cast<unsigned>(end.month)} /
        day{static_cast<unsigned>(end.day)}};
    return (endDays - startDays).count();
}

} // namespace DateUtils
