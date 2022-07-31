/*
 * BN.h
 * by: Valerie Zhang
 *
 * Purpose: The BN class is a implementation of a Bayes Network which stores
 *          connections between variables and probability information. 
 */

#ifndef _BN_H_
#define _BN_H_

#include "CPT.h"
#include "Node.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

static const int IC = 100;

class BN {
public:
    BN();
    ~BN();

    Node* getEntry(string name);
    void openAndParse(string filename, vector<string>& vars);
    void reset();

private:
    int capacity;
    int numVar;
    Node *table;

    ifstream infile;

    /* setting up the table */
    void parseFile(vector<string>& vars);
    void addVar(string line, vector<string>& vars);
    void addPC(string line);
    void addCPT(string line);

    /* maintaining the table */
    void expand();
    void loadFactorCheck();

    /* hash functions */
    int hashIndex(string key);
    int linearProbeIndex(string key, int index);
};
#endif