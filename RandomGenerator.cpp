#include "RandomGenerator.h"

const double PI = 3.14159265358979323846;

// Default constructor: seed with current time
RandomGenerator::RandomGenerator() {
    seed = static_cast<unsigned int>(time(0));
    srand(seed);
    hasSpare = false;
    spare = 0.0;
}

// Constructor with custom seed
RandomGenerator::RandomGenerator(unsigned int customSeed) {
    seed = customSeed;
    srand(seed);
    hasSpare = false;
    spare = 0.0;
}

// Destructor
RandomGenerator::~RandomGenerator() {
    // Nothing to clean up
}

// Generate uniform random number in [0, 1]
double RandomGenerator::generateUniform() {
    return static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
}

// Box-Muller transform to generate standard normal
// Generates two independent normals, caches one for efficiency
double RandomGenerator::generateNormal() {
    if (hasSpare) {
        hasSpare = false;
        return spare;
    }
    
    hasSpare = true;
    
    // Generate two uniform random numbers
    double u1, u2;
    do {
        u1 = generateUniform();
    } while (u1 == 0.0); // Avoid log(0)
    
    u2 = generateUniform();
    
    // Box-Muller transformation
    double r = sqrt(-2.0 * log(u1));
    double theta = 2.0 * PI * u2;
    
    spare = r * sin(theta);
    return r * cos(theta);
}

// Generate correlated pair (Z1, Z2) with correlation rho
// Formula: Z2 = rho * Z1 + sqrt(1-rho^2) * Z3
void RandomGenerator::generateCorrelatedNormals(double rho, double& Z1, double& Z2) {
    Z1 = generateNormal();
    double Z3 = generateNormal();
    
    Z2 = rho * Z1 + sqrt(1.0 - rho * rho) * Z3;
}
