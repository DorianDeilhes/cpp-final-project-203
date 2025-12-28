#ifndef LSMPRICER_H
#define LSMPRICER_H

#include "SABRSimulator.h"
#include "BermudanOption.h"
#include "PolynomialRegression.h"
#include "PricingResults.h"
#include <vector>
#include <cmath>

// Longstaff-Schwartz Monte Carlo pricer for Bermudan options
class LSMPricer {
private:
    double discountRate;     // Risk-free rate r
    int polynomialDegree;    // Degree for regression (default 3)
    double standardError;    // Standard error of last pricing
    
    // Helper: discount factor from t to t+dt
    double discountFactor(double dt) {
        return exp(-discountRate * dt);
    }
    
public:
    // Constructor
    LSMPricer(double r = 0.05, int polyDegree = 3);
    
    // Destructor
    ~LSMPricer();
    
    // Main pricing function (SPEC COMPLIANT SIGNATURE)
    double price(SABRSimulator& sim, BermudanOption& option, int nPaths);
    
    // Regression fit - least squares on (X, Y) data
    std::vector<double> regressionFit(const std::vector<double>& X, const std::vector<double>& Y);
    
    // Compute basis function values at given state
    std::vector<double> basisFunctions(double F, double alpha);
    
    // Get standard error from last pricing
    double getStandardError() const { return standardError; }
    
    // Set parameters
    void setDiscountRate(double r) { discountRate = r; }
    void setPolynomialDegree(int deg) { polynomialDegree = deg; }
};

#endif
