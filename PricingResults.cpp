#include "PricingResults.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

// Constructor
PricingResults::PricingResults() {
    optionPrice = 0.0;
    standardError = 0.0;
}

// Destructor
PricingResults::~PricingResults() {
    // Vectors handle their own cleanup
}

// Set main results
void PricingResults::setPrice(double price) {
    optionPrice = price;
}

void PricingResults::setStandardError(double stdErr) {
    standardError = stdErr;
}

// Add convergence data point
void PricingResults::addConvergencePoint(int nPaths, double price, double stdErr) {
    convergenceNPaths.push_back(nPaths);
    convergencePrices.push_back(price);
    convergenceErrors.push_back(stdErr);
}

// Set exercise boundary
void PricingResults::setExerciseBoundary(const std::vector<double>& boundary) {
    exerciseBoundary = boundary;
}

// Display results to console
void PricingResults::display() const {
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "========================================" << std::endl;
    std::cout << "PRICING RESULTS" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Option Price: " << optionPrice << std::endl;
    std::cout << "Standard Error: " << standardError << std::endl;
    std::cout << "95% Confidence Interval: [" 
              << (optionPrice - 1.96 * standardError) << ", "
              << (optionPrice + 1.96 * standardError) << "]" << std::endl;
    
    if (!convergenceNPaths.empty()) {
        std::cout << "\nConvergence Analysis:" << std::endl;
        std::cout << "N Paths\t\tPrice\t\tStd Error" << std::endl;
        std::cout << "-------\t\t-----\t\t---------" << std::endl;
        for (size_t i = 0; i < convergenceNPaths.size(); i++) {
            std::cout << convergenceNPaths[i] << "\t\t"
                     << convergencePrices[i] << "\t\t"
                     << convergenceErrors[i] << std::endl;
        }
    }
    std::cout << "========================================" << std::endl;
}

// Save to file
void PricingResults::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    file << std::fixed << std::setprecision(4);
    file << "PRICING RESULTS" << std::endl;
    file << "===============" << std::endl;
    file << "Price: " << optionPrice << std::endl;
    file << "Standard Error: " << standardError << std::endl;
    
    if (!convergenceNPaths.empty()) {
        file << "\nCONVERGENCE DATA" << std::endl;
        file << "NPaths\tPrice\tStdError" << std::endl;
        for (size_t i = 0; i < convergenceNPaths.size(); i++) {
            file << convergenceNPaths[i] << "\t"
                 << convergencePrices[i] << "\t"
                 << convergenceErrors[i] << std::endl;
        }
    }
    file.close();
}

// Get summary string
std::string PricingResults::getSummary() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(4);
    oss << "Price: " << optionPrice << ", StdErr: " << standardError;
    return oss.str();
}
