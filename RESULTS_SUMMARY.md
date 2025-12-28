# SABR Results Discussion

## Sensitivity Analysis Results 

### 1. Beta (β) Sensitivity - **VERY STRONG EFFECT**

| Beta | Price | Change | Model Type |
|------|-------|--------|------------|
| 0.0 | **0.0765** | Baseline | Normal vol |
| 0.3 | **0.3055** | +299% | Intermediate |
| 0.5 | **0.7683** | +904% | CIR (standard) |
| 0.7 | **1.8994** | +2,382% | Intermediate |
| 1.0 | **7.5498** | +9,767% | Lognormal |

 β has a great impact. Price increases exponentially with β.
because Higher β → volatility scales with F → more upside potential


---

### 2. Nu (ν) Sensitivity - **MILD EFFECT**

| Nu | Price | Change | Vol-of-Vol |
|----|-------|--------|------------|
| 0.1 | **0.7795** | Baseline | Low |
| 0.2 | **0.7776** | -0.24% | Medium |
| 0.4 | **0.7771** | -0.31% | Baseline |
| 0.6 | **0.7991** | +2.51% | High |
| 0.8 | **0.8167** | +4.77% | Very High |

 ν has modest impact (only 5% range for 8× change in ν)

---

### 3. Rho (ρ) Sensitivity - **MINIMAL EFFECT**

| Rho | Price | Change | Interpretation |
|-----|-------|--------|----------------|
| -0.7 | **0.7756** | -0.26% | Strong negative |
| -0.3 | **0.7776** | Baseline | Typical rates |
| 0.0 | **0.7815** | +0.50% | No correlation |
| 0.3 | **0.7828** | +0.67% | Positive |
| 0.7 | **0.7701** | -0.96% | Strong positive |

 ρ has very small impact (< 1% variation)

---

### 4. Strike (K) Sensitivity - **AS EXPECTED**

| Strike | Moneyness | Price | Intrinsic | Time Value |
|--------|-----------|-------|-----------|------------|
| 90 | Deep ITM | **9.8713** | 10.00 | -0.13* |
| 95 | ITM | **4.9381** | 5.00 | -0.06 |
| 100 | ATM | **0.7725** | 0.00 | +0.77 |
| 105 | OTM | **0.0065** | 0.00 | +0.01 |
| 110 | Deep OTM | **0.0001** | 0.00 | +0.00 |

Slightly below intrinsic due to early exercise discounting

Perfect gradient from ITM → OTM
ITM options:
Price ≈ Intrinsic Value (early exercise likely)
ATM options:
Pure time value ≈ 0.77
OTM options:
Very low probability → near zero price
