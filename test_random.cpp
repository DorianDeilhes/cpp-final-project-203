#include <iostream>
#include <iomanip>
#include <cmath>
#include "RandomGenerator.h"

using namespace std;

int main() {
    cout << "========================================" << endl;
    cout << "Random Number Generator Test" << endl;
    cout << "========================================" << endl << endl;
    
    RandomGenerator rng(12345);  // Fixed seed for reproducibility
    
    // Test 1: Uniform distribution
    cout << "Test 1: Uniform Random Numbers" << endl;
    cout << "Generating 10 samples from U(0,1):" << endl;
    for (int i = 0; i < 10; i++) {
        cout << "  " << rng.generateUniform() << endl;
    }
    cout << endl;
    
    // Test 2: Normal distribution statistics
    cout << "Test 2: Normal Distribution Statistics" << endl;
    int nSamples = 10000;
    double sum = 0.0;
    double sumSquared = 0.0;
    
    for (int i = 0; i < nSamples; i++) {
        double z = rng.generateNormal();
        sum += z;
        sumSquared += z * z;
    }
    
    double mean = sum / nSamples;
    double variance = (sumSquared / nSamples) - (mean * mean);
    double stdDev = sqrt(variance);
    
    cout << "Generated " << nSamples << " samples" << endl;
    cout << fixed << setprecision(4);
    cout << "Mean (should be ~0.0): " << mean << endl;
    cout << "Std Dev (should be ~1.0): " << stdDev << endl;
    
    bool meanOK = (fabs(mean) < 0.05);
    bool stdOK = (fabs(stdDev - 1.0) < 0.05);
    
    cout << "Mean test: " << (meanOK ? "PASS" : "FAIL") << endl;
    cout << "Std Dev test: " << (stdOK ? "PASS" : "FAIL") << endl;
    cout << endl;
    
    // Test 3: Correlated normals
    cout << "Test 3: Correlated Normal Variables" << endl;
    double rho = 0.7;  // Target correlation
    nSamples = 10000;
    
    double sumZ1 = 0.0, sumZ2 = 0.0, sumZ1Z2 = 0.0;
    double sumZ1Sq = 0.0, sumZ2Sq = 0.0;
    
    for (int i = 0; i < nSamples; i++) {
        double Z1, Z2;
        rng.generateCorrelatedNormals(rho, Z1, Z2);
        
        sumZ1 += Z1;
        sumZ2 += Z2;
        sumZ1Z2 += Z1 * Z2;
        sumZ1Sq += Z1 * Z1;
        sumZ2Sq += Z2 * Z2;
    }
    
    double meanZ1 = sumZ1 / nSamples;
    double meanZ2 = sumZ2 / nSamples;
    double covariance = (sumZ1Z2 / nSamples) - (meanZ1 * meanZ2);
    double varZ1 = (sumZ1Sq / nSamples) - (meanZ1 * meanZ1);
    double varZ2 = (sumZ2Sq / nSamples) - (meanZ2 * meanZ2);
    double correlation = covariance / (sqrt(varZ1) * sqrt(varZ2));
    
    cout << "Target correlation rho: " << rho << endl;
    cout << "Measured correlation: " << correlation << endl;
    
    bool corrOK = (fabs(correlation - rho) < 0.05);
    cout << "Correlation test: " << (corrOK ? "PASS" : "FAIL") << endl;
    cout << endl;
    
    cout << "========================================" << endl;
    cout << "All tests completed!" << endl;
    cout << "========================================" << endl;
    
    return 0;
}
