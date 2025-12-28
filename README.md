# SABR Bermudan Option Pricing Library

A C++ implementation of a Bermudan option pricer using the SABR (Stochastic Alpha Beta Rho) model and the Longstaff-Schwartz Monte Carlo algorithm.

## Mathematical Framework

### SABR Model

The SABR model describes the evolution of a forward rate F and its stochastic volatility α:

$$
dF_t = α_t F_t^β dW_t^F$$
$$dα_t = ν α_t dW_t^α$$


Where:
- **F**: Forward rate
- **α**: Stochastic volatility
- **β ∈ [0, 1]**: Backbone parameter (0=normal, 0.5=CIR, 1=lognormal)
- **ν > 0**: Volatility of volatility
- **ρ ∈ (-1, 1)**: Correlation between Brownian motions

### Euler-Maruyama Discretization

For time step Δt:


$$F_{n+1} = F_n + α_n F_n^β √Δt Z_1$$
$$α_{n+1} = α_n + ν α_n √Δt Z_2$$


Where $Z_1$, $Z_2$ are correlated standard normals with correlation ρ.

### Longstaff-Schwartz Algorithm

The algorithm prices Bermudan options using backward induction:

1. **Simulate paths**: Generate N Monte Carlo paths of $(F, α)$
2. **Initialize at maturity**: $V[path] = max(F_T - K, 0)$
3. **Backward induction**: At each exercise date:
   - Identify in-the-money paths
   - Fit polynomial regression: Continuation value ~ $a_0 + a_1 F + a_2 F² + a_3 F³$
   - Compare immediate exercise vs continuation
   - Update path values
4. **Discount to present**: Price = average of discounted values

## Project Structure

```
big_project/
├── RandomGenerator.h/cpp       - Random number generation (Box-Muller)
├── SABRSimulator.h/cpp         - SABR model path simulation
├── BermudanOption.h/cpp        - Option payoff and exercise dates
├── PolynomialRegression.h/cpp  - Least squares regression
├── LSMPricer.h/cpp             - Longstaff-Schwartz pricer
├── main.cpp                    - Main pricing program
├── test_random.cpp             - Random generator tests
├── Makefile                    - Build configuration
└── README.md                   - This file
```

## Compilation

### Prerequisites
- g++ compiler with C++11 support
- Make (optional, or compile manually)

### Using Make

```bash
# Compile all programs
make all

# Compile only main program
make main

# Compile only tests
make test_random

# Clean build files
make clean
```

### Manual Compilation

```bash
# Compile main program
g++ -std=c++11 -O2 -o main main.cpp RandomGenerator.cpp SABRSimulator.cpp BermudanOption.cpp PolynomialRegression.cpp LSMPricer.cpp

# Compile test program
g++ -std=c++11 -O2 -o test_random test_random.cpp RandomGenerator.cpp
```

## Usage

### Running Tests

First, verify the random number generator:

```bash
./test_random
```

Expected output:
- Mean ≈ 0.0 (within 0.05)
- Standard deviation ≈ 1.0 (within 0.05)
- Correlation matches target (within 0.05)

### Running Main Pricing Program

```bash
./main
```

This will:
1. Price a Bermudan call option with baseline parameters
2. Display the option price and standard error
3. Run a convergence analysis with different numbers of paths

### Baseline Parameters

| Parameter | Value | Description |
|-----------|-------|-------------|
| F₀ | 100.0 | Initial forward rate |
| α₀ | 0.20 | Initial volatility |
| β | 0.5 | Beta (CIR-type model) |
| ν | 0.4 | Vol-of-vol |
| ρ | -0.3 | Correlation |
| K | 100.0 | Strike (at-the-money) |
| r | 0.05 | Risk-free rate |
| Exercise dates | 0.25, 0.5, 0.75, 1.0 | Quarterly |

## Example Output

```
========================================
SABR Bermudan Option Pricing
Longstaff-Schwartz Monte Carlo Method
========================================

Model Parameters:
  F0 = 100
  alpha0 = 0.2
  beta = 0.5
  nu = 0.4
  rho = -0.3

Pricing Parameters:
  Number of paths = 10000
  Steps per period = 25
  Polynomial degree = 3

Simulating 10000 paths...
Running backward induction...

========================================
RESULTS
========================================
Option Price: 8.4532
Standard Error: 0.1234
95% Confidence Interval: [8.2113, 8.6951]
========================================
```

## Implementation Details

### Random Number Generation
- Uses **Box-Muller transformation** to generate standard normals
- Generates correlated pairs using: Z₂ = ρZ₁ + √(1-ρ²)Z₃
- Caches spare normal for efficiency

### SABR Simulation
- **Euler-Maruyama scheme** for SDE discretization
- **Positivity enforcement**: Truncates $F$ and $α$ at $0.001$ to prevent negative values
- Time step: $Δt$ = $T$ / (nExerciseDates $×$ stepsPerPeriod)

### Polynomial Regression
- **3rd degree polynomial** (customizable): $C(F) ≈ a₀ + a₁F + a₂F² + a₃F³$
- **Least squares**: Solves $(X^T X)a = X^T C$ using Gaussian elimination
- **Manual implementation**: No external libraries (Eigen, etc.)

### LSM Algorithm
- **Backward induction** from maturity to first exercise date
- **In-the-money filtering**: Regression only on paths with positive payoff
- **Exercise decision**: Exercise if payoff > predicted continuation value

## Convergence Analysis

The standard error should decrease as O(1/√N):

| # Paths | Std Error | Expected Improvement |
|---------|-----------|---------------------|
| 1,000 | ~0.40 | Baseline |
| 5,000 | ~0.18 | ÷√5 |
| 10,000 | ~0.13 | ÷√10 |
| 50,000 | ~0.06 | ÷√50 |

