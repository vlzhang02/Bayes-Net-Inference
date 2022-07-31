###
### Makefile for Bayes Net Inference Program
###
### Author: Valerie Zhang
###

CXX      = clang++
CXXFLAGS = -g3 -Ofast -Wall -Wextra -std=c++11 

BayesNet:  main.o CPT.o Node.o BN.o Inference.o
	$(CXX) $(CXXFLAGS) -o $@ $^

Inference.o: Inference.cpp
	$(CXX) $(CXXFLAGS) -c $^
	
BN.o: BN.cpp
	$(CXX) $(CXXFLAGS) -c $^

Node.o: Node.cpp
	$(CXX) $(CXXFLAGS) -c $^
	
CPT.o: CPT.cpp
	$(CXX) $(CXXFLAGS) -c $^

unit_test: unit_test_driver.o CPT.o Node.o BN.o Inference.o
	$(CXX) $(CXXFLAGS) $^

clean: 
	rm *.o a.out *~ *#