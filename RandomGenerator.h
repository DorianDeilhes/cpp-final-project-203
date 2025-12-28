#ifndef RANDOMGENERATOR_H
#define RANDOMGENERATOR_H

#include <cstdlib>
#include <cmath>
#include <ctime>

// Random number generator for Monte Carlo simulation
// Implements Box-Muller algorithm for normal random variables
class RandomGenerator {
private:
    unsigned int seed;
    bool hasSpare;         // For Box-Muller optimization
    double spare;          // Cached normal random variable
    
public:
    // Constructor: initializes with time-based seed
    RandomGenerator();
    
    // Constructor with custom seed (for reproducible results)
    RandomGenerator(unsigned int customSeed);
    
    // Destructor
    ~RandomGenerator();
    
    // Generate uniform random number in [0, 1]
    double generateUniform();
    
    // Generate standard normal random variable (mean=0, std=1)
    // Uses Box-Muller transformation
    double generateNormal();
    
    // Generate correlated pair of normal random variables
    // Z2 = rho * Z1 + sqrt(1-rho^2) * Z3
    // where Z1, Z3 are independent standard normals
    void generateCorrelatedNormals(double rho, double& Z1, double& Z2);
};

#endif
