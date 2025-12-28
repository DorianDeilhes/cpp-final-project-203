#include <iostream>
#include <iomanip>
#include <fstream>
#include "SABRSimulator.h"
#include "BermudanOption.h"
#include "LSMPricer.h"

using namespace std;

int main() {
    cout << "========================================" << endl;
    cout << "SABR Parameter Sensitivity Analysis" << endl;
    cout << "========================================" << endl << endl;
    
    // Base parameters
    double F0 = 100.0;
    double alpha0 = 0.20;
    double K = 100.0;
    double r = 0.05;
    std::vector<double> exerciseDates = {0.25, 0.5, 0.75, 1.0};
    int nPaths = 10000;
    int stepsPerPeriod = 25;
    int polyDegree = 3;
    
    cout << fixed << setprecision(4);
    
    // Open file for results
    ofstream outfile("sensitivity_results.txt");
    outfile << fixed << setprecision(4);
    
    // Test 1: Beta sensitivity
    cout << "Test 1: Beta (backbone) Sensitivity" << endl;
    cout << "====================================" << endl;
    cout << "Beta\tPrice\t\tStd Error" << endl;
    cout << "----\t-----\t\t---------" << endl;
    
    outfile << "BETA SENSITIVITY" << endl;
    outfile << "Beta\tPrice\tStdError" << endl;
    
    double betas[] = {0.0, 0.3, 0.5, 0.7, 1.0};
    for (int i = 0; i < 5; i++) {
        double beta = betas[i];
        SABRSimulator sim(F0, alpha0, beta, 0.4, -0.3);
        BermudanOption opt(K, exerciseDates, CALL);
        LSMPricer pricer(r, polyDegree);
        
        double price = pricer.price(sim, opt, nPaths);
        double stdErr = pricer.getStandardError();
        
        cout << beta << "\t" << price << "\t\t" << stdErr << endl;
        outfile << beta << "\t" << price << "\t" << stdErr << endl;
    }
    cout << endl;
    outfile << endl;
    
    // Test 2: Nu (vol-of-vol) sensitivity
    cout << "Test 2: Nu (Vol-of-Vol) Sensitivity" << endl;
    cout << "====================================" << endl;
    cout << "Nu\tPrice\t\tStd Error" << endl;
    cout << "----\t-----\t\t---------" << endl;
    
    outfile << "NU SENSITIVITY" << endl;
    outfile << "Nu\tPrice\tStdError" << endl;
    
    double nus[] = {0.1, 0.2, 0.4, 0.6, 0.8};
    for (int i = 0; i < 5; i++) {
        double nu = nus[i];
        SABRSimulator sim(F0, alpha0, 0.5, nu, -0.3);
        BermudanOption opt(K, exerciseDates, CALL);
        LSMPricer pricer(r, polyDegree);
        
        double price = pricer.price(sim, opt, nPaths);
        double stdErr = pricer.getStandardError();
        
        cout << nu << "\t" << price << "\t\t" << stdErr << endl;
        outfile << nu << "\t" << price << "\t" << stdErr << endl;
    }
    cout << endl;
    outfile << endl;
    
    // Test 3: Rho (correlation) sensitivity
    cout << "Test 3: Rho (Correlation) Sensitivity" << endl;
    cout << "======================================" << endl;
    cout << "Rho\tPrice\t\tStd Error" << endl;
    cout << "----\t-----\t\t---------" << endl;
    
    outfile << "RHO SENSITIVITY" << endl;
    outfile << "Rho\tPrice\tStdError" << endl;
    
    double rhos[] = {-0.7, -0.3, 0.0, 0.3, 0.7};
    for (int i = 0; i < 5; i++) {
        double rho = rhos[i];
        SABRSimulator sim(F0, alpha0, 0.5, 0.4, rho);
        BermudanOption opt(K, exerciseDates, CALL);
        LSMPricer pricer(r, polyDegree);
        
        double price = pricer.price(sim, opt, nPaths);
        double stdErr = pricer.getStandardError();
        
        cout << rho << "\t" << price << "\t\t" << stdErr << endl;
        outfile << rho << "\t" << price << "\t" << stdErr << endl;
    }
    cout << endl;
    outfile << endl;
    
    // Test 4: Strike (moneyness) sensitivity
    cout << "Test 4: Strike (Moneyness) Sensitivity" << endl;
    cout << "=======================================" << endl;
    cout << "Strike\tMoneyness\tPrice\t\tStd Error" << endl;
    cout << "------\t---------\t-----\t\t---------" << endl;
    
    outfile << "STRIKE SENSITIVITY" << endl;
    outfile << "Strike\tMoneyness\tPrice\tStdError" << endl;
    
    double strikes[] = {90.0, 95.0, 100.0, 105.0, 110.0};
    const char* moneyness[] = {"ITM", "ITM", "ATM", "OTM", "OTM"};
    
    for (int i = 0; i < 5; i++) {
        double strike = strikes[i];
        SABRSimulator sim(F0, alpha0, 0.5, 0.4, -0.3);
        BermudanOption opt(strike, exerciseDates, CALL);
        LSMPricer pricer(r, polyDegree);
        
        double price = pricer.price(sim, opt, nPaths);
        double stdErr = pricer.getStandardError();
        
        cout << strike << "\t" << moneyness[i] << "\t\t" << price << "\t\t" << stdErr << endl;
        outfile << strike << "\t" << moneyness[i] << "\t" << price << "\t" << stdErr << endl;
    }
    cout << endl;
    outfile << endl;
    
    outfile.close();
    
    cout << "========================================" << endl;
    cout << "Analysis complete!" << endl;
    cout << "Results saved to sensitivity_results.txt" << endl;
    cout << "========================================" << endl;
    
    return 0;
}
