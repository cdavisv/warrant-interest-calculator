#ifndef CURRENCY_H
#define CURRENCY_H

#include <string>
#include <cstdint>
#include <optional>

namespace Currency {

// Parse a dollar amount string (e.g., "1234.56" or "1234") into cents.
// Returns nullopt if the string is not a valid amount.
std::optional<int64_t> parseDollars(const std::wstring& input);

// Format cents as a dollar string with commas and 2 decimal places.
// e.g., 123456 -> "$1,234.56"
std::wstring formatCents(int64_t cents);

} // namespace Currency

#endif
