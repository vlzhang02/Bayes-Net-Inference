/*
 * Inference.cpp
 * by: Valerie Zhang
 *
 * Purpose: An implementation of Inference class. 
 *
 */
#include "Inference.h"

using namespace std;

Inference::Inference() {}

Inference::Inference(string filename) 
{
    cout << "\nLoading file \"" << filename << "\"\n\n";
    BayesNet.openAndParse(filename, vars);
}

Inference::~Inference() 
{
    distribution.clear();
    evidence.clear();
}

void Inference::run() 
{
    string input = "";
    while (getline(cin, input)) {
        if (input == "quit") {break;}
        string query = getQueryAndEvidence(input); // get query variable
        eAsk(query); // run algorithm
        printDistribution(); // print distribution
        reset();
    }
}

string Inference::getQueryAndEvidence(string input) {
    stringstream ss(input);
    string query;
    string word;
    ss >> query; // get query variable name
    pair<string, string> e;
    int count = -1;
    while (ss >> word) {
        if (word == "|") {count++;}
        else if (word == "=") {count++;}
        else if (count == 0) { // get evidence variable name
            e.first = word;
            count++;
        } else if (count == 2) { // get evidence variable value
            if (word.length() == 2) {
                word.erase(word.begin()+1); // trim commas
            }
            e.second = word;
            Node* curr = BayesNet.getEntry(e.first);
            curr->setVal(word); // sets values in Bayes Net
            evidence.push_back(e); // add to evidence
            count = 0;
        }
    }
    return query;
}
/*
 * eAsk()
 * Purpose:     fill distribution table for query variable(before normalization)
 * Parameters:  query variable
 * Returns:     none
 */
void Inference::eAsk(string query)
{
    Node* curr = BayesNet.getEntry(query);
    pair<string, double> toAdd;
    for (int i = 0; i < curr->getNumVal() - 1; i++) { // for each possible value of query
        toAdd.first = curr->getValue(i);
        pair<string, string> e = {query, toAdd.first};
        evidence.push_back(e); // adds X = xi to evidence table
        curr->setVal(toAdd.first);
        toAdd.second = eAll(0); // get P(xi, e)
        distribution.push_back(toAdd); // add to distribution table
        evidence.pop_back(); // removes values from evidence table
        curr->setVal("NONE");
    }
    toAdd = {curr->getValue(curr->getNumVal() - 1), 0}; 
    distribution.push_back(toAdd); // add last entry with value of 0
    normalize();
}
/*
 * eAll()
 * Purpose:     calculates P(xi,e) for distribution using BN
 * Parameters:  none
 * Returns:     P(xi,e)
 */
double Inference::eAll(size_t count)
{
    if (count == vars.size()) { // reached end of vector
        return 1.0;
    }
    string var = vars[count]; // get variable
    if (inEvidence(var)) { // if in evidence
        string key = determineKey(var); // get key
        Node* curr = BayesNet.getEntry(var); 
        double num = curr->getProbability(key, curr->getIndex(curr->getAssignedVal())); // get probability
        return num * eAll(count+1); // recurse
    } else {
        return summation(var, count); // perform summation
    }
}
/*
 * summation()
 * Purpose:     calculates summation of probababilities of a variable 
 *              given its parents
 * Parameters:  variable whose probabilities are being summed and var count
 * Returns:     sum
 */
double Inference::summation(string var, size_t count) 
{
    Node* curr = BayesNet.getEntry(var);
    if (curr->getNumChildren() == 0) { //
        return 1.0;
    }
    double sum = 0;
    for (int i = 0; i < curr->getNumVal(); i++) {
        curr->setVal(curr->getValue(i)); // assign value in BayesNet
        pair<string, string> e = {var, curr->getAssignedVal()};
        evidence.push_back(e); // add variable with assigned value to evidence set
        sum += eAll(count);
        curr->setVal("NONE"); // reset value in BayesNet
        evidence.pop_back(); // remove entry from evidence set
    }
    return sum;
}
/*
 * inEvidence()
 * Purpose:     determines if variable is in evidence
 * Parameters:  variable
 * Returns:     true if in evidence, false if not
 */
bool Inference::inEvidence(string var) 
{
    for (size_t i = 0; i < evidence.size(); i++) {
        if (evidence[i].first == var) {
            return true;
        }
    }
    return false;
}
/*
 * determineKey()
 * Purpose:     generates key based on a variable's parents' values for CPT
 * Parameters:  variable 
 * Returns:     key
 */
string Inference::determineKey(string var) 
{
    string key = "";
    Node* curr = BayesNet.getEntry(var);
    if (curr->getNumParents() == 0) {
        key = "NULL";
        return key;
    }
    for (int i = 0; i < curr->getNumParents(); i++) {
        // get parent's assigned value
        Node* check = BayesNet.getEntry(curr->getParent(i)); 
        key += check->getAssignedVal(); // concatenate to key
    }
    return key;
}
/*
 * normalize()
 * Purpose:     normalizes probabilities in distribution
 * Parameters:  none
 * Returns:     none
 */
void Inference::normalize()
{
    double nConstant = eAll(0); // get normalization constant
    double final = 0; 
    for (size_t i = 0; i < distribution.size() - 1; i++) { 
        distribution[i].second = distribution[i].second/nConstant; // normalize
        final += distribution[i].second; // add all values together
    }
    // get final value by subtracting sum from 1
    distribution[distribution.size() - 1].second = 1 - final; 
}
/*
 * printDistribution()
 * Purpose:     print values in distribution
 * Parameters:  none
 * Returns:     none
 */
void Inference::printDistribution() 
{
    for (size_t i = 0; i < distribution.size(); i++) {
        cout << "P(" <<  distribution[i].first << ") = "; 
        cout << setprecision(digits(distribution[i].second)) << distribution[i].second;
        if (i != distribution.size() - 1) {
            cout << ", ";
        }
    }
    cout << "\n\n";
}
/*
 * digits()
 * Purpose:     determines number of digits to be printed
 * Parameters:  number for print
 * Returns:     number of digits to print
 */
int Inference::digits(double num) {
    string s = to_string(num);
    if ((s[2] == '0') and (s[3] != '0') and (s[5] != '0')) { // for 0.0##0 case
        return 2;
    } else if ((s[2] == '0') and (s[3] == '0') and (s[5] != '0')) { // for 0.00#0 case
        return 1;
    }
    return 3;
}
/*
 * reset()
 * Purpose:     resets instance for new query
 * Parameters:  none
 * Returns:     none
 */
void Inference::reset() {
    distribution.clear();
    evidence.clear();
    BayesNet.reset();
}
