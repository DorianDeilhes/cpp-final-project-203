#ifndef BERMUDANOPTION_H
#define BERMUDANOPTION_H

#include <vector>
#include <algorithm>

// Option type enumeration
enum OptionType {
    CALL,
    PUT
};

// Bermudan option: can be exercised at discrete dates before maturity
class BermudanOption {
private:
    double strike;                          // Strike price K
    std::vector<double> exerciseDates;      // Exercise times
    OptionType optionType;                  // CALL or PUT
    
public:
    // Constructor
    BermudanOption(double strike, const std::vector<double>& dates, OptionType type);
    
    // Destructor
    ~BermudanOption();
    
    // Calculate payoff at given spot price
    // Call: max(spot - K, 0)
    // Put: max(K - spot, 0)
    double payoff(double spot) const;
    
    // Return vector of exercise times
    std::vector<double> getExerciseDates() const;
    
    // Getters
    double getStrike() const { return strike; }
    int getNExerciseDates() const { return exerciseDates.size(); }
    double getExerciseDate(int index) const { return exerciseDates[index]; }
    OptionType getOptionType() const { return optionType; }
};

#endif
