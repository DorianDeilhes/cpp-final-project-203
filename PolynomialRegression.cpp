#include "PolynomialRegression.h"
#include <cmath>
#include <algorithm>

// Constructor
PolynomialRegression::PolynomialRegression(int degree) {
    this->degree = degree;
    coefficients = new double[degree + 1];
    for (int i = 0; i <= degree; i++) {
        coefficients[i] = 0.0;
    }
}

// Destructor
PolynomialRegression::~PolynomialRegression() {
    delete[] coefficients;
}

// Compute basis functions: [1, F, F^2, ..., F^degree]
void PolynomialRegression::computeBasis(double F, double* basis) {
    basis[0] = 1.0;
    for (int i = 1; i <= degree; i++) {
        basis[i] = basis[i-1] * F;  // F^i = F * F^{i-1}
    }
}

// Gaussian elimination with partial pivoting to solve Ax = b
void PolynomialRegression::gaussianElimination(double** A, double* b, double* x, int n) {
    // Create augmented matrix [A|b]
    double** aug = new double*[n];
    for (int i = 0; i < n; i++) {
        aug[i] = new double[n + 1];
        for (int j = 0; j < n; j++) {
            aug[i][j] = A[i][j];
        }
        aug[i][n] = b[i];
    }
    
    // Forward elimination
    for (int k = 0; k < n; k++) {
        // Find pivot
        int maxRow = k;
        double maxVal = fabs(aug[k][k]);
        for (int i = k + 1; i < n; i++) {
            if (fabs(aug[i][k]) > maxVal) {
                maxVal = fabs(aug[i][k]);
                maxRow = i;
            }
        }
        
        // Swap rows
        if (maxRow != k) {
            double* temp = aug[k];
            aug[k] = aug[maxRow];
            aug[maxRow] = temp;
        }
        
        // Eliminate column
        for (int i = k + 1; i < n; i++) {
            if (fabs(aug[k][k]) > 1e-10) {
                double factor = aug[i][k] / aug[k][k];
                for (int j = k; j <= n; j++) {
                    aug[i][j] -= factor * aug[k][j];
                }
            }
        }
    }
    
    // Back substitution
    for (int i = n - 1; i >= 0; i--) {
        x[i] = aug[i][n];
        for (int j = i + 1; j < n; j++) {
            x[i] -= aug[i][j] * x[j];
        }
        if (fabs(aug[i][i]) > 1e-10) {
            x[i] /= aug[i][i];
        } else {
            x[i] = 0.0;  // Singular matrix, set to zero
        }
    }
    
    // Clean up
    for (int i = 0; i < n; i++) {
        delete[] aug[i];
    }
    delete[] aug;
}

// Fit polynomial using least squares
void PolynomialRegression::fit(double* F_values, double* C_values, int nPoints) {
    int p = degree + 1;  // Number of coefficients
    
    // Build design matrix X: X[i][j] = F_i^j
    double** X = new double*[nPoints];
    for (int i = 0; i < nPoints; i++) {
        X[i] = new double[p];
        computeBasis(F_values[i], X[i]);
    }
    
    // Compute X^T X (p x p matrix)
    double** XTX = new double*[p];
    for (int i = 0; i < p; i++) {
        XTX[i] = new double[p];
        for (int j = 0; j < p; j++) {
            XTX[i][j] = 0.0;
            for (int k = 0; k < nPoints; k++) {
                XTX[i][j] += X[k][i] * X[k][j];
            }
        }
    }
    
    // Compute X^T C (p x 1 vector)
    double* XTC = new double[p];
    for (int i = 0; i < p; i++) {
        XTC[i] = 0.0;
        for (int k = 0; k < nPoints; k++) {
            XTC[i] += X[k][i] * C_values[k];
        }
    }
    
    // Solve normal equations: XTX * a = XTC
    gaussianElimination(XTX, XTC, coefficients, p);
    
    // Clean up
    for (int i = 0; i < nPoints; i++) {
        delete[] X[i];
    }
    delete[] X;
    
    for (int i = 0; i < p; i++) {
        delete[] XTX[i];
    }
    delete[] XTX;
    
    delete[] XTC;
}

// Predict value at F
double PolynomialRegression::predict(double F) {
    double* basis = new double[degree + 1];
    computeBasis(F, basis);
    
    double result = 0.0;
    for (int i = 0; i <= degree; i++) {
        result += coefficients[i] * basis[i];
    }
    
    delete[] basis;
    return result;
}
