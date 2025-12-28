#include "LSMPricer.h"
#include <iostream>
#include <algorithm>

// Constructor
LSMPricer::LSMPricer(double r, int polyDegree) {
    this->discountRate = r;
    this->polynomialDegree = polyDegree;
    this->standardError = 0.0;
}

// Destructor
LSMPricer::~LSMPricer() {
    // Nothing to clean up
}

// Regression fit - public method for least squares
std::vector<double> LSMPricer::regressionFit(const std::vector<double>& X, const std::vector<double>& Y) {
    PolynomialRegression reg(polynomialDegree);
    
    // Convert vectors to arrays for PolynomialRegression
    int n = X.size();
    double* X_arr = new double[n];
    double* Y_arr = new double[n];
    
    for (int i = 0; i < n; i++) {
        X_arr[i] = X[i];
        Y_arr[i] = Y[i];
    }
    
    reg.fit(X_arr, Y_arr, n);
    
    // Extract coefficients
    std::vector<double> coeffs(polynomialDegree + 1);
    for (int i = 0; i <= polynomialDegree; i++) {
        coeffs[i] = reg.getCoefficient(i);
    }
    
    delete[] X_arr;
    delete[] Y_arr;
    
    return coeffs;
}

// Compute basis functions [1, F, F^2, ..., F^degree]
std::vector<double> LSMPricer::basisFunctions(double F, double alpha) {
    std::vector<double> basis(polynomialDegree + 1);
    basis[0] = 1.0;
    for (int i = 1; i <= polynomialDegree; i++) {
        basis[i] = basis[i-1] * F;  // Could also include alpha terms if needed
    }
    return basis;
}

// Main pricing function using Longstaff-Schwartz algorithm
// SPEC COMPLIANT: Takes SABRSimulator& and BermudanOption& as parameters
double LSMPricer::price(SABRSimulator& sim, BermudanOption& option, int nPaths) {
    int nExerciseDates = option.getNExerciseDates();
    double T = option.getExerciseDate(nExerciseDates - 1);
    
    // Determine time steps
    int stepsPerPeriod = 25;  // Default
    int totalSteps = (nExerciseDates - 1) * stepsPerPeriod;
    double dt = T / static_cast<double>(totalSteps);
    
    // Allocate path storage
    double** F_paths = new double*[nPaths];
    double** alpha_paths = new double*[nPaths];
    for (int i = 0; i < nPaths; i++) {
        F_paths[i] = new double[totalSteps + 1];
        alpha_paths[i] = new double[totalSteps + 1];
    }
    
    // Simulate all paths
    std::cout << "Simulating " << nPaths << " paths..." << std::endl;
    sim.simulatePaths(nPaths, totalSteps, T, F_paths, alpha_paths);
    
    // Map exercise dates to time step indices
    int* exerciseSteps = new int[nExerciseDates];
    for (int m = 0; m < nExerciseDates; m++) {
        exerciseSteps[m] = static_cast<int>(option.getExerciseDate(m) / dt + 0.5);
    }
    
    // Value array: V[i] = value of option for path i
    double* V = new double[nPaths];
    
    // Initialize at maturity (last exercise date)
    int lastStep = exerciseSteps[nExerciseDates - 1];
    for (int i = 0; i < nPaths; i++) {
        V[i] = option.payoff(F_paths[i][lastStep]);
    }
    
    std::cout << "Running backward induction..." << std::endl;
    
    // Backward induction through exercise dates
    for (int m = nExerciseDates - 2; m >= 0; m--) {
        int currentStep = exerciseSteps[m];
        int nextStep = exerciseSteps[m + 1];
        double discountToNext = discountFactor((nextStep - currentStep) * dt);
        
        // Identify in-the-money paths
        std::vector<double> F_itm;
        std::vector<double> C_itm;
        std::vector<int> itm_indices;
        
        for (int i = 0; i < nPaths; i++) {
            double payoffNow = option.payoff(F_paths[i][currentStep]);
            if (payoffNow > 0.0) {
                F_itm.push_back(F_paths[i][currentStep]);
                C_itm.push_back(V[i] * discountToNext);
                itm_indices.push_back(i);
            }
        }
        
        if (F_itm.empty()) {
            // No paths in the money, just discount
            for (int i = 0; i < nPaths; i++) {
                V[i] *= discountToNext;
            }
            continue;
        }
        
        // Fit regression to continuation values using public method
        std::vector<double> coeffs = regressionFit(F_itm, C_itm);
        
        // Exercise decision for each path
        for (int i = 0; i < nPaths; i++) {
            double immediatePayoff = option.payoff(F_paths[i][currentStep]);
            
            if (immediatePayoff > 0.0) {
                // Predict continuation value
                std::vector<double> basis = basisFunctions(F_paths[i][currentStep], alpha_paths[i][currentStep]);
                double continuationValue = 0.0;
                for (size_t j = 0; j < coeffs.size(); j++) {
                    continuationValue += coeffs[j] * basis[j];
                }
                
                if (immediatePayoff > continuationValue) {
                    V[i] = immediatePayoff;  // Exercise
                } else {
                    V[i] *= discountToNext;  // Continue
                }
            } else {
                V[i] *= discountToNext;
            }
        }
    }
    
    // Discount from first exercise date to t=0
    double discountToZero = discountFactor(option.getExerciseDate(0));
    
    // Compute price and standard error
    double sum = 0.0;
    double sumSquared = 0.0;
    for (int i = 0; i < nPaths; i++) {
        double discounted = V[i] * discountToZero;
        sum += discounted;
        sumSquared += discounted * discounted;
    }
    
    double optionPrice = sum / static_cast<double>(nPaths);
    double variance = (sumSquared / nPaths) - (optionPrice * optionPrice);
    standardError = sqrt(variance / nPaths);
    
    // Clean up
    for (int i = 0; i < nPaths; i++) {
        delete[] F_paths[i];
        delete[] alpha_paths[i];
    }
    delete[] F_paths;
    delete[] alpha_paths;
    delete[] V;
    delete[] exerciseSteps;
    
    return optionPrice;
}
