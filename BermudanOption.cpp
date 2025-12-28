#include "BermudanOption.h"

// Constructor
BermudanOption::BermudanOption(double strike, const std::vector<double>& dates, OptionType type) {
    this->strike = strike;
    this->exerciseDates = dates;  // Vector assignment (deep copy)
    this->optionType = type;
}

// Destructor
BermudanOption::~BermudanOption() {
    // Vector handles its own memory cleanup automatically
}

// Calculate payoff
double BermudanOption::payoff(double spot) const {
    if (optionType == CALL) {
        return std::max(spot - strike, 0.0);
    } else {  // PUT
        return std::max(strike - spot, 0.0);
    }
}

// Return vector of exercise dates
std::vector<double> BermudanOption::getExerciseDates() const {
    return exerciseDates;
}
