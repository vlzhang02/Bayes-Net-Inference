/*
 * Node.h
 * by: Valerie Zhang
 *
 * Purpose: store all information about 
 *
 */
#ifndef _NODE_H
#define _NODE_H

#include "CPT.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Node {
public:
    Node();
    Node(string name, vector<string> value);
    ~Node();
    Node &operator=(const Node &other);

    string getName();
    string getAssignedVal();
    int getNumVal();
    int getIndex(string value);
    string getValue(int index);
    int getNumChildren();
    string getChild(int index);
    int getNumParents();
    string getParent(int index);
    double getProbability(string key, int index);

    void setName(string input);
    void setVal(string input);
    void addVal(string value);
    void addParent(string parent);
    void addChild(string child);
    void addToCPT(string line);

private:
    string name;
    string assignedValue;
    vector<string> values;
    vector<string> parents;
    vector<string> children;
    CPT cpt;
};
#endif