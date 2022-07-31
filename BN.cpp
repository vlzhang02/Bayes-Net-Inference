/*
 * BN.cpp
 * by: Valerie Zhang
 *
 * Purpose: An implementation of BN class. A Bayes Network stores information
 *          about variable connections and probability.
 */

#include "BN.h"

using namespace std;

/*
 * default constructor
 */
BN::BN()
{
    capacity = IC;

    table          = new Node[capacity]; // table is a pointer
    Node start;
    for (int i = 0; i < capacity; i++) {
        table[i] = start;
    }
    numVar = 0;
}
/*
 * destructor
 */
BN::~BN() 
{
    delete [] table;
    numVar = 0;
}
/* 
 * openAndParseFile()
 * Purpose: opens and parses file
 * Parameters: filename
 * Returns: none
 */
void BN::openAndParse(string filename, vector<string>& vars) 
{
    infile.open(filename);
    if (!infile.is_open()) {
        cerr << "Error: could not open " << filename << "\n";
        exit(EXIT_FAILURE);
    }
    parseFile(vars);
    infile.close();
}
/*
 * parseFile()
 * Purpose:     parses file for variable, parent/children, and CPT info
 * Parameters:  address of variable order vector
 * Returns:     none
 */
void BN::parseFile(vector<string>& vars) 
{
    string line = "";
    int index;
    int lineCount = 0;
    int count = 0;
    while(getline(infile, line)) {
        lineCount++;
        if (line[0] == '#') {
            count++;
        } else if (count == 0) { // add variables first
            addVar(line, vars);
        } else if (count == 1) { // add children and parents
            addPC(line);
        } else if (count == 2) { // add to CPT
            if (!isdigit(line.back())) {
                string name = line; // get variable name
                index = linearProbeIndex(name, hashIndex(name));
            } else {
                table[index].addToCPT(line); 
            }
        }
    }
}
/*
 * addVar()
 * Purpose:     adds variable and its values to the table, and gets 
 *              variable order
 * Parameters:  line to parse
 * Returns:     none
 */
void BN::addVar(string line, vector<string>& vars) 
{
    loadFactorCheck();
    stringstream ss(line);
    string input;
    ss >> input; // get var name
    vars.push_back(input);
    int index = linearProbeIndex(input, hashIndex(input));
    table[index].setName(input); // add var to table
    while (ss >> input) { // get each possible value
        table[index].addVal(input); // add value
    }
    numVar++; // increment number of variables
}
/*
 * addPC()
 * Purpose:     adds parents and child to specified var to the table
 * Parameters:  line to parse
 * Returns:     none
 */
void BN::addPC(string line) 
{
    stringstream ss(line);
    string child;
    string input;
    ss >> child; // get child
    int index = linearProbeIndex(child, hashIndex(child)); // get child's index
    while (ss >> input) { // get each parent
        table[index].addParent(input); // assign parent to child
        // assign child to parent
        table[linearProbeIndex(input, hashIndex(input))].addChild(child);
    } 
}
/*
 * getEntry()
 * Purpose:     get pointer to variable's entry in table
 * Parameters:  variable name
 * Returns:     pointer to entry
 */
Node* BN::getEntry(string name) 
{
    return &(table[linearProbeIndex(name, hashIndex(name))]);
}
/*
 * reset()
 * Purpose:     reset BN
 * Parameters:  none
 * Returns:     none
 */
void BN::reset()
{
    for (int i = 0; i < capacity; i++) {
        if (table[i].getAssignedVal() != "NONE") {
            table[i].setVal("NONE"); // set all assigned assignedValues to "NONE"
        }
    }
}
/** *****************************************************************\
*                   hashtable maintaining functions                  *
\********************************************************************/
/*                           void expand()
 * parameters:  none
 * purpose:     expands the capacity and rehashes all cities
 * returns:     n/a
 */
void BN::expand()
{
    int        oldCapacity = capacity;
    Node *oldTable    = table; // save old table to delete it

    capacity = 2 * capacity + 2;        // update capacity
    table    = new Node[capacity]; // create new table
    Node n;
    for (int i = 0; i < capacity; i++) {
        table[i] = n;
    }

    for (int i = 0; i < oldCapacity; i++) {
        if (oldTable[i].getName() != "NONE") { // if keywrd empty: continue,
            Node curr = oldTable[i]; // else, make copy of word entry,
            string    key  = curr.getName(); // & put into new table
            // linear probe index uses new table
            table[linearProbeIndex(key, hashIndex(key))] = curr;
        }
    }
    delete [] oldTable;
}

/*                           void loadFactorCheck()
 * parameters:  none
 * purpose:     checks the load factor and rehashes if necessary
 * returns:     n/a
 */
void BN::loadFactorCheck()
{
    double c  = capacity * 1.0;
    double nc = numVar * 1.0;
    if (nc / c > .7) {
        expand();
    }
} 

/** *****************************************************************\
*                           hash functions                           *
\********************************************************************/

/*             int linearProbeIndex(string key, int index)
 * parameters:
 *  -   int index represents the original hash index for a key
 *  -   string &key is a key to find the index for
 * purpose: to find the correct index for a key using linear probing
 * returns: an integer representing the key's index
 */
int BN::linearProbeIndex(string key, int index)
{
    int  attempt = 0;
    bool found   = false;
    while (!found) {
        index = (index + attempt) % capacity;
        if (table[index].getName() == "NONE") {
            return index;
        } else if (table[index].getName() == key) {
            return index;
        }
        attempt++;
    }
    return 0;
}

/*                        int hashIndex(string &key)
 * parameters:
 *  -   string &key is a key to find an index for
 * purpose: calls the hash function on the key, then mods it to get an
 *          index
 * returns: the index corresponding to the key
 */
int BN::hashIndex(string key)
{
    // first get hash value for key
    // then mod by capacity to get index
    return (hash<string>{}(key) % capacity);
}