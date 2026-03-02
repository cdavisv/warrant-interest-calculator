#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <cstdint>

class InterestCalculator {
public:
    struct Result {
        long long days;
        int64_t interestCents;
        int64_t totalCents;
    };

    // Compute simple interest.
    // principalCents: principal amount in cents
    // ratePercent: annual interest rate as percentage (e.g. 6.0 for 6%)
    // days: number of days
    // Throws std::invalid_argument on invalid inputs.
    static Result compute(int64_t principalCents, double ratePercent, long long days);
};

#endif
