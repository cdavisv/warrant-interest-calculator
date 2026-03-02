#ifndef DATE_UTILS_H
#define DATE_UTILS_H

#include <string>
#include <optional>

namespace DateUtils {

struct Date {
    int year;
    int month;
    int day;
};

// Parse "MM-DD-YYYY" string into Date. Returns nullopt on failure.
std::optional<Date> parseDate(const std::wstring& dateStr);

// Validate that a Date represents a real calendar date.
bool isValidDate(const Date& d);

// Is the given year a leap year?
bool isLeapYear(int year);

// Count days in a given month/year (handles leap years).
int daysInMonth(int month, int year);

// Calculate the number of days between two dates.
// Returns negative if end is before start.
long long daysBetween(const Date& start, const Date& end);

} // namespace DateUtils

#endif
