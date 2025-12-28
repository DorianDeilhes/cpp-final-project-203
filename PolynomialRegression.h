#ifndef POLYNOMIALREGRESSION_H
#define POLYNOMIALREGRESSION_H

// Simple polynomial regression for Longstaff-Schwartz
// Fits: C(F) ≈ a0 + a1*F + a2*F^2 + ... + a_degree*F^degree
// Uses least squares: minimize ||X*a - C||^2
// Solution: a = (X^T X)^{-1} X^T C

class PolynomialRegression {
private:
    int degree;              // Polynomial degree (e.g., 3)
    double* coefficients;    // Coefficients a0, a1, ..., a_degree
    
    // Helper: compute basis functions [1, F, F^2, ..., F^degree]
    void computeBasis(double F, double* basis);
    
    // Matrix operations for solving normal equations
    void matrixMultiply(double** A, double** B, double** result, int m, int n, int p);
    void matrixTranspose(double** A, double** AT, int rows, int cols);
    void vectorMatrixMultiply(double** A, double* b, double* result, int rows, int cols);
    
    // Gaussian elimination to solve linear system Ax = b
    void gaussianElimination(double** A, double* b, double* x, int n);
    
public:
    // Constructor
    PolynomialRegression(int degree);
    
    // Destructor
    ~PolynomialRegression();
    
    // Fit polynomial to data points (F_values, C_values)
    // nPoints = number of data points
    void fit(double* F_values, double* C_values, int nPoints);
    
    // Predict continuation value at given F
    double predict(double F);
    
    // Get coefficient
    double getCoefficient(int i) const { return coefficients[i]; }
};

#endif
