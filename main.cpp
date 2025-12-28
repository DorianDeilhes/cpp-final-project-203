#include <iostream>
#include <iomanip>
#include "SABRSimulator.h"
#include "BermudanOption.h"
#include "LSMPricer.h"

using namespace std;

int main() {
    cout << "========================================" << endl;
    cout << "SABR Bermudan Option Pricing" << endl;
    cout << "Longstaff-Schwartz Monte Carlo Method" << endl;
    cout << "========================================" << endl << endl;
    
    // SABR model parameters (from Table 1)
    double F0 = 100.0;        // Initial forward rate
    double alpha0 = 0.20;     // Initial volatility
    double beta = 0.5;        // Beta parameter (CIR-type)
    double nu = 0.4;          // Vol-of-vol
    double rho = -0.3;        // Correlation
    
    // Option parameters
    double K = 100.0;         // Strike price (ATM)
    double r = 0.05;          // Risk-free rate
    
    // Exercise dates: quarterly from 0.25 to 1.0 year
    std::vector<double> exerciseDates = {0.25, 0.5, 0.75, 1.0};
    
    // Pricing parameters
    int nPaths = 10000;       // Number of Monte Carlo paths
    int stepsPerPeriod = 25;  // Time steps between exercise dates
    int polyDegree = 3;       // Polynomial degree for regression
    
    cout << "Model Parameters:" << endl;
    cout << "  F0 = " << F0 << endl;
    cout << "  alpha0 = " << alpha0 << endl;
    cout << "  beta = " << beta << endl;
    cout << "  nu = " << nu << endl;
    cout << "  rho = " << rho << endl << endl;
    
    cout << "Option Parameters:" << endl;
    cout << "  Strike K = " << K << endl;
    cout << "  Risk-free rate r = " << r << endl;
    cout << "  Exercise dates: ";
    for (size_t i = 0; i < exerciseDates.size(); i++) {
        cout << exerciseDates[i];
        if (i < exerciseDates.size() - 1) cout << ", ";
    }
    cout << endl << endl;
    
    cout << "Pricing Parameters:" << endl;
    cout << "  Number of paths = " << nPaths << endl;
    cout << "  Steps per period = " << stepsPerPeriod << endl;
    cout << "  Polynomial degree = " << polyDegree << endl << endl;
    
    // Create SABR simulator
    SABRSimulator simulator(F0, alpha0, beta, nu, rho);
    
    // Create Bermudan call option
    BermudanOption option(K, exerciseDates, CALL);
    
    // Create pricer
    LSMPricer pricer(r, polyDegree);
    
    // Price the option
    cout << "Pricing..." << endl << endl;
    double price = pricer.price(simulator, option, nPaths);
    double stdError = pricer.getStandardError();
    
    cout << "========================================" << endl;
    cout << "RESULTS" << endl;
    cout << "========================================" << endl;
    cout << fixed << setprecision(4);
    cout << "Option Price: " << price << endl;
    cout << "Standard Error: " << stdError << endl;
    cout << "95% Confidence Interval: [" 
         << (price - 1.96 * stdError) << ", " 
         << (price + 1.96 * stdError) << "]" << endl;
    cout << "========================================" << endl << endl;
    
    // Convergence test with different number of paths
    cout << "Convergence Analysis:" << endl;
    cout << "N Paths\t\tPrice\t\tStd Error" << endl;
    cout << "-------\t\t-----\t\t---------" << endl;
    
    int pathCounts[] = {1000, 5000, 10000, 50000};
    for (int i = 0; i < 4; i++) {
        LSMPricer testPricer(r, polyDegree);
        double testPrice = testPricer.price(simulator, option, pathCounts[i]);
        double testStdErr = testPricer.getStandardError();
        
        cout << pathCounts[i] << "\t\t" 
             << setprecision(4) << testPrice << "\t\t" 
             << setprecision(4) << testStdErr << endl;
    }
    
    cout << endl << "Pricing complete!" << endl;
    
    return 0;
}
