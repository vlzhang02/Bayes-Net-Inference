/*
 * CPT.cpp
 * by: Valerie Zhang
 *
 * Purpose: An implementation of CPT class. A CPT stores conditional
 *          probabilities 
 */

#include "CPT.h"

using namespace std;
/*
 * default constructor
 */
CPT::CPT() 
{
    capacity = INITIAL_CAPACITY;

    table =  new entry[capacity];
    entry e;
    e.key = "NONE";
    for (int i = 0; i < capacity; i++) {
        table[i] = e;
    }
    numPs = 0;
}
/*
 * destructor
 */
CPT::~CPT() 
{
    delete [] table;
    numPs = 0;
}
/*
 * &operator=()
 * Purpose:     defines assignment operator to make a deep copy of the
 *              instance on the right hand side(rhs) into the instance on the
 *              left hand side(lhs)
 * Arguments:   reference to instance on the rhs
 * Returns:     reference to the lhs
 */
CPT &CPT::operator=(const CPT &other) 
{
    if (this == &other) {return *this;}
    delete [] table;
    table =  new entry[capacity];
    entry e;
    e.key = "NONE";
    for (int i = 0; i < capacity; i++) {
        table[i] = e;
    }
    // copy elements
    capacity = other.capacity;
    numPs = other.numPs;
    for (int i = 0; i < capacity; i++) {
        table[i].key = other.getKey(i);
        for (int j = 0; j < other.numProbabilities(i); j++) {
            table[i].probabilities.push_back(other.getProbability(table[i].key, j));
        }
    }
    return *this;
}
/*
 * getKey()
 * Purpose:     get key at particular index
 * Parameters:  index
 * Returns:     string key
 */
string CPT::getKey(int index) const
{
    return table[index].key;
}
/*
 * getProbability()
 * Purpose:     get probability of variable with specific parent values
 * Parameters:  string key
 * Returns:     conditional probabiltiy
 */
double CPT::getProbability(string key, int index) const
{
    return table[linearProbeIndex(key, hashIndex(key))].probabilities[index];
}
/*
 * numProbabilities()
 * Purpose:     gets total number of probabilities in an entry
 * Parameters:  index in table   
 * Returns:     number of probabilities in an entry
 */
int CPT::numProbabilities(int index) const 
{
    return table[index].probabilities.size();
}
/*
 * add()
 * Purpose:     add conditional probability to table
 * Parameters:  string key and probability
 * Returns:     none
 */
void CPT::add(string key, double probability) 
{
    loadFactorCheck();
    int index = linearProbeIndex(key, hashIndex(key));
    table[index].key = key;
    table[index].probabilities.push_back(probability);
}
/*
 * parseLine()
 * Purpose:     parses line for key and probabilities
 * Parameters:  line to parse
 * Returns:     none
 */
void CPT::parseLine(string line) 
{
    stringstream ss(line);
    string key = "";
    string toCheck = "";
    double total = 0; // use to get probability for last value
    while (ss >> toCheck) {
        if(isDecimal(toCheck)) {
            if (key.length() == 0) { // when variables has no parents
                key = "NULL";
            }
            total += stod(toCheck); // add to total
            add(key, stod(toCheck)); // add to CPT
        } else {
            key += toCheck;
        }
    }
    add(key, 1 - total); // adds missing probability to table
    numPs++;
}
/*
 * isNum()
 * Purpose:     checks if string is a number
 * Parameters:  string to check
 * Returns:     true if a number, false if not
 */
bool CPT::isDecimal(string s)
{
    for (size_t i = 0; i < s.length(); i++) {
        if (s[i] == '.') {return true;}
    }
    return false;
}
 /********************************************************************\
*                   hashtable maintaining functions                  *
\********************************************************************/

/* expand()
 * Parameters:  none
 * Purpose:     expands the capacity and rehashes all entries
 * Returns:     n/a
 */
void CPT::expand() 
{
    int        oldCapacity = capacity;
    entry *oldTable        = table; // save old table to delete it

    capacity = 2 * capacity + 2;        // update capacity
    table    = new entry[capacity]; // create new table
    entry e;
    e.key = "NONE";
    for (int i = 0; i < capacity; i++) { // initialized with empty symbol
        table[i] = e;
    }
    for (int i = 0; i < oldCapacity; i++) {
        if (oldTable[i].key != "NONE") { // if entry empty: continue,
            string key = oldTable[i].key;
            int index = linearProbeIndex(key, hashIndex(key));
            // copy over contents in table
            table[index].key = oldTable[i].key;
            for (size_t j = 0; j < oldTable[i].probabilities.size(); j++) {
                table[index].probabilities.push_back(oldTable[i].probabilities[j]);
            }
        }
    }
    delete [] oldTable;
}
/* loadFactorCheck()
 * Parameters:  none
 * Purpose:     checks the load factor and rehashes if necessary
 * Returns:     n/a
 */
void CPT::loadFactorCheck()
{
    double c  = capacity * 1.0;
    double nc = numPs * 1.0;
    if (nc / c > .7) {
        expand();
    }
}
/********************************************************************\
*                           hash functions                           *
\********************************************************************/

/* linearProbeIndex()
 * parameters:
 *  -   int index represents the original hash index for a key
 *  -   string &key is a key to find the index for
 * purpose:     to find the correct index for a key using linear probing
 * returns:     an integer representing the key's index
 */
int CPT::linearProbeIndex(string key, int index) const
{
    int  attempt = 0;
    bool found   = false;
    while (!found) {
        index = (index + attempt) % capacity;
        if (table[index].key == "NONE") { // if index is empty
            return index;
        } else if (table[index].key == key) { // if clause matches
            return index;
        }
        attempt++;
    }
    return 0;
}
/* hashIndex()
 * parameters:
 *  -   string &key is a key to find an index for
 * purpose: calls the hash function on the key, then mods it to get an
 *          index
 * returns: the index corresponding to the key
 */
int CPT::hashIndex(string key) const
{
    // first get hash value for key
    // then mod by capacity to get index
    return (hash<string>{}(key) % capacity);
}