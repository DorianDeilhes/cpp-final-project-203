# Makefile for SABR Bermudan Option Pricing

CXX = g++
CXXFLAGS = -Wall -O2 -std=c++11

# Object files
OBJS = RandomGenerator.o SABRSimulator.o BermudanOption.o PolynomialRegression.o LSMPricer.o PricingResults.o

# Executables
TARGETS = main test_random sensitivity_analysis

all: $(TARGETS)

# Main program
main: main.o $(OBJS)
	$(CXX) $(CXXFLAGS) -o main main.o $(OBJS)

# Test programs
test_random: test_random.o RandomGenerator.o
	$(CXX) $(CXXFLAGS) -o test_random test_random.o RandomGenerator.o

sensitivity_analysis: sensitivity_analysis.o $(OBJS)
	$(CXX) $(CXXFLAGS) -o sensitivity_analysis sensitivity_analysis.o $(OBJS)

# Object file compilation
RandomGenerator.o: RandomGenerator.cpp RandomGenerator.h
	$(CXX) $(CXXFLAGS) -c RandomGenerator.cpp

SABRSimulator.o: SABRSimulator.cpp SABRSimulator.h RandomGenerator.h
	$(CXX) $(CXXFLAGS) -c SABRSimulator.cpp

BermudanOption.o: BermudanOption.cpp BermudanOption.h
	$(CXX) $(CXXFLAGS) -c BermudanOption.cpp

PolynomialRegression.o: PolynomialRegression.cpp PolynomialRegression.h
	$(CXX) $(CXXFLAGS) -c PolynomialRegression.cpp

LSMPricer.o: LSMPricer.cpp LSMPricer.h SABRSimulator.h BermudanOption.h PolynomialRegression.h PricingResults.h
	$(CXX) $(CXXFLAGS) -c LSMPricer.cpp

PricingResults.o: PricingResults.cpp PricingResults.h
	$(CXX) $(CXXFLAGS) -c PricingResults.cpp

main.o: main.cpp SABRSimulator.h BermudanOption.h LSMPricer.h
	$(CXX) $(CXXFLAGS) -c main.cpp

test_random.o: test_random.cpp RandomGenerator.h
	$(CXX) $(CXXFLAGS) -c test_random.cpp

sensitivity_analysis.o: sensitivity_analysis.cpp SABRSimulator.h BermudanOption.h LSMPricer.h
	$(CXX) $(CXXFLAGS) -c sensitivity_analysis.cpp

# Clean build files
clean:
	rm -f *.o $(TARGETS)

# Run tests
test: test_random
	./test_random

# Run main program
run: main
	./main

.PHONY: all clean test run
