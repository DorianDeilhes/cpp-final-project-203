#ifndef PRICINGRESULTS_H
#define PRICINGRESULTS_H

#include <vector>
#include <string>

// Storage for pricing results and diagnostics
class PricingResults {
private:
    double optionPrice;
    double standardError;
    std::vector<double> convergencePrices;    // Prices at different N
    std::vector<double> convergenceErrors;     // Std errors at different N
    std::vector<int> convergenceNPaths;        // N values tested
    std::vector<double> exerciseBoundary;      // Exercise boundary (optional)
    
public:
    // Constructor
    PricingResults();
    
    // Destructor
    ~PricingResults();
    
    // Set main results
    void setPrice(double price);
    void setStandardError(double stdErr);
    
    // Add convergence data point
    void addConvergencePoint(int nPaths, double price, double stdErr);
    
    // Set exercise boundary
    void setExerciseBoundary(const std::vector<double>& boundary);
    
    // Getters
    double getPrice() const { return optionPrice; }
    double getStandardError() const { return standardError; }
    std::vector<double> getConvergencePrices() const { return convergencePrices; }
    std::vector<double> getConvergenceErrors() const { return convergenceErrors; }
    std::vector<int> getConvergenceNPaths() const { return convergenceNPaths; }
    
    // Output methods
    void display() const;
    void saveToFile(const std::string& filename) const;
    std::string getSummary() const;
};

#endif
