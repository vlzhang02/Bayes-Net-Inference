/*
 * Inference.h
 * by: Valerie Zhang
 *
 * Purpose: Asks for user query and evidence, and returns probabilities based 
 *          on the information in the Bayes Network(BN)
 */
#ifndef _INFERENCE_H_
#define _INFERENCE_H_
#include "CPT.h"
#include "Node.h"
#include "BN.h"
#include <string>
#include <queue>
#include <utility>
#include <sstream>
#include <fstream>
#include <iomanip>

using namespace std;

class Inference {
public:
    Inference();
    Inference(string filename);
    ~Inference();

    void run(); 
private:
    BN BayesNet;
    vector<pair <string, double>> distribution;
    vector<pair <string, string>> evidence;
    vector<string> vars;

    string getQueryAndEvidence(string input);
    void eAsk(string query);
    bool inEvidence(string var);
    string determineKey(string var);
    double eAll(size_t count);
    double summation(string var, size_t count);
    void normalize();
    void printDistribution();
    int digits(double num);
    void reset();
};
#endif