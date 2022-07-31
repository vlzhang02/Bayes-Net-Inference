/*
 * CPT.h
 * by: Valerie Zhang
 *
 * Purpose: Stores conditional probabilities of a variable in a Bayes Net
 */

#ifndef _CPT_H_
#define _CPT_H_

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <functional>

using namespace std;

static const int INITIAL_CAPACITY = 100;

class CPT {
public:
    CPT();
    ~CPT();
    CPT &operator=(const CPT &other);
    
    int numProbabilities(int index) const;

    double getProbability(string key, int index) const;
    string getKey(int index) const;
    void parseLine(string line);

private:
    struct entry {
        string key; // parent values
        vector<double> probabilities;
    };

    int capacity;
    int numPs;
    entry* table;

    bool isDecimal(string s);
    void finalProbability();
    void add(string key, double probability);

    /* maintaining the table */
    void expand();
    void loadFactorCheck();

    /* hash functions */
    int hashIndex(string key) const;
    int linearProbeIndex(string key, int index) const;
};
#endif