#ifndef SABRSIMULATOR_H
#define SABRSIMULATOR_H

#include "RandomGenerator.h"
#include <cmath>

// SABR Model: Stochastic Alpha Beta Rho
// dF_t = alpha_t * F_t^beta * dW1
// dalpha_t = nu * alpha_t * dW2
// where dW1 and dW2 have correlation rho

class SABRSimulator {
private:
    double F0;        // Initial forward rate
    double alpha0;    // Initial volatility
    double beta;      // Backbone parameter [0, 1]
    double nu;        // Vol-of-vol (volatility of volatility)
    double rho;       // Correlation between Brownian motions
    RandomGenerator* rng;  // Pointer to random generator
    
public:
    // Constructor
    SABRSimulator(double F0, double alpha0, double beta, double nu, double rho);
    
    // Destructor
    ~SABRSimulator();
    
    // Simulate single path from 0 to T with nSteps
    // Stores results in arrays F_path and alpha_path (must be pre-allocated)
    void simulatePath(int nSteps, double T, double* F_path, double* alpha_path);
    
    // Simulate multiple paths
    // F_paths[i][j] = forward rate of path i at step j
    // alpha_paths[i][j] = volatility of path i at step j
    void simulatePaths(int nPaths, int nSteps, double T, 
                      double** F_paths, double** alpha_paths);
    
    // Get parameters
    double getF0() const { return F0; }
    double getAlpha0() const { return alpha0; }
    double getBeta() const { return beta; }
    double getNu() const { return nu; }
    double getRho() const { return rho; }
};

#endif
