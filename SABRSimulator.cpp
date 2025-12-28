#include "SABRSimulator.h"
#include <algorithm>

// Constructor
SABRSimulator::SABRSimulator(double F0, double alpha0, double beta, double nu, double rho) {
    this->F0 = F0;
    this->alpha0 = alpha0;
    this->beta = beta;
    this->nu = nu;
    this->rho = rho;
    this->rng = new RandomGenerator();
}

// Destructor
SABRSimulator::~SABRSimulator() {
    delete rng;
}

// Simulate single path using Euler-Maruyama discretization
void SABRSimulator::simulatePath(int nSteps, double T, double* F_path, double* alpha_path) {
    double dt = T / static_cast<double>(nSteps);
    double sqrt_dt = sqrt(dt);
    
    // Initialize
    F_path[0] = F0;
    alpha_path[0] = alpha0;
    
    // Euler-Maruyama scheme
    for (int i = 0; i < nSteps; i++) {
        double Z1, Z2;
        rng->generateCorrelatedNormals(rho, Z1, Z2);
        
        double F_current = F_path[i];
        double alpha_current = alpha_path[i];
        
        // Discretization formulas:
        // F_{n+1} = F_n + alpha_n * F_n^beta * sqrt(dt) * Z1
        // alpha_{n+1} = alpha_n + nu * alpha_n * sqrt(dt) * Z2
        
        double F_next = F_current + alpha_current * pow(F_current, beta) * sqrt_dt * Z1;
        double alpha_next = alpha_current + nu * alpha_current * sqrt_dt * Z2;
        
        // Ensure positivity (truncate at small positive value)
        F_next = std::max(F_next, 0.001);
        alpha_next = std::max(alpha_next, 0.001);
        
        F_path[i + 1] = F_next;
        alpha_path[i + 1] = alpha_next;
    }
}

// Simulate multiple paths
void SABRSimulator::simulatePaths(int nPaths, int nSteps, double T, 
                                 double** F_paths, double** alpha_paths) {
    for (int i = 0; i < nPaths; i++) {
        simulatePath(nSteps, T, F_paths[i], alpha_paths[i]);
    }
}
