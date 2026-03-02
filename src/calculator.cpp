#include "calculator.h"
#include <stdexcept>
#include <cmath>

InterestCalculator::Result InterestCalculator::compute(
    int64_t principalCents, double ratePercent, long long days)
{
    if (principalCents <= 0)
        throw std::invalid_argument("Warrant amount must be greater than zero.");
    if (ratePercent <= 0.0 || ratePercent > 20.0)
        throw std::invalid_argument(
            "Interest rate must be greater than 0 and at most 20 percent.");
    if (days < 0)
        throw std::invalid_argument("End date must be after start date.");

    double principal = static_cast<double>(principalCents) / 100.0;
    double rate = ratePercent / 100.0;
    double interest = principal * rate * (static_cast<double>(days) / 365.0);

    int64_t interestCents = static_cast<int64_t>(std::round(interest * 100.0));
    int64_t totalCents = principalCents + interestCents;

    return Result{days, interestCents, totalCents};
}
