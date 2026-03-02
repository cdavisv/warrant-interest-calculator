#include "currency.h"
#include <cwchar>

namespace Currency {

std::optional<int64_t> parseDollars(const std::wstring& input) {
    if (input.empty()) return std::nullopt;

    // Strip leading/trailing whitespace
    size_t start = input.find_first_not_of(L" \t");
    size_t end = input.find_last_not_of(L" \t");
    if (start == std::wstring::npos) return std::nullopt;
    std::wstring s = input.substr(start, end - start + 1);

    // Strip optional leading '$'
    if (!s.empty() && s[0] == L'$') s = s.substr(1);
    if (s.empty()) return std::nullopt;

    // Find decimal point
    auto dotPos = s.find(L'.');

    std::wstring wholePart, fracPart;
    if (dotPos == std::wstring::npos) {
        wholePart = s;
        fracPart = L"00";
    } else {
        wholePart = s.substr(0, dotPos);
        fracPart = s.substr(dotPos + 1);
    }

    // Validate: fracPart must be 1 or 2 digits
    if (fracPart.length() == 1) fracPart += L'0';
    if (fracPart.length() != 2) return std::nullopt;

    // Validate all characters are digits
    if (wholePart.empty()) return std::nullopt;
    for (wchar_t c : wholePart) {
        if (c < L'0' || c > L'9') return std::nullopt;
    }
    for (wchar_t c : fracPart) {
        if (c < L'0' || c > L'9') return std::nullopt;
    }

    try {
        int64_t dollars = std::stoll(wholePart);
        int64_t cents = std::stoi(std::wstring(fracPart.begin(), fracPart.end()));
        return dollars * 100 + cents;
    } catch (...) {
        return std::nullopt;
    }
}

std::wstring formatCents(int64_t cents) {
    bool negative = cents < 0;
    if (negative) cents = -cents;

    int64_t dollars = cents / 100;
    int64_t remainder = cents % 100;

    // Format dollars with comma separators
    std::wstring dollarStr = std::to_wstring(dollars);
    std::wstring withCommas;
    int count = 0;
    for (int i = static_cast<int>(dollarStr.length()) - 1; i >= 0; --i) {
        if (count > 0 && count % 3 == 0) withCommas = L',' + withCommas;
        withCommas = dollarStr[i] + withCommas;
        ++count;
    }

    // Format cents with leading zero
    wchar_t centsBuf[4];
    swprintf(centsBuf, 4, L"%02lld", static_cast<long long>(remainder));

    std::wstring result = L"$" + withCommas + L"." + centsBuf;
    if (negative) result = L"-" + result;
    return result;
}

} // namespace Currency
